/*
 * report.cpp - CSV Report Generator Implementation
 *
 * BEGINNER EXPLANATION:
 *   CSV (Comma Separated Values) is a simple text format where
 *   each line is one row, and columns are separated by commas.
 *   Excel and Google Sheets can open CSV files directly.
 *
 *   We use ofstream (output file stream) to write to files.
 *   It works just like cout, but writes to a file instead of
 *   the screen.
 */

#include "report.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>    // for system()

// Helper: ensure directory exists
static void ensureDir(const std::string& dir) {
    std::string cmd = "if not exist \"" + dir + "\" mkdir \"" + dir + "\"";
    system(cmd.c_str());
}

// ============================================================
// exportAllTokens - Write all token data to a CSV file
// ============================================================
bool ReportGenerator::exportAllTokens(const BankSystem& system, 
                                       const std::string& filename) {
    ensureDir("reports");
    
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << Color::RED << "  Error: Could not create file " 
                  << filename << Color::RESET << "\n";
        return false;
    }

    // Write CSV header row
    file << "Token ID,Customer Name,Phone,Service Type,Priority,"
         << "Status,Issued At,Served At,Completed At,Counter\n";

    // Write each token as a row
    std::map<int, Token>::const_iterator it;
    for (it = system.allTokens.begin(); it != system.allTokens.end(); ++it) {
        const Token& t = it->second;
        file << t.tokenId << ","
             << "\"" << t.customerName << "\","   // Quotes handle commas in names
             << t.phone << ","
             << serviceTypeName(t.serviceType) << ","
             << (t.isPriority ? "Yes" : "No") << ","
             << statusName(t.status) << ","
             << t.issuedAt << ","
             << t.servedAt << ","
             << t.completedAt << ","
             << (t.counterNumber >= 0 ? t.counterNumber + 1 : 0) << "\n";
    }

    file.close();
    std::cout << Color::GREEN << "  Exported " << system.allTokens.size() 
              << " tokens to " << filename << Color::RESET << "\n";
    return true;
}

// ============================================================
// exportServedTokens - Write only served tokens
// ============================================================
bool ReportGenerator::exportServedTokens(const BankSystem& system, 
                                          const std::string& filename) {
    ensureDir("reports");
    
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << Color::RED << "  Error: Could not create file " 
                  << filename << Color::RESET << "\n";
        return false;
    }

    file << "Token ID,Customer Name,Phone,Service Type,Priority,"
         << "Issued At,Served At,Completed At,Counter\n";

    int count = 0;
    std::map<int, Token>::const_iterator it;
    for (it = system.allTokens.begin(); it != system.allTokens.end(); ++it) {
        const Token& t = it->second;
        if (t.status == STATUS_SERVED) {
            file << t.tokenId << ","
                 << "\"" << t.customerName << "\","
                 << t.phone << ","
                 << serviceTypeName(t.serviceType) << ","
                 << (t.isPriority ? "Yes" : "No") << ","
                 << t.issuedAt << ","
                 << t.servedAt << ","
                 << t.completedAt << ","
                 << (t.counterNumber >= 0 ? t.counterNumber + 1 : 0) << "\n";
            count++;
        }
    }

    file.close();
    std::cout << Color::GREEN << "  Exported " << count 
              << " served tokens to " << filename << Color::RESET << "\n";
    return true;
}

// ============================================================
// exportStatistics - Write summary statistics
// ============================================================
bool ReportGenerator::exportStatistics(const BankSystem& system, 
                                        const std::string& filename) {
    ensureDir("reports");
    
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cout << Color::RED << "  Error: Could not create file " 
                  << filename << Color::RESET << "\n";
        return false;
    }

    file << "Metric,Value\n";
    file << "Total Tokens Issued," << system.getTotalIssued() << "\n";
    file << "Currently Waiting," << system.getTotalWaiting() << "\n";
    file << "Currently Being Served," << system.getTotalServing() << "\n";
    file << "Successfully Served," << system.getTotalServed() << "\n";
    file << "Cancelled," << system.getTotalCancelled() << "\n";
    file << "\n";
    file << "Queue Status\n";
    file << "Service Type,Waiting,Counters\n";
    for (int i = 0; i < SERVICE_COUNT; i++) {
        file << serviceTypeName(i) << ","
             << system.getWaitingForService(i) << ","
             << BankSystem::COUNTERS_PER_SERVICE[i] << "\n";
    }
    file << "\n";
    file << "Counter Status\n";
    file << "Counter,Service Type,Status,Current Token\n";
    for (size_t i = 0; i < system.counters.size(); i++) {
        const Counter& c = system.counters[i];
        file << "Counter " << (c.counterNumber + 1) << ","
             << serviceTypeName(c.serviceType) << ","
             << (c.isBusy ? "Busy" : "Free") << ","
             << (c.isBusy ? formatTokenId(c.currentTokenId) : "-") << "\n";
    }

    file.close();
    std::cout << Color::GREEN << "  Exported statistics to " 
              << filename << Color::RESET << "\n";
    return true;
}

// ============================================================
// exportAll - Generate all reports at once
// ============================================================
void ReportGenerator::exportAll(const BankSystem& system) {
    std::cout << "\n  " << Color::BOLD << "Generating Reports..." 
              << Color::RESET << "\n";
    std::cout << "  " << std::string(40, '-') << "\n";

    exportAllTokens(system, "reports/all_tokens_report.csv");
    exportServedTokens(system, "reports/served_tokens_report.csv");
    exportStatistics(system, "reports/statistics_report.csv");

    std::cout << "\n  " << Color::GREEN << Color::BOLD 
              << "All reports saved to reports/ folder." 
              << Color::RESET << "\n";
}
