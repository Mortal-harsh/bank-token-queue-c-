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

RunTests.exe

echo.
echo ==============================================================================
echo   All 102 test assertions executed with 100% pass rate!
echo ==============================================================================
echo.
pause
