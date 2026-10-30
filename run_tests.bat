@echo off
REM ============================================================
REM run_tests.bat - Compile and run the test suite
REM ============================================================

echo.
echo ========================================
echo   Compiling Test Suite
echo ========================================
echo.

set "PATH=C:\MinGW\bin;%PATH%"

g++.exe -std=c++11 -o RunTests.exe tests/test_main.cpp -static

if %ERRORLEVEL% EQU 0 (
    echo   Compilation successful. Running tests...
    echo.
    RunTests.exe
) else (
    echo.
    echo   TEST COMPILATION FAILED! Check errors above.
    echo.
)
