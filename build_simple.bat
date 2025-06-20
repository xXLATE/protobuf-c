@echo off
echo Building simple protoc-gen-pawn...

REM Create output directory
if not exist build mkdir build

REM Set MSYS2 paths
set MSYS2_ROOT=C:\msys64
set PROTOBUF_INCLUDE=%MSYS2_ROOT%\mingw64\include
set PROTOBUF_LIB=%MSYS2_ROOT%\mingw64\lib
set MINGW_BIN=%MSYS2_ROOT%\mingw64\bin

REM Add MSYS2 to PATH temporarily
set PATH=%MINGW_BIN%;%PATH%

echo Compiling simple protoc-gen-pawn...
echo Include path: %PROTOBUF_INCLUDE%
echo Library path: %PROTOBUF_LIB%

%MINGW_BIN%\g++.exe -std=c++17 -I%PROTOBUF_INCLUDE% ^
    -L%PROTOBUF_LIB% ^
    -o build/protoc-gen-pawn.exe ^
    protoc-gen-pawn/simple_main.cc ^
    -lprotobuf -lprotoc -labsl_log_internal_message -labsl_log_internal_check_op -labsl_hash -labsl_raw_hash_set -labsl_raw_logging_internal -labsl_strings

if %ERRORLEVEL% EQU 0 (
    echo Build completed successfully! Executable at: build\protoc-gen-pawn.exe
    echo.
    echo Testing with protoc...
    protoc --plugin=protoc-gen-pawn=build\protoc-gen-pawn.exe --pawn_out=. test_enum.proto
    if exist test_enum.pb-pawn.inc (
        echo Generated test_enum.pb-pawn.inc successfully!
        echo.
        echo Contents:
        type test_enum.pb-pawn.inc
    ) else (
        echo Failed to generate output file
    )
    
    echo.
    echo Testing with test_message.proto...
    protoc --plugin=protoc-gen-pawn=build\protoc-gen-pawn.exe --pawn_out=. test_message.proto
    if exist test_message.pb-pawn.inc (
        echo Generated test_message.pb-pawn.inc successfully!
        echo.
        echo Contents:
        type test_message.pb-pawn.inc
    ) else (
        echo Failed to generate test_message output file
    )
) else (
    echo Build failed. Error code: %ERRORLEVEL%
)

pause 