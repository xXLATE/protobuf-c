#include <iostream>
#include <string>

// Простая имитация основных функций для тестирования логики

std::string UnderscoresToCamelCase(const std::string& name, bool cap_first_letter) {
  std::string result;
  bool cap_next_letter = cap_first_letter;
  
  for (size_t i = 0; i < name.size(); i++) {
    if (name[i] == '_') {
      cap_next_letter = true;
    } else if (cap_next_letter) {
      result += toupper(name[i]);
      cap_next_letter = false;
    } else {
      result += tolower(name[i]);
    }
  }
  
  return result;
}

std::string DotsToUnderscores(const std::string& name) {
  std::string result = name;
  std::replace(result.begin(), result.end(), '.', '_');
  return result;
}

std::string EnumName(const std::string& full_name) {
  std::string result = DotsToUnderscores(full_name);
  return UnderscoresToCamelCase(result, true);
}

std::string StructName(const std::string& full_name) {
  std::string result = DotsToUnderscores(full_name);
  return UnderscoresToCamelCase(result, true);
}

std::string FieldName(const std::string& name) {
  return UnderscoresToCamelCase(name, false);
}

void testPawnNaming() {
  std::cout << "Testing Pawn naming conventions:\n";
  
  // Test enum names
  std::cout << "EnumName('test.Status') = " << EnumName("test.Status") << std::endl;
  std::cout << "EnumName('my_package.MyEnum') = " << EnumName("my_package.MyEnum") << std::endl;
  
  // Test struct names  
  std::cout << "StructName('test.User') = " << StructName("test.User") << std::endl;
  std::cout << "StructName('my_package.UserMessage') = " << StructName("my_package.UserMessage") << std::endl;
  
  // Test field names
  std::cout << "FieldName('user_id') = " << FieldName("user_id") << std::endl;
  std::cout << "FieldName('first_name') = " << FieldName("first_name") << std::endl;
  std::cout << "FieldName('is_active') = " << FieldName("is_active") << std::endl;
}

void generateTestEnum() {
  std::cout << "\nGenerating test enum:\n";
  std::cout << "// test.Status\n";
  std::cout << "enum Status {\n";
  std::cout << "  Status_Unknown = 0,\n";
  std::cout << "  Status_Active = 1,\n";
  std::cout << "  Status_Inactive = 2\n";
  std::cout << "}\n\n";
}

void generateTestStruct() {
  std::cout << "Generating test struct:\n";
  std::cout << "// test.User\n";
  std::cout << "enum struct User {\n";
  std::cout << "  int id;\n";
  std::cout << "  char name[256];\n";
  std::cout << "  Status status;\n";
  std::cout << "  bool isActive;\n";
  std::cout << "  float score;\n";
  std::cout << "}\n\n";
}

int main() {
  testPawnNaming();
  generateTestEnum();
  generateTestStruct();
  
  std::cout << "Test completed. This demonstrates the expected output format.\n";
  return 0;
} 