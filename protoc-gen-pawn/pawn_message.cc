// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.
//
// Copyright (c) 2008-2013, Dave Benson.  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Modified for Pawn by the protobuf-pawn project

#include "pawn_message.h"
#include "pawn_enum.h"
#include "pawn_helpers.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace pawn {

MessageGenerator::MessageGenerator(const Descriptor* descriptor)
  : descriptor_(descriptor) {
}

MessageGenerator::~MessageGenerator() {}

void MessageGenerator::GenerateStructDefinition(io::Printer* printer) {
  std::string struct_name = StructName(descriptor_);
  
  printer->Print("// $full_name$\n", "full_name", descriptor_->full_name());
  printer->Print("enum struct $struct_name$ {\n", "struct_name", struct_name);
  printer->Indent();
  
  // Generate fields
  for (int i = 0; i < descriptor_->field_count(); i++) {
    const FieldDescriptor* field = descriptor_->field(i);
    GenerateFieldDefinition(field, printer);
  }
  
  printer->Outdent();
  printer->Print("}\n\n");
}

void MessageGenerator::GenerateFieldDefinition(const FieldDescriptor* field, io::Printer* printer) {
  std::string field_type = PawnFieldType(field);
  std::string field_name = FieldName(field);
  
  if (field->is_repeated()) {
    // For repeated fields, we'll use a simple array approach
    printer->Print("$type$ $name$[1024]; // repeated field\n", 
                   "type", field_type,
                   "name", field_name);
    printer->Print("int $name$_count;\n",
                   "name", field_name);
  } else {
    printer->Print("$type$ $name$;\n",
                   "type", field_type,
                   "name", field_name);
  }
}

std::string MessageGenerator::PawnFieldType(const FieldDescriptor* field) {
  switch (field->type()) {
    case FieldDescriptor::TYPE_DOUBLE:
    case FieldDescriptor::TYPE_FLOAT:
      return "float";
    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_UINT64:
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_UINT32:
    case FieldDescriptor::TYPE_SFIXED32:
    case FieldDescriptor::TYPE_SFIXED64:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_SINT64:
      return "int";
    case FieldDescriptor::TYPE_BOOL:
      return "bool";
    case FieldDescriptor::TYPE_STRING:
    case FieldDescriptor::TYPE_BYTES:
      return "char[256]"; // Fixed size for now
    case FieldDescriptor::TYPE_ENUM:
      return EnumName(field->enum_type());
    case FieldDescriptor::TYPE_MESSAGE:
      return StructName(field->message_type());
    default:
      return "int"; // fallback
  }
}

void MessageGenerator::GenerateEnumDefinitions(io::Printer* printer) {
  // Generate nested enums
  for (int i = 0; i < descriptor_->enum_type_count(); i++) {
    EnumGenerator enum_generator(descriptor_->enum_type(i));
    enum_generator.GenerateDefinition(printer);
  }
}

void MessageGenerator::GenerateNestedMessageDefinitions(io::Printer* printer) {
  // Generate nested messages
  for (int i = 0; i < descriptor_->nested_type_count(); i++) {
    MessageGenerator nested_generator(descriptor_->nested_type(i));
    nested_generator.GenerateStructDefinition(printer);
  }
}

}  // namespace pawn
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
