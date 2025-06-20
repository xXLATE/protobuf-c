@echo off
echo Building protoc-gen-pawn with MSYS2 protobuf libraries...

REM Create output directory
if not exist build mkdir build

REM Set MSYS2 paths
set MSYS2_ROOT=C:\msys64
set PROTOBUF_INCLUDE=%MSYS2_ROOT%\mingw64\include
set PROTOBUF_LIB=%MSYS2_ROOT%\mingw64\lib
set MINGW_BIN=%MSYS2_ROOT%\mingw64\bin

REM Add MSYS2 to PATH temporarily
set PATH=%MINGW_BIN%;%PATH%

echo Attempting to build with MSYS2 protobuf libraries...
echo Include path: %PROTOBUF_INCLUDE%
echo Library path: %PROTOBUF_LIB%

%MINGW_BIN%\g++.exe -std=c++17 -I. -Iprotoc-gen-pawn -I%PROTOBUF_INCLUDE% ^
    -L%PROTOBUF_LIB% ^
    -o build/protoc-gen-pawn.exe ^
    protoc-gen-pawn/main.cc ^
    protoc-gen-pawn/pawn_generator.cc ^
    protoc-gen-pawn/pawn_file.cc ^
    protoc-gen-pawn/pawn_enum.cc ^
    protoc-gen-pawn/pawn_message.cc ^
    protoc-gen-pawn/pawn_service.cc ^
    protoc-gen-pawn/pawn_helpers.cc ^
    -lprotobuf -lprotoc

if %ERRORLEVEL% EQU 0 (
    echo Build completed successfully! Executable at: build\protoc-gen-pawn.exe
    echo.
    echo Testing with protoc...
    protoc --plugin=protoc-gen-pawn=build\protoc-gen-pawn.exe --pawn_out=. test_enum.proto
    if exist test_enum.pb-pawn.inc (
        echo Generated test_enum.pb-pawn.inc successfully!
        echo Contents:
        type test_enum.pb-pawn.inc
    ) else (
        echo Failed to generate output file
    )
) else (
    echo Build failed. Error code: %ERRORLEVEL%
)

pause 