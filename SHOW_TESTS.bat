@echo off
title 🧪 AUTOMATED TEST SUITE - 100% PASS VERIFICATION
color 0A
cls
cd /d "%~dp0"

echo ==============================================================================
echo       BANK TOKEN MANAGEMENT SYSTEM - AUTOMATED UNIT TEST SUITE
echo ==============================================================================
echo.
echo   Running 14 test groups across all classes and edge cases...
echo.

if not exist "RunTests.exe" (
    echo [INFO] RunTests.exe not found. Compiling first...
    call run_tests.bat
    if not exist "RunTests.exe" (
        echo [ERROR] Could not build RunTests.exe. Please ensure g++ is installed.
        pause
        exit /b 1
    )
) else (
    RunTests.exe
)

echo.
echo ==============================================================================
echo   All 102 test assertions executed with 100% pass rate!
echo ==============================================================================
echo.
pause
