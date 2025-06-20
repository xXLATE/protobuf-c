#!/usr/bin/env python3
"""
protoc-gen-pawn: Protocol Buffer compiler plugin for Pawn language

This script serves as a protoc plugin to generate Pawn include files
from Protocol Buffer definitions.

Usage:
  protoc --plugin=protoc-gen-pawn=protoc-gen-pawn.py --pawn_out=. test.proto
"""

import sys
import os
from google.protobuf.compiler import plugin_pb2
from google.protobuf.descriptor_pb2 import FileDescriptorProto


class PawnGenerator:
    """Generates Pawn code from protobuf descriptors"""
    
    def __init__(self):
        self.generated_files = []
    
    def generate_file(self, file_desc: FileDescriptorProto) -> str:
        """Generate Pawn code for a single proto file"""
        output = []
        
        # Header guard
        guard_name = self._get_guard_name(file_desc.name)
        output.append(f"/* Generated from {file_desc.name} */")
        output.append(f"#if defined {guard_name}")
        output.append("  #endinput")
        output.append("#endif")
        output.append(f"#define {guard_name}")
        output.append("")
        
        # Generate enums
        for enum_desc in file_desc.enum_type:
            output.extend(self._generate_enum(enum_desc))
            output.append("")
        
        # Generate messages
        for message_desc in file_desc.message_type:
            output.extend(self._generate_message(message_desc))
            output.append("")
        
        # Generate services
        for service_desc in file_desc.service:
            output.extend(self._generate_service(service_desc))
            output.append("")
        
        output.append("/* End of generated file */")
        return "\n".join(output)
    
    def _get_guard_name(self, filename: str) -> str:
        """Generate include guard name from filename"""
        base = os.path.splitext(os.path.basename(filename))[0]
        return f"_protobuf_{base.replace('-', '_').replace('.', '_')}_included"
    
    def _generate_enum(self, enum_desc) -> list:
        """Generate Pawn constants for enum"""
        output = []
        enum_name = enum_desc.name.upper()
        
        output.append(f"/* {enum_desc.name} enum values */")
        
        for value in enum_desc.value:
            constant_name = f"{enum_name}__{value.name}"
            output.append(f"#define {constant_name} {value.number}")
        
        return output
    
    def _generate_message(self, message_desc) -> list:
        """Generate Pawn constants for message"""
        output = []
        message_name = message_desc.name
        
        output.append(f"/* {message_name} message field tags */")
        
        for field in message_desc.field:
            tag_name = f"{message_name.upper()}_FIELD_{field.name.upper()}"
            output.append(f"#define {tag_name} {field.number}")
        
        # Generate nested enums
        for nested_enum in message_desc.enum_type:
            output.append("")
            output.extend(self._generate_enum(nested_enum))
        
        # Generate nested messages
        for nested_message in message_desc.nested_type:
            output.append("")
            output.extend(self._generate_message(nested_message))
        
        return output
    
    def _generate_service(self, service_desc) -> list:
        """Generate Pawn constants for service"""
        output = []
        service_name = service_desc.name
        
        output.append(f"/* {service_name} service method IDs */")
        
        for i, method in enumerate(service_desc.method):
            method_id = f"{service_name.upper()}_METHOD_{method.name.upper()}"
            output.append(f"#define {method_id} {i}")
        
        return output


def main():
    """Main entry point for protoc plugin"""
    try:
        # Read request from stdin (binary protobuf data)
        data = sys.stdin.buffer.read()
        request = plugin_pb2.CodeGeneratorRequest()
        request.ParseFromString(data)
        
        # Create response
        response = plugin_pb2.CodeGeneratorResponse()
        
        # Generate files
        generator = PawnGenerator()
        
        for file_name in request.file_to_generate:
            # Find the file descriptor
            file_desc = None
            for proto_file in request.proto_file:
                if proto_file.name == file_name:
                    file_desc = proto_file
                    break
            
            if file_desc is None:
                continue
            
            # Generate output
            content = generator.generate_file(file_desc)
            
            # Create output file
            output_file = response.file.add()
            output_file.name = file_name.replace('.proto', '.pb-pawn.inc')
            output_file.content = content
        
        # Write response to stdout (binary protobuf data)
        sys.stdout.buffer.write(response.SerializeToString())
        
    except Exception as e:
        # Error handling
        response = plugin_pb2.CodeGeneratorResponse()
        response.error = f"protoc-gen-pawn error: {str(e)}"
        sys.stdout.buffer.write(response.SerializeToString())
        return 1
    
    return 0


if __name__ == '__main__':
    sys.exit(main()) 