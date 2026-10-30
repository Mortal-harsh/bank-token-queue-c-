@echo off
REM ============================================================
REM run_tests.bat - Compile and run the test suite
REM ============================================================

echo.
echo ========================================
echo   Compiling Test Suite
echo ========================================
echo.

where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    if exist "C:\MinGW\bin\g++.exe" set "PATH=C:\MinGW\bin;%PATH%"
    if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"
    if exist "C:\msys64\ucrt64\bin\g++.exe" set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    if exist "C:\TDM-GCC-64\bin\g++.exe" set "PATH=C:\TDM-GCC-64\bin;%PATH%"
)

g++ -std=c++11 -o RunTests.exe tests/test_main.cpp -static

if %ERRORLEVEL% EQU 0 (
    echo   Compilation successful. Running tests...
    echo.
    RunTests.exe
) else (
    echo.
    echo   TEST COMPILATION FAILED! Check errors above.
    echo.
)
