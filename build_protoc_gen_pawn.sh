#!/bin/bash

# Simple build script for protoc-gen-pawn

echo "Building protoc-gen-pawn..."

# Create output directory
mkdir -p build

# Compile the sources
g++ -std=c++17 -I. -Iprotoc-gen-pawn \
    -I/usr/include/google/protobuf \
    -o build/protoc-gen-pawn \
    protoc-gen-pawn/main.cc \
    protoc-gen-pawn/pawn_generator.cc \
    protoc-gen-pawn/pawn_file.cc \
    protoc-gen-pawn/pawn_enum.cc \
    protoc-gen-pawn/pawn_message.cc \
    protoc-gen-pawn/pawn_service.cc \
    protoc-gen-pawn/pawn_helpers.cc \
    protobuf-c/protobuf-c.pb.cc \
    -lprotoc -lprotobuf

echo "Build completed. Executable at: build/protoc-gen-pawn" 