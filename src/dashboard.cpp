/*
 * dashboard.cpp - Dashboard Statistics Implementation
 *
 * BEGINNER EXPLANATION:
 *   This file creates a visual dashboard using text characters.
 *   We use box-drawing characters and colors to make it look nice
 *   even in a text console.
 *
 *   "static" methods belong to the class itself, not to any
 *   particular object. You call them as Dashboard::display(system).
 */

#include "dashboard.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

// ============================================================
// display - Show the full dashboard
// ============================================================
void Dashboard::display(const BankSystem& system) {
    int totalIssued   = system.getTotalIssued();
    int totalWaiting  = system.getTotalWaiting();
    int totalServing  = system.getTotalServing();
    int totalServed   = system.getTotalServed();
    int totalCancelled= system.getTotalCancelled();

    std::cout << "\n";
    std::cout << Color::BOLD << Color::CYAN;
    printBoxLine(60, '+', '=');
    printBoxText("DASHBOARD - Bank Token Management System", 60, Color::BOLD + Color::CYAN);
    printBoxLine(60, '+', '=');
    std::cout << Color::RESET;

    // Overall Statistics
    std::cout << "\n  " << Color::BOLD << "OVERALL STATISTICS" << Color::RESET << "\n";
    std::cout << "  " << std::string(40, '-') << "\n";
    
    std::cout << "  Total Tokens Issued   : " << Color::BOLD << Color::WHITE 
              << totalIssued << Color::RESET << "\n";
    std::cout << "  Currently Waiting     : " << Color::YELLOW 
              << totalWaiting << Color::RESET << "\n";
    std::cout << "  Currently Being Served: " << Color::CYAN 
              << totalServing << Color::RESET << "\n";
    std::cout << "  Successfully Served   : " << Color::GREEN 
              << totalServed << Color::RESET << "\n";
    std::cout << "  Cancelled             : " << Color::RED 
              << totalCancelled << Color::RESET << "\n";

    // Per-Service Queue Statistics
    std::cout << "\n  " << Color::BOLD << "QUEUE STATUS BY SERVICE" << Color::RESET << "\n";
    std::cout << "  " << std::string(40, '-') << "\n";

    std::cout << "  " << std::left << std::setw(22) << "Service Type" 
              << std::setw(10) << "Waiting" 
              << std::setw(10) << "Counters" << "\n";
    std::cout << "  " << std::string(42, '-') << "\n";

    for (int i = 0; i < SERVICE_COUNT; i++) {
        int waiting = system.getWaitingForService(i);
        std::string waitColor = (waiting > 5) ? Color::RED : 
                                (waiting > 2) ? Color::YELLOW : Color::GREEN;
        
        std::cout << "  " << std::left << std::setw(22) << serviceTypeName(i)
                  << waitColor << std::setw(10) << waiting << Color::RESET
                  << std::setw(10) << BankSystem::COUNTERS_PER_SERVICE[i] << "\n";
    }

    // Counter Status
    std::cout << "\n  " << Color::BOLD << "COUNTER STATUS" << Color::RESET << "\n";
    std::cout << "  " << std::string(40, '-') << "\n";

    for (size_t i = 0; i < system.counters.size(); i++) {
        system.counters[i].display();
    }

    // Efficiency metrics
    if (totalIssued > 0) {
        std::cout << "\n  " << Color::BOLD << "EFFICIENCY METRICS" << Color::RESET << "\n";
        std::cout << "  " << std::string(40, '-') << "\n";

        double servedPct = (totalIssued > 0) 
            ? (100.0 * totalServed / totalIssued) : 0.0;
        double cancelPct = (totalIssued > 0) 
            ? (100.0 * totalCancelled / totalIssued) : 0.0;
        
        std::cout << "  Completion Rate : " << Color::GREEN 
                  << std::fixed << std::setprecision(1) << servedPct 
                  << "%" << Color::RESET << "\n";
        std::cout << "  Cancellation Rate: " << Color::RED 
                  << std::fixed << std::setprecision(1) << cancelPct 
                  << "%" << Color::RESET << "\n";

        // Counter utilization
        int busyCounters = 0;
        for (size_t i = 0; i < system.counters.size(); i++) {
            if (system.counters[i].isBusy) busyCounters++;
        }
        double utilization = (system.counters.size() > 0) 
            ? (100.0 * busyCounters / system.counters.size()) : 0.0;
        
        std::cout << "  Counter Utilization: " << Color::CYAN 
                  << std::fixed << std::setprecision(1) << utilization 
                  << "% (" << busyCounters << "/" << system.counters.size() 
                  << " busy)" << Color::RESET << "\n";
    }

    std::cout << "\n";
    printBoxLine(60, '+', '=');
}

// ============================================================
// displaySummaryBar - Quick status line for the main menu
// ============================================================
void Dashboard::displaySummaryBar(const BankSystem& system) {
    std::cout << Color::DIM << "  [ "
              << "Issued: " << system.getTotalIssued() << " | "
              << Color::YELLOW << "Waiting: " << system.getTotalWaiting() << Color::DIM << " | "
              << Color::CYAN << "Serving: " << system.getTotalServing() << Color::DIM << " | "
              << Color::GREEN << "Served: " << system.getTotalServed() << Color::DIM
              << " ]" << Color::RESET << "\n";
}
