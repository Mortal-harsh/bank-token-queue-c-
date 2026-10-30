/*
 * main.cpp - Entry Point & Main Menu Loop
 *
 * PURPOSE:
 *   This is where the program starts (the main() function).
 *   It shows the main menu, handles user input cleanly line-by-line,
 *   and calls the appropriate functions in the BankSystem.
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

#include "utils.h"
#include "bank_system.h"
#include "dashboard.h"
#include "report.h"
#include "persist.h"
#include "demo.h"

// ============================================================
// Enable ANSI color support on Windows 10+
// ============================================================
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    SetConsoleOutputCP(65001);
}
#else
void enableColors() {}
#endif

// ============================================================
// Robust Line-Based Input Helpers
// ============================================================
// By reading full lines and parsing them, we prevent all
// classic C++ input buffer desynchronization bugs (like leftover
// newlines causing skipped prompts).

static std::string readLine(const std::string& prompt = "") {
    if (!prompt.empty()) std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        return "";
    }
    return trim(line);
}

static int readInt(const std::string& prompt = "") {
    std::string line = readLine(prompt);
    if (line.empty()) return -999;
    try {
        return std::stoi(line);
    } catch (...) {
        return -999;
    }
}

// ============================================================
// Display the Main Menu
// ============================================================
void showMainMenu(const BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::CYAN;
    printBoxLine(50, '+', '=');
    printBoxText("BANK TOKEN MANAGEMENT SYSTEM", 50, Color::BOLD + Color::CYAN);
    printBoxLine(50, '+', '=');
    std::cout << Color::RESET;

    // Summary bar
    Dashboard::displaySummaryBar(system);
    
    std::cout << "\n";
    printBoxLine(50);
    printBoxText("1. Issue New Token",          50, Color::GREEN);
    printBoxText("2. Call Next Token (Serve)",   50, Color::CYAN);
    printBoxText("3. Complete Current Service",  50, Color::BLUE);
    printBoxText("4. View All Queues",           50, Color::YELLOW);
    printBoxText("5. View Counter Status",       50, Color::YELLOW);
    printBoxText("6. Dashboard & Statistics",    50, Color::MAGENTA);
    printBoxText("7. Search Token",              50, Color::WHITE);
    printBoxText("8. Export CSV Reports",         50, Color::WHITE);
    printBoxText("9. Load Demo Data",            50, Color::DIM);
    printBoxLine(50);
    printBoxText("0. Save & Exit",               50, Color::RED);
    printBoxLine(50);
}

// ============================================================
// Action: Issue New Token
// ============================================================
void actionIssueToken(BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::GREEN;
    printBoxLine(50);
    printBoxText("ISSUE NEW TOKEN", 50, Color::BOLD + Color::GREEN);
    printBoxLine(50);
    std::cout << Color::RESET;

    std::string name = readLine("\n  Customer Name : ");
    if (name.empty()) {
        std::cout << Color::RED << "  Error: Name cannot be empty." 
                  << Color::RESET << "\n";
        return;
    }

    std::string phone = readLine("  Phone Number  : ");
    if (phone.empty()) {
        std::cout << Color::RED << "  Error: Phone cannot be empty." 
                  << Color::RESET << "\n";
        return;
    }

    std::cout << "\n  Service Types:\n";
    std::cout << "    " << Color::CYAN << "0" << Color::RESET << " - Deposit\n";
    std::cout << "    " << Color::CYAN << "1" << Color::RESET << " - Withdrawal\n";
    std::cout << "    " << Color::CYAN << "2" << Color::RESET << " - Account Services\n";
    std::cout << "    " << Color::CYAN << "3" << Color::RESET << " - Loan\n";
    int serviceType = readInt("  Select service [0-3]: ");

    if (serviceType < 0 || serviceType >= SERVICE_COUNT) {
        std::cout << Color::RED << "  Error: Invalid service type." 
                  << Color::RESET << "\n";
        return;
    }

    std::string priorityInput = readLine("  Priority (Senior/Specially-abled)? [y/N]: ");
    bool isPriority = (!priorityInput.empty() && 
                       (priorityInput[0] == 'y' || priorityInput[0] == 'Y'));

    // Issue the token
    int tokenId = system.issueToken(name, phone, serviceType, isPriority);

    if (tokenId > 0) {
        std::cout << "\n  " << Color::GREEN << Color::BOLD 
                  << "Token Issued Successfully!" << Color::RESET << "\n";
        
        Token* t = system.getToken(tokenId);
        if (t) {
            std::cout << "\n";
            t->display();
        }

        std::cout << "\n  " << Color::YELLOW 
                  << "Position in queue: " << system.getWaitingForService(serviceType) 
                  << Color::RESET << "\n";
    } else {
        std::cout << Color::RED << "  Error: Failed to issue token." 
                  << Color::RESET << "\n";
    }
}

// ============================================================
// Action: Call Next Token
// ============================================================
void actionCallNext(BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::CYAN;
    printBoxLine(50);
    printBoxText("CALL NEXT TOKEN", 50, Color::BOLD + Color::CYAN);
    printBoxLine(50);
    std::cout << Color::RESET;

    // Show available counters
    std::cout << "\n  " << Color::BOLD << "Available Counters:" << Color::RESET << "\n";
    for (size_t i = 0; i < system.counters.size(); i++) {
        system.counters[i].display();
    }

    std::stringstream prompt;
    prompt << "\n  Select counter number [1-" << system.counters.size() << "]: ";
    int counterNum = readInt(prompt.str());
    int counterIdx = counterNum - 1;  // Convert to 0-based index

    if (counterIdx < 0 || counterIdx >= system.getCounterCount()) {
        std::cout << Color::RED << "  Error: Invalid counter number." 
                  << Color::RESET << "\n";
        return;
    }

    if (system.counters[counterIdx].isBusy) {
        std::cout << Color::RED 
                  << "  Error: Counter is busy. Complete current service first (option 3)." 
                  << Color::RESET << "\n";
        return;
    }

    int tokenId = system.callNextToken(counterIdx);

    if (tokenId > 0) {
        Token* t = system.getToken(tokenId);
        std::cout << "\n  " << Color::GREEN << Color::BOLD 
                  << "Now Serving:" << Color::RESET << "\n";
        if (t) t->display();
        std::cout << "\n  " << Color::CYAN << "at Counter " 
                  << (counterIdx + 1) << " (" 
                  << system.counters[counterIdx].counterName << ")"
                  << Color::RESET << "\n";
    } else {
        int svc = system.counters[counterIdx].serviceType;
        std::cout << Color::YELLOW 
                  << "  No tokens waiting for " << serviceTypeName(svc) << "." 
                  << Color::RESET << "\n";
    }
}

// ============================================================
// Action: Complete Service
// ============================================================
void actionCompleteService(BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::BLUE;
    printBoxLine(50);
    printBoxText("COMPLETE SERVICE", 50, Color::BOLD + Color::BLUE);
    printBoxLine(50);
    std::cout << Color::RESET;

    // Show busy counters
    std::cout << "\n  " << Color::BOLD << "Busy Counters:" << Color::RESET << "\n";
    bool anyBusy = false;
    for (size_t i = 0; i < system.counters.size(); i++) {
        if (system.counters[i].isBusy) {
            system.counters[i].display();
            anyBusy = true;
        }
    }

    if (!anyBusy) {
        std::cout << Color::YELLOW << "  No counters are currently busy." 
                  << Color::RESET << "\n";
        return;
    }

    std::stringstream prompt;
    prompt << "\n  Select counter number [1-" << system.counters.size() << "]: ";
    int counterNum = readInt(prompt.str());
    int counterIdx = counterNum - 1;

    if (counterIdx < 0 || counterIdx >= system.getCounterCount()) {
        std::cout << Color::RED << "  Error: Invalid counter number." 
                  << Color::RESET << "\n";
        return;
    }

    int tokenId = system.completeService(counterIdx);

    if (tokenId > 0) {
        std::cout << "\n  " << Color::GREEN << Color::BOLD 
                  << "Service Completed!" << Color::RESET << "\n";
        Token* t = system.getToken(tokenId);
        if (t) t->display();
    } else {
        std::cout << Color::YELLOW 
                  << "  Counter " << (counterIdx + 1) << " is not serving anyone." 
                  << Color::RESET << "\n";
    }
}

// ============================================================
// Action: View All Queues
// ============================================================
void actionViewQueues(BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::YELLOW;
    printBoxLine(50);
    printBoxText("ALL QUEUES", 50, Color::BOLD + Color::YELLOW);
    printBoxLine(50);
    std::cout << Color::RESET;

    for (int q = 0; q < SERVICE_COUNT; q++) {
        std::cout << "\n  " << Color::BOLD << serviceTypeName(q) 
                  << " Queue" << Color::RESET;
        std::cout << " (" << system.queues[q].size() << " waiting)\n";
        std::cout << "  " << std::string(35, '-') << "\n";

        std::vector<int> ids = system.queues[q].getAllTokenIds();
        if (ids.empty()) {
            std::cout << "  " << Color::DIM << "  (empty)" 
                      << Color::RESET << "\n";
        } else {
            for (size_t i = 0; i < ids.size(); i++) {
                Token* t = system.getToken(ids[i]);
                if (t) {
                    std::string prioStr = t->isPriority 
                        ? (Color::MAGENTA + " [P]" + Color::RESET) : "";
                    std::cout << "  " << Color::CYAN << "  " << (i + 1) << ". " 
                              << Color::RESET << t->tokenId 
                              << " - " << t->customerName 
                              << prioStr << "\n";
                }
            }
        }
    }
}

// ============================================================
// Action: View Counter Status
// ============================================================
void actionViewCounters(BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::YELLOW;
    printBoxLine(50);
    printBoxText("COUNTER STATUS", 50, Color::BOLD + Color::YELLOW);
    printBoxLine(50);
    std::cout << Color::RESET;

    std::cout << "\n";
    for (size_t i = 0; i < system.counters.size(); i++) {
        system.counters[i].display();
        
        if (system.counters[i].isBusy) {
            Token* t = system.getToken(system.counters[i].currentTokenId);
            if (t) {
                std::cout << "      " << Color::DIM 
                          << t->customerName << " | " << t->phone
                          << Color::RESET << "\n";
            }
        }
    }
}

// ============================================================
// Action: Search Tokens
// ============================================================
void actionSearch(BankSystem& system) {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::WHITE;
    printBoxLine(50);
    printBoxText("SEARCH TOKENS", 50, Color::BOLD + Color::WHITE);
    printBoxLine(50);
    std::cout << Color::RESET;

    std::string query = readLine("\n  Search (ID / Name / Phone / Status): ");
    if (query.empty()) {
        std::cout << Color::YELLOW << "  Please enter a search term." 
                  << Color::RESET << "\n";
        return;
    }

    std::vector<Token> results = system.searchTokens(query);

    if (results.empty()) {
        std::cout << Color::YELLOW << "  No tokens found matching \"" 
                  << query << "\"." << Color::RESET << "\n";
    } else {
        std::cout << "\n  " << Color::GREEN << "Found " << results.size() 
                  << " result(s):" << Color::RESET << "\n\n";
        for (size_t i = 0; i < results.size(); i++) {
            results[i].display();
            std::cout << "\n";
        }
    }
}

// ============================================================
// Main Function - Program Entry Point
// ============================================================
int main(int argc, char* argv[]) {
    enableColors();

    BankSystem system;

    // Load saved data if present
    if (Persistence::hasSavedData()) {
        bool loaded = Persistence::loadAll(system);
        if (loaded) {
            std::cout << Color::GREEN << "\n  Previous session data loaded." 
                      << Color::RESET << "\n";
        }
    }

    // Check for --demo flag
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--demo") == 0) {
            loadDemoData(system);
            break;
        }
    }

    // ========== Main Menu Loop ==========
    bool running = true;
    while (running) {
        showMainMenu(system);

        int choice = readInt("\n  Enter choice: ");
        if (std::cin.eof()) {
            Persistence::saveAll(system);
            break;
        }

        switch (choice) {
            case 1:
                actionIssueToken(system);
                break;

            case 2:
                actionCallNext(system);
                break;

            case 3:
                actionCompleteService(system);
                break;

            case 4:
                actionViewQueues(system);
                break;

            case 5:
                actionViewCounters(system);
                break;

            case 6:
                Dashboard::display(system);
                break;

            case 7:
                actionSearch(system);
                break;

            case 8:
                ReportGenerator::exportAll(system);
                break;

            case 9:
                loadDemoData(system);
                break;

            case 0: {
                bool saved = Persistence::saveAll(system);
                if (saved) {
                    std::cout << Color::GREEN << "\n  Data saved successfully." 
                              << Color::RESET << "\n";
                } else {
                    std::cout << Color::RED << "\n  Warning: Could not save data." 
                              << Color::RESET << "\n";
                }
                std::cout << Color::BOLD << "\n  Thank you for using Bank Token Management System!" 
                          << Color::RESET << "\n";
                std::cout << Color::DIM << "  Goodbye.\n" << Color::RESET << "\n";
                running = false;
                break;
            }

            default:
                std::cout << Color::RED << "\n  Invalid choice. Please select 0-9." 
                          << Color::RESET << "\n";
                break;
        }

        // Pause after interactive menu action if not EOF
        if (running && choice >= 1 && choice <= 9 && !std::cin.eof()) {
            readLine("\n  Press Enter to continue...");
        }
    }

    return 0;
}
