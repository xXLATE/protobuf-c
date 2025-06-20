@echo off
echo Building standalone protoc-gen-pawn...

REM Create output directory
if not exist build mkdir build

echo Compiling standalone version...
g++ -std=c++11 -o build/protoc-gen-pawn-standalone.exe protoc-gen-pawn/standalone_main.cc

if %ERRORLEVEL% EQU 0 (
    echo Build completed successfully!
    echo Executable at: build\protoc-gen-pawn-standalone.exe
    echo.
    echo Testing with test_enum.proto...
    type test_enum.proto | build\protoc-gen-pawn-standalone.exe > test_enum.pb-pawn.inc
    echo Generated: test_enum.pb-pawn.inc
    echo.
    echo Contents:
    type test_enum.pb-pawn.inc
) else (
    echo Build failed. Error code: %ERRORLEVEL%
)

pause 