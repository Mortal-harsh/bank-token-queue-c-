@echo off
title 🏦 BANK TOKEN MANAGEMENT SYSTEM - LIVE DEMO FOR EVALUATION
color 0B
cls
cd /d "%~dp0"

echo ==============================================================================
echo       BANK TOKEN MANAGEMENT SYSTEM - C++ PROJECT EVALUATION DEMO
echo ==============================================================================
echo.
echo   Welcome! This script runs the live C++ system with pre-loaded demo data.
echo.
echo   [RECOMMENDED PRESENTATION FLOW FOR TEACHER/EXAMINER]:
echo   -----------------------------------------------------
echo   Step 1: Press '6' to show real-time Bank Dashboard ^& Counter Analytics
echo   Step 2: Press '4' to show FIFO Queues with Senior Citizen [PRIORITY] jumps
echo   Step 3: Press '1' to issue a new customer Token live
echo   Step 4: Press '2' to call and serve next token at a Teller Counter
echo   Step 5: Press '7' to demonstrate the Token Search Engine
echo   Step 6: Press '8' to export live audit reports to Excel CSV
echo   Step 7: Press '0' to demonstrate local file persistence (auto-save)
echo.
echo ==============================================================================
echo   Press any key to start the interactive C++ Application...
echo ==============================================================================
pause >nul

BankTokenSystem.exe --demo

echo.
echo ==============================================================================
echo   Demonstration Finished. Data saved to 'data/' and reports in 'reports/'.
echo ==============================================================================
echo.
pause
