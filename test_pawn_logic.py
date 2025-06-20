#!/usr/bin/env python3
"""
Тестирование логики генератора Pawn кода из protobuf
"""

import re

def dots_to_underscores(name):
    """Преобразует точки в подчеркивания"""
    return name.replace('.', '_')

def underscores_to_camel_case(name, cap_first_letter=True):
    """Преобразует подчеркивания в CamelCase с правильной обработкой составных слов"""
    result = ""
    cap_next_letter = cap_first_letter
    
    for char in name:
        if char == '_':
            cap_next_letter = True
        elif cap_next_letter:
            result += char.upper()
            cap_next_letter = False
        else:
            result += char.lower()
    
    return result

def split_camel_case(name):
    """Разделяет CamelCase на слова с подчеркиваниями"""
    # Добавляем подчеркивания перед заглавными буквами
    result = re.sub('([A-Z])', r'_\1', name)
    # Убираем лишние подчеркивания в начале
    return result.lstrip('_').lower()

def enum_name(full_name):
    """Генерирует имя enum для Pawn"""
    result = dots_to_underscores(full_name)
    return underscores_to_camel_case(result, True)

def enum_value_name(enum_name, value_name):
    """Генерирует имя значения enum"""
    return f"{enum_name}_{underscores_to_camel_case(value_name, True)}"

def struct_name(full_name):
    """Генерирует имя struct для Pawn"""
    result = dots_to_underscores(full_name)
    return underscores_to_camel_case(result, True)

def field_name(name):
    """Генерирует имя поля"""
    return underscores_to_camel_case(name, False)

def service_name(full_name):
    """Генерирует имя сервиса"""
    # Убираем последнюю часть пути (обычно это имя сервиса)
    parts = full_name.split('.')
    service_part = parts[-1] if parts else full_name
    # Преобразуем CamelCase в underscore_case, а затем обратно для нормализации
    normalized = split_camel_case(service_part)
    return underscores_to_camel_case(normalized, True)

def method_name(name):
    """Генерирует имя метода"""
    # Аналогично сервису, нормализуем CamelCase
    normalized = split_camel_case(name)
    return underscores_to_camel_case(normalized, True)

def pawn_field_type(proto_type):
    """Преобразует тип protobuf в тип Pawn"""
    type_mapping = {
        'double': 'float',
        'float': 'float', 
        'int32': 'int',
        'int64': 'int',
        'uint32': 'int',
        'uint64': 'int',
        'sint32': 'int',
        'sint64': 'int',
        'fixed32': 'int',
        'fixed64': 'int',
        'sfixed32': 'int',
        'sfixed64': 'int',
        'bool': 'bool',
        'string': 'char[256]',
        'bytes': 'char[256]'
    }
    return type_mapping.get(proto_type, 'int')

def generate_enum(name, values):
    """Генерирует enum Pawn"""
    enum_name_str = enum_name(name)
    print(f"// {name}")
    print(f"enum {enum_name_str} {{")
    
    for i, (value_name, value_number) in enumerate(values):
        if i > 0:
            print(",")
        enum_value = enum_value_name(enum_name_str, value_name)
        print(f"  {enum_value} = {value_number}", end="")
    
    print()
    print("}\n")

def generate_struct(name, fields):
    """Генерирует enum struct Pawn"""
    struct_name_str = struct_name(name)
    print(f"// {name}")
    print(f"enum struct {struct_name_str} {{")
    
    for field_proto_name, field_type, is_repeated in fields:
        field_name_str = field_name(field_proto_name)
        pawn_type = pawn_field_type(field_type)
        
        if is_repeated:
            print(f"  {pawn_type} {field_name_str}[1024]; // repeated field")
            print(f"  int {field_name_str}Count;")
        else:
            print(f"  {pawn_type} {field_name_str};")
    
    print("}\n")

def generate_service(name, methods):
    """Генерирует константы сервиса"""
    service_name_str = service_name(name)
    print(f"// Service constants for {name}")
    print(f"enum {service_name_str}Methods {{")
    
    for i, method_proto_name in enumerate(methods):
        if i > 0:
            print(",")
        method_name_str = method_name(method_proto_name)
        print(f"  {service_name_str}_{method_name_str} = {i + 1}", end="")
    
    print()
    print("}\n")

def test_naming_conventions():
    """Тестирует конвенции именования"""
    print("=== Тестирование конвенций именования ===\n")
    
    # Тестируем enum
    print("Enum names:")
    print(f"  'test.Status' -> '{enum_name('test.Status')}'")
    print(f"  'my_package.MyEnum' -> '{enum_name('my_package.MyEnum')}'")
    
    # Тестируем struct
    print("\nStruct names:")
    print(f"  'test.User' -> '{struct_name('test.User')}'")
    print(f"  'my_package.UserMessage' -> '{struct_name('my_package.UserMessage')}'")
    
    # Тестируем поля
    print("\nField names:")
    print(f"  'user_id' -> '{field_name('user_id')}'")
    print(f"  'first_name' -> '{field_name('first_name')}'")
    print(f"  'is_active' -> '{field_name('is_active')}'")
    
    # Тестируем сервисы и методы
    print("\nService names:")
    print(f"  'test.UserService' -> '{service_name('test.UserService')}'")
    print(f"  'GetUser' -> '{method_name('GetUser')}'")
    print(f"  'CreateUser' -> '{method_name('CreateUser')}'")
    
    print("\n" + "="*50 + "\n")

def test_generation():
    """Тестирует генерацию кода"""
    print("=== Тестирование генерации кода ===\n")
    
    # Генерируем простой enum
    generate_enum("test.Status", [
        ("UNKNOWN", 0),
        ("ACTIVE", 1), 
        ("INACTIVE", 2)
    ])
    
    # Генерируем структуру
    generate_struct("test.User", [
        ("id", "int32", False),
        ("name", "string", False),
        ("status", "Status", False),
        ("is_active", "bool", False),
        ("scores", "float", True)  # repeated
    ])
    
    # Генерируем сервис
    generate_service("test.UserService", [
        "GetUser",
        "CreateUser", 
        "UpdateUser",
        "DeleteUser"
    ])

if __name__ == "__main__":
    test_naming_conventions()
    test_generation()
    print("Тестирование завершено. Этот вывод показывает ожидаемый формат генерируемого кода Pawn.") 