@echo off
echo Testing protoc-gen-pawn generation...

echo.
echo Generating test_enum.pb-pawn.inc...
protoc --plugin=protoc-gen-pawn=protoc-gen-pawn.bat --proto_path=. --pawn_out=. test_enum.proto
if %errorlevel% neq 0 (
    echo FAILED: Could not generate test_enum.pb-pawn.inc
    exit /b 1
)

echo.
echo Generating test_message.pb-pawn.inc...
protoc --plugin=protoc-gen-pawn=protoc-gen-pawn.bat --proto_path=. --pawn_out=. test_message.proto
if %errorlevel% neq 0 (
    echo FAILED: Could not generate test_message.pb-pawn.inc
    exit /b 1
)

echo.
echo Generating t/test.pb-pawn.inc...
protoc --plugin=protoc-gen-pawn=protoc-gen-pawn.bat --proto_path=. --pawn_out=. t/test.proto
if %errorlevel% neq 0 (
    echo FAILED: Could not generate t/test.pb-pawn.inc
    exit /b 1
)

echo.
echo SUCCESS: All Pawn files generated successfully!
echo.

echo Generated files:
dir *.pb-pawn.inc
dir t\*.pb-pawn.inc

echo.
echo Pawn generator is working correctly. 