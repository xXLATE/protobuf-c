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

#include "pawn_helpers.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <google/protobuf/descriptor.h>
#include <absl/strings/str_split.h>
#include <absl/strings/ascii.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace pawn {

namespace {

// Helper functions to replace protobuf internal utilities
bool HasSuffixString(const std::string& str, const std::string& suffix) {
  return str.size() >= suffix.size() &&
         str.substr(str.size() - suffix.size()) == suffix;
}

std::string StripSuffixString(const std::string& str, const std::string& suffix) {
  if (HasSuffixString(str, suffix)) {
    return str.substr(0, str.size() - suffix.size());
  }
  return str;
}

std::string SimpleItoa(int32_t value) {
  return std::to_string(value);
}

std::string SimpleItoa(int64_t value) {
  return std::to_string(value);
}

std::string SimpleItoa(uint32_t value) {
  return std::to_string(value);
}

std::string SimpleItoa(uint64_t value) {
  return std::to_string(value);
}

std::string SimpleFtoa(float value) {
  return std::to_string(value);
}

std::string SimpleDtoa(double value) {
  return std::to_string(value);
}

std::string CEscape(const std::string& str) {
  std::string result;
  for (char c : str) {
    switch (c) {
      case '\n': result += "\\n"; break;
      case '\r': result += "\\r"; break;
      case '\t': result += "\\t"; break;
      case '\"': result += "\\\""; break;
      case '\\': result += "\\\\"; break;
      default: result += c; break;
    }
  }
  return result;
}

std::string StringToUpper(const std::string& str) {
  return absl::AsciiStrToUpper(str);
}

std::string DotsToUnderscores(const std::string& name) {
  std::string result = name;
  std::replace(result.begin(), result.end(), '.', '_');
  return result;
}

}  // anonymous namespace

std::string StripProto(const std::string& filename) {
  if (HasSuffixString(filename, ".protodevel")) {
    return StripSuffixString(filename, ".protodevel");
  } else {
    return StripSuffixString(filename, ".proto");
  }
}

std::string FilenameToPawnInclude(const std::string& filename) {
  return StripProto(filename) + ".inc";
}

std::string FilenameToPawnGuard(const std::string& filename) {
  std::string basename = StripProto(filename);
  std::replace(basename.begin(), basename.end(), '/', '_');
  std::replace(basename.begin(), basename.end(), '.', '_');
  
  std::string result = "_";
  result += basename;
  result += "_included";
  return result;
}

std::string EnumName(const EnumDescriptor* enum_descriptor) {
  return "E_" + StringToUpper(DotsToUnderscores(enum_descriptor->name()));
}

std::string EnumValueName(const EnumValueDescriptor* enum_value) {
  return StringToUpper(enum_value->type()->name()) + "_" + StringToUpper(enum_value->name());
}

std::string StringifyDefaultValue(const FieldDescriptor* field) {
  switch (field->type()) {
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_SFIXED32:
      return SimpleItoa(field->default_value_int32());
    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_SFIXED64:
      return SimpleItoa(field->default_value_int64());
    case FieldDescriptor::TYPE_UINT32:
    case FieldDescriptor::TYPE_FIXED32:
      return SimpleItoa(field->default_value_uint32());
    case FieldDescriptor::TYPE_UINT64:
    case FieldDescriptor::TYPE_FIXED64:
      return SimpleItoa(field->default_value_uint64());
    case FieldDescriptor::TYPE_FLOAT:
      return SimpleFtoa(field->default_value_float());
    case FieldDescriptor::TYPE_DOUBLE:
      return SimpleDtoa(field->default_value_double());
    case FieldDescriptor::TYPE_BOOL:
      return field->default_value_bool() ? "true" : "false";
    case FieldDescriptor::TYPE_STRING:
    case FieldDescriptor::TYPE_BYTES:
      return "\"" + CEscape(field->default_value_string()) + "\"";
    case FieldDescriptor::TYPE_ENUM:
      return EnumValueName(field->default_value_enum());
    case FieldDescriptor::TYPE_MESSAGE:
      return "/* message field default not supported */";
  }
  // Never reaches here
  return "";
}

std::string MessageEnumName(const Descriptor* descriptor) {
  return "E_" + StringToUpper(descriptor->name()) + "_DATA";
}

std::string MessageFieldName(const FieldDescriptor* field) {
  return "P_" + StringToUpper(field->name());
}

}  // namespace pawn
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
