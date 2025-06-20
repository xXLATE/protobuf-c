@echo off
echo ================================================
echo Building protoc-gen-pawn - Protocol Buffer Pawn Generator
echo ================================================

REM Set MSYS2 paths
set MSYS2_ROOT=C:\msys64
set PROTOBUF_INCLUDE=%MSYS2_ROOT%\mingw64\include
set PROTOBUF_LIB=%MSYS2_ROOT%\mingw64\lib
set MINGW_BIN=%MSYS2_ROOT%\mingw64\bin

REM Add MSYS2 to PATH for this session
set PATH=%MINGW_BIN%;%PATH%

REM Create output directory
if not exist build mkdir build

echo.
echo Compiling protoc-gen-pawn plugin...
%MINGW_BIN%\g++.exe -std=c++17 ^
    -I%PROTOBUF_INCLUDE% ^
    -L%PROTOBUF_LIB% ^
    -o build/protoc-gen-pawn.exe ^
    protoc-gen-pawn/plugin_main.cc ^
    -lprotobuf -lprotoc ^
    -labsl_log_internal_message ^
    -labsl_log_internal_check_op ^
    -labsl_hash ^
    -labsl_raw_hash_set ^
    -labsl_raw_logging_internal ^
    -labsl_strings

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo Build completed successfully!
echo.
echo ================================================
echo Testing protoc-gen-pawn
echo ================================================

echo.
echo Testing with test_enum.proto...
protoc --plugin=protoc-gen-pawn=build\protoc-gen-pawn.exe --pawn_out=. test_enum.proto
if %ERRORLEVEL% EQU 0 (
    echo ✓ Generated test_enum.pb-pawn.inc
) else (
    echo ✗ Failed to generate test_enum.pb-pawn.inc
)

echo.
echo Testing with test_message.proto...
protoc --plugin=protoc-gen-pawn=build\protoc-gen-pawn.exe --pawn_out=. test_message.proto
if %ERRORLEVEL% EQU 0 (
    echo ✓ Generated test_message.pb-pawn.inc
) else (
    echo ✗ Failed to generate test_message.pb-pawn.inc
)

echo.
echo ================================================
echo protoc-gen-pawn is ready to use!
echo ================================================
echo.
echo Usage:
echo   protoc --plugin=protoc-gen-pawn=build\protoc-gen-pawn.exe --pawn_out=output_dir your_file.proto
echo.
echo Example output files:
if exist test_enum.pb-pawn.inc (
    echo test_enum.pb-pawn.inc:
    echo ----------------------------------------
    type test_enum.pb-pawn.inc
    echo ----------------------------------------
) 