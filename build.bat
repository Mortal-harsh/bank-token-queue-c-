@echo off
REM ============================================================
REM build.bat - Compile the Bank Token Management System
REM ============================================================
REM This script compiles all source files into one executable.
REM 
REM HOW IT WORKS:
REM   g++ is the GNU C++ compiler.
REM   -std=c++11 tells it to use C++11 features.
REM   -o specifies the output executable name.
REM   -static links all libraries into the executable so it
REM          runs on any Windows machine without extra DLLs.
REM ============================================================

echo.
echo ========================================
echo   Building Bank Token Management System
echo ========================================
echo.

where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    if exist "C:\MinGW\bin\g++.exe" set "PATH=C:\MinGW\bin;%PATH%"
    if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"
    if exist "C:\msys64\ucrt64\bin\g++.exe" set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    if exist "C:\TDM-GCC-64\bin\g++.exe" set "PATH=C:\TDM-GCC-64\bin;%PATH%"
)

g++ -std=c++11 -o BankTokenSystem.exe ^
    src/main.cpp ^
    src/token.cpp ^
    src/queue.cpp ^
    src/counter.cpp ^
    src/bank_system.cpp ^
    src/dashboard.cpp ^
    src/report.cpp ^
    src/persist.cpp ^
    -static

if %ERRORLEVEL% EQU 0 (
    echo.
    echo   BUILD SUCCESSFUL!
    echo   Run: BankTokenSystem.exe
    echo   Demo: BankTokenSystem.exe --demo
    echo.
) else (
    echo.
    echo   BUILD FAILED! Check errors above.
    echo.
)
