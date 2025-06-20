#include <iostream>
#include <string>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace pawn {

class PawnGenerator : public CodeGenerator {
public:
  virtual ~PawnGenerator() {}

  virtual bool Generate(const FileDescriptor* file,
                       const std::string& parameter,
                       OutputDirectory* output_directory,
                       std::string* error) const override {
    
    std::string basename = StripProto(file->name());
    std::string filename = basename + ".pb-pawn.inc";
    
    std::unique_ptr<io::ZeroCopyOutputStream> output(
        output_directory->Open(filename));
    io::Printer printer(output.get(), '$');
    
    GenerateHeader(&printer, file);
    GenerateEnums(&printer, file);
    GenerateMessages(&printer, file);
    GenerateServices(&printer, file);
    GenerateFooter(&printer, file);
    
    return true;
  }

private:
  std::string StripProto(const std::string& filename) const {
    if (filename.size() >= 6 && filename.substr(filename.size() - 6) == ".proto") {
      return filename.substr(0, filename.size() - 6);
    }
    return filename;
  }
  
  void GenerateHeader(io::Printer* printer, const FileDescriptor* file) const {
    std::string guard_name = GuardName(file->name());
    
    printer->Print("/* Generated from $filename$ */\n", "filename", file->name());
    printer->Print("#if defined $guard$\n", "guard", guard_name);
    printer->Print("  #endinput\n");
    printer->Print("#endif\n");
    printer->Print("#define $guard$\n\n", "guard", guard_name);
  }
  
  void GenerateEnums(io::Printer* printer, const FileDescriptor* file) const {
    for (int i = 0; i < file->enum_type_count(); i++) {
      const EnumDescriptor* enum_desc = file->enum_type(i);
      
      printer->Print("/* $name$ values */\n", "name", enum_desc->name());
      
      for (int j = 0; j < enum_desc->value_count(); j++) {
        const EnumValueDescriptor* value = enum_desc->value(j);
        
        std::string constant_name = ToUpper(enum_desc->name()) + "__" + value->name();
        printer->Print("#define $constant$ $value$\n",
                      "constant", constant_name,
                      "value", std::to_string(value->number()));
      }
      printer->Print("\n");
    }
  }
  
  void GenerateMessages(io::Printer* printer, const FileDescriptor* file) const {
    for (int i = 0; i < file->message_type_count(); i++) {
      const Descriptor* message = file->message_type(i);
      
      printer->Print("/* $name$ message field tags */\n", "name", message->name());
      
      for (int j = 0; j < message->field_count(); j++) {
        const FieldDescriptor* field = message->field(j);
        
        std::string tag_name = ToUpper(message->name()) + "_FIELD_" + ToUpper(field->name());
        printer->Print("#define $tag$ $number$\n",
                      "tag", tag_name,
                      "number", std::to_string(field->number()));
      }
      printer->Print("\n");
    }
  }
  
  void GenerateServices(io::Printer* printer, const FileDescriptor* file) const {
    for (int i = 0; i < file->service_count(); i++) {
      const ServiceDescriptor* service = file->service(i);
      
      printer->Print("/* $name$ service method IDs */\n", "name", service->name());
      
      for (int j = 0; j < service->method_count(); j++) {
        const MethodDescriptor* method = service->method(j);
        
        std::string method_id = ToUpper(service->name()) + "_METHOD_" + ToUpper(method->name());
        printer->Print("#define $method_id$ $index$\n",
                      "method_id", method_id,
                      "index", std::to_string(j));
      }
      printer->Print("\n");
    }
  }
  
  void GenerateFooter(io::Printer* printer, const FileDescriptor* file) const {
    printer->Print("/* End of generated file */\n");
  }
  
  std::string GuardName(const std::string& filename) const {
    std::string result = "_protobuf_";
    for (char c : filename) {
      if (c == '/' || c == '\\' || c == '.') {
        result += '_';
      } else {
        result += std::tolower(c);
      }
    }
    result += "_included";
    return result;
  }
  
  std::string ToUpper(const std::string& str) const {
    std::string result = str;
    for (char& c : result) {
      c = std::toupper(c);
    }
    return result;
  }
};

}  // namespace pawn
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

int main(int argc, char* argv[]) {
  google::protobuf::compiler::pawn::PawnGenerator generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
} 