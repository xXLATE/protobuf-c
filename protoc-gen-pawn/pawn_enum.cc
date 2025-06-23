// Modified for Pawn by the protobuf-pawn project

#include "pawn_enum.h"
#include "pawn_helpers.h"

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>
#include <string>

namespace google {
namespace protobuf {
namespace compiler {
namespace pawn {

EnumGenerator::EnumGenerator(const EnumDescriptor* descriptor)
  : descriptor_(descriptor) {
}

EnumGenerator::~EnumGenerator() {}

void EnumGenerator::GenerateDefinition(io::Printer* printer) {
  printer->Print(
    "// Enum: $full_name$\n",
    "full_name", descriptor_->full_name());

  for (int i = 0; i < descriptor_->value_count(); i++) {
    const EnumValueDescriptor* value = descriptor_->value(i);
    printer->Print(
      "#define $name$ $value$\n",
      "name", EnumValueName(value),
      "value", std::to_string(value->number()));
  }
  printer->Print("\n");
}

}  // namespace pawn
}  // namespace compiler
}  // namespace protobuf
}  // namespace google 