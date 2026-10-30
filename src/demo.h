/*
 * demo.h - Demo Mode Data Generator
 *
 * PURPOSE:
 *   Populates the system with realistic sample data so you can
 *   demonstrate the application without manually entering data.
 *
 * BEGINNER EXPLANATION:
 *   This is a header-only file (no .cpp needed) because all the
 *   code is in inline functions. "inline" tells the compiler to
 *   insert the function's code directly where it's called, rather
 *   than making a separate function call.
 *
 *   We use arrays of sample names and phone numbers to create
 *   realistic-looking test data.
 */

#ifndef DEMO_H
#define DEMO_H

#include "bank_system.h"
#include "utils.h"
#include <iostream>

inline void loadDemoData(BankSystem& system) {
    std::cout << "\n  " << Color::BOLD << Color::MAGENTA 
              << "Loading Demo Data..." << Color::RESET << "\n";
    std::cout << "  " << std::string(40, '-') << "\n";

    // Sample customer data
    // Format: {name, phone, serviceType, isPriority}
    struct DemoCustomer {
        const char* name;
        const char* phone;
        int service;
        bool priority;
    };

    DemoCustomer customers[] = {
        // Deposits
        {"Aarav Sharma",      "9876543210", SERVICE_DEPOSIT,    false},
        {"Priya Patel",       "9123456789", SERVICE_DEPOSIT,    false},
        {"Ramesh Kumar",      "9988776655", SERVICE_DEPOSIT,    true },  // Priority
        {"Neha Gupta",        "9012345678", SERVICE_DEPOSIT,    false},
        
        // Withdrawals
        {"Vikram Singh",      "9876501234", SERVICE_WITHDRAWAL, false},
        {"Sunita Devi",       "9345678901", SERVICE_WITHDRAWAL, true },  // Priority
        {"Arjun Reddy",       "9567890123", SERVICE_WITHDRAWAL, false},
        
        // Account Services
        {"Meera Joshi",       "9234567890", SERVICE_ACCOUNT,    false},
        {"Kabir Malhotra",    "9456789012", SERVICE_ACCOUNT,    false},
        {"Lakshmi Iyer",      "9678901234", SERVICE_ACCOUNT,    true },  // Priority
        {"Rohan Chatterjee",  "9890123456", SERVICE_ACCOUNT,    false},
        
        // Loans
        {"Divya Nair",        "9012398765", SERVICE_LOAN,       false},
        {"Sanjay Verma",      "9234501678", SERVICE_LOAN,       false},
        {"Padma Rao",         "9456703890", SERVICE_LOAN,       true },  // Priority
        {"Amit Deshmukh",     "9678905012", SERVICE_LOAN,       false},
    };

    int numCustomers = sizeof(customers) / sizeof(customers[0]);

    for (int i = 0; i < numCustomers; i++) {
        int id = system.issueToken(
            customers[i].name,
            customers[i].phone,
            customers[i].service,
            customers[i].priority
        );

        std::string prioStr = customers[i].priority 
            ? (Color::MAGENTA + " [PRIORITY]" + Color::RESET) : "";
        
        std::cout << "  " << Color::GREEN << "+" << Color::RESET 
                  << " " << formatTokenId(id)
                  << " - " << customers[i].name
                  << " (" << serviceTypeName(customers[i].service) << ")"
                  << prioStr << "\n";
    }

    // Also serve a few tokens to show served data
    std::cout << "\n  " << Color::BOLD << "Auto-serving some tokens..." 
              << Color::RESET << "\n";

    // Serve at counter 0 (Deposit counter 1)
    int served = system.callNextToken(0);
    if (served > 0) {
        std::cout << "  " << Color::CYAN << ">" << Color::RESET 
                  << " " << formatTokenId(served) << " called at Counter 1\n";
        system.completeService(0);
        std::cout << "  " << Color::GREEN << "v" << Color::RESET 
                  << " " << formatTokenId(served) << " service completed\n";
    }

    // Serve at counter 2 (Withdrawal counter 1)
    served = system.callNextToken(2);
    if (served > 0) {
        std::cout << "  " << Color::CYAN << ">" << Color::RESET 
                  << " " << formatTokenId(served) << " called at Counter 3\n";
        system.completeService(2);
        std::cout << "  " << Color::GREEN << "v" << Color::RESET 
                  << " " << formatTokenId(served) << " service completed\n";
    }

    // Leave some being served currently
    served = system.callNextToken(1);
    if (served > 0) {
        std::cout << "  " << Color::CYAN << ">" << Color::RESET 
                  << " " << formatTokenId(served) << " now being served at Counter 2\n";
    }

    std::cout << "\n  " << Color::GREEN << Color::BOLD 
              << "Demo data loaded: " << numCustomers << " tokens created." 
              << Color::RESET << "\n";
}

#endif // DEMO_H
