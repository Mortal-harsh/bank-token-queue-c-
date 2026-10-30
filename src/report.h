/*
 * report.h - CSV Report Generator
 *
 * PURPOSE:
 *   Generates CSV (Comma Separated Values) reports that can be
 *   opened in Excel or any spreadsheet application.
 */

#ifndef REPORT_H
#define REPORT_H

#include "bank_system.h"
#include <string>

class ReportGenerator {
public:
    // Export all tokens to a CSV file
    static bool exportAllTokens(const BankSystem& system, const std::string& filename);

    // Export only served tokens to a CSV file
    static bool exportServedTokens(const BankSystem& system, const std::string& filename);

    // Export statistics summary to a CSV file
    static bool exportStatistics(const BankSystem& system, const std::string& filename);

    // Export all reports at once (calls the above three)
    static void exportAll(const BankSystem& system);
};

#endif // REPORT_H
