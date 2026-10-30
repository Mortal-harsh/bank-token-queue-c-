/*
 * dashboard.h - Dashboard Statistics Display
 *
 * PURPOSE:
 *   Provides functions to display a beautiful dashboard with
 *   real-time statistics about the bank's token system.
 */

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "bank_system.h"

class Dashboard {
public:
    // Display the full dashboard with all statistics
    static void display(const BankSystem& system);

    // Display a summary bar (shown on the main menu)
    static void displaySummaryBar(const BankSystem& system);
};

#endif // DASHBOARD_H
