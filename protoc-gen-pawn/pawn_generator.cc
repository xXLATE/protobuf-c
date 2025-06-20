// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
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

// Copyright (c) 2008-2025, Dave Benson and the protobuf-c authors.
// All rights reserved.
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

// Modified to implement Pawn code by AI Assistant.

#include <memory>
#include <utility>
#include <vector>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <protobuf-c/protobuf-c.pb.h>
#include "pawn_file.h"
#include "pawn_generator.h"
#include "pawn_helpers.h"

namespace protobuf_pawn {

// Parses a set of comma-delimited name/value pairs, e.g.:
//   "foo=bar,baz,qux=corge"
// parses to the pairs:
//   ("foo", "bar"), ("baz", ""), ("qux", "corge")
void ParseOptions(const std::string& text, std::vector<std::pair<std::string, std::string> >* output) {
  std::vector<std::string> parts;
  SplitStringUsing(text, ",", &parts);

  for (unsigned i = 0; i < parts.size(); i++) {
    std::string::size_type equals_pos = parts[i].find_first_of('=');
    std::pair<std::string, std::string> value;
    if (equals_pos == std::string::npos) {
      value.first = parts[i];
      value.second = "";
    } else {
      value.first = parts[i].substr(0, equals_pos);
      value.second = parts[i].substr(equals_pos + 1);
    }
    output->push_back(value);
  }
}

PawnGenerator::PawnGenerator() {}
PawnGenerator::~PawnGenerator() {}

bool PawnGenerator::Generate(const google::protobuf::FileDescriptor* file,
                            const std::string& parameter,
                            google::protobuf::compiler::OutputDirectory* output_directory,
                            std::string* error) const {
  if (file->options().GetExtension(pb_c_file).no_generate())
    return true;

  std::vector<std::pair<std::string, std::string> > options;
  ParseOptions(parameter, &options);

  // -----------------------------------------------------------------
  // parse generator options

  // TODO(kenton):  If we ever have more options, we may want to create a
  //   class that encapsulates them which we can pass down to all the
  //   generator classes.  Currently we pass dllexport_decl down to all of
  //   them via the constructors, but we don't want to have to add another
  //   constructor parameter for every option.

  // If the dllexport_decl option is passed to the compiler, we need to write
  // it in front of every symbol that should be exported if this .proto is
  // compiled into a Windows DLL.  E.g., if the user invokes the protocol
  // compiler as:
  //   protoc --pawn_out=dllexport_decl=FOO_EXPORT:outdir foo.proto
  // then we'll define constants like this:
  //   const FOO_EXPORT PERSON_ID = 1;
  // FOO_EXPORT is a macro which should expand to appropriate export declaration.
  std::string dllexport_decl;

  for (unsigned i = 0; i < options.size(); i++) {
    if (options[i].first == "dllexport_decl") {
      dllexport_decl = options[i].second;
    } else {
      *error = "Unknown generator option: " + options[i].first;
      return false;
    }
  }

  // -----------------------------------------------------------------

  std::string basename = StripProto(file->name());
  basename.append(".pb-pawn");

  FileGenerator file_generator(file, dllexport_decl);

  // Generate Pawn include file.
  {
    std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> output(
      output_directory->Open(basename + ".inc"));
    google::protobuf::io::Printer printer(output.get(), '$');
    file_generator.GeneratePawnSource(&printer);
  }

  return true;
}

}  // namespace protobuf_pawn
