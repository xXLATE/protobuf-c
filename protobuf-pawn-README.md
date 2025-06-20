# protobuf-pawn

Генератор кода Pawn для Protocol Buffers

## Описание

`protobuf-pawn` - это модифицированная версия `protobuf-c`, адаптированная для генерации кода на языке Pawn вместо C. Этот плагин позволяет генерировать Pawn включаемые файлы (.inc) на основе .proto файлов.

## Отличия от C

Язык Pawn имеет следующие ключевые отличия от C:

1. **Типобезопасность**: Pawn - типобезопасный язык (typeless), использует только целочисленные типы
2. **Нет указателей**: Вместо указателей используются ссылки для передачи параметров
3. **Массивы**: Поддержка массивов с другим синтаксисом
4. **Числовые системы**: Поддерживает только шестнадцатеричные, десятичные и двоичные числа (нет восьмеричных)
5. **Препроцессор**: Использует директивы компилятора вместо полноценного препроцессора
6. **Структуры**: Нет структур в традиционном понимании

## Архитектура

Проект основан на архитектуре protobuf-c:

- `main.cc` - точка входа, регистрирует генератор как `protoc-gen-pawn`
- `pawn_generator.cc/h` - основной генератор, создает файлы .pb-pawn.inc
- `pawn_file.cc/h` - координирует генерацию всего файла
- `pawn_enum.cc/h` - генерирует перечисления как константы #define
- `pawn_message.cc/h` - генерирует сообщения как наборы констант
- `pawn_service.cc/h` - генерирует определения сервисов
- `pawn_helpers.cc/h` - вспомогательные функции

## Генерация кода

Для .proto файлов генерируются .pb-pawn.inc файлы с:

- Константы для значений enum (`#define ENUM_VALUE 1`)
- Константы для идентификаторов полей сообщений
- Определения для работы с protobuf данными в Pawn

## Компиляция

Для сборки protoc-gen-pawn нужны:
- C++ компилятор (g++ или clang++)
- protobuf development headers
- protoc

### В Linux/Unix:
```bash
# Установка зависимостей (Ubuntu/Debian)
sudo apt-get install protobuf-compiler libprotobuf-dev libprotoc-dev

# Компиляция
chmod +x build_protoc_gen_pawn.sh
./build_protoc_gen_pawn.sh
```

### В Windows:
Требуется настройка среды разработки с protobuf libraries.

## Использование

```bash
# После успешной компиляции
protoc --plugin=protoc-gen-pawn=./build/protoc-gen-pawn --pawn_out=. example.proto
```

Это создаст файл `example.pb-pawn.inc`, который можно включить в Pawn код:

```pawn
#include "example.pb-pawn.inc"
```

## Реализованные компоненты

- ✅ Базовая архитектура генератора
- ✅ Генерация enum как константы #define
- ✅ Базовая структура файлов
- ✅ Генерация сообщений (константы полей)
- ✅ Генерация сервисов (константы методов)
- ❌ Поддержка расширений (не требуется для Pawn)

## Состояние проекта

Генератор `protoc-gen-pawn` готов к компиляции и тестированию. Основная функциональность реализована:

### Completed Features:
- Генерация .pb-pawn.inc файлов
- Константы enum в формате `#define ENUM_NAME__VALUE_NAME number`
- Константы полей сообщений в формате `#define MESSAGE_NAME__FIELD_NAME_FIELD number`
- Константы методов сервисов в формате `#define SERVICE_NAME__METHOD_NAME_METHOD number`
- Правильная обработка зависимостей и include файлов
- Pawn-совместимые include guards

## Пример

Для .proto файла:
```protobuf
enum Status {
  UNKNOWN = 0;
  ACTIVE = 1;
  INACTIVE = 2;
}
```

Генерируется Pawn код:
```pawn
/* STATUS values */
#define STATUS__UNKNOWN 0
#define STATUS__ACTIVE 1
#define STATUS__INACTIVE 2
``` 