/*
 * utils.h - Utility functions, constants, and color codes
 * 
 * PURPOSE:
 *   This file holds shared constants, color codes for the console,
 *   and small helper functions used throughout the project.
 *
 * BEGINNER EXPLANATION:
 *   - We use "ANSI escape codes" to add colors in the Windows console.
 *     For example, "\033[32m" turns text green.
 *   - The #ifndef / #define / #endif pattern is called an "include guard".
 *     It prevents this file from being included twice in the same 
 *     compilation unit, which would cause errors.
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <cctype>

// ============================================================
// ANSI Color Codes for Console Output
// ============================================================
// These are special strings that tell the terminal to change
// the color of the text. "\033[" starts an escape sequence.
// The number selects the color. "m" ends the sequence.

namespace Color {
    const std::string RESET   = "\033[0m";      // Back to default
    const std::string RED     = "\033[31m";      // Error messages
    const std::string GREEN   = "\033[32m";      // Success messages
    const std::string YELLOW  = "\033[33m";      // Warnings, highlights
    const std::string BLUE    = "\033[34m";      // Information
    const std::string MAGENTA = "\033[35m";      // Special items
    const std::string CYAN    = "\033[36m";      // Headers
    const std::string WHITE   = "\033[37m";      // Normal text
    const std::string BOLD    = "\033[1m";       // Bold text
    const std::string DIM     = "\033[2m";       // Dimmed text
    const std::string BG_BLUE = "\033[44m";      // Blue background
    const std::string BG_GREEN= "\033[42m";      // Green background
}

// ============================================================
// Service Types - The 4 types of bank services
// ============================================================
// We use an "enum" (enumeration) to give meaningful names to 
// numbers. Instead of remembering that 0 = Deposit, we just
// write SERVICE_DEPOSIT.

enum ServiceType {
    SERVICE_DEPOSIT    = 0,
    SERVICE_WITHDRAWAL = 1,
    SERVICE_ACCOUNT    = 2,
    SERVICE_LOAN       = 3,
    SERVICE_COUNT      = 4   // Total number of service types
};

// ============================================================
// Token Status - What state a token is in
// ============================================================

enum TokenStatus {
    STATUS_WAITING  = 0,   // Customer is in the queue
    STATUS_SERVING  = 1,   // Customer is being served right now
    STATUS_SERVED   = 2,   // Customer has been served and left
    STATUS_CANCELLED= 3    // Token was cancelled
};

// ============================================================
// Helper Functions
// ============================================================

/*
 * serviceTypeName - Converts a ServiceType number to a readable string
 * 
 * EXAMPLE:
 *   serviceTypeName(SERVICE_DEPOSIT)  returns "Deposit"
 */
inline std::string serviceTypeName(int type) {
    switch (type) {
        case SERVICE_DEPOSIT:    return "Deposit";
        case SERVICE_WITHDRAWAL: return "Withdrawal";
        case SERVICE_ACCOUNT:    return "Account Services";
        case SERVICE_LOAN:       return "Loan";
        default:                 return "Unknown";
    }
}

/*
 * statusName - Converts a TokenStatus number to a readable string
 */
inline std::string statusName(int status) {
    switch (status) {
        case STATUS_WAITING:   return "Waiting";
        case STATUS_SERVING:   return "Serving";
        case STATUS_SERVED:    return "Served";
        case STATUS_CANCELLED: return "Cancelled";
        default:               return "Unknown";
    }
}

/*
 * statusColor - Returns the ANSI color code for a given status
 * 
 * This makes it easy to color-code the status in the console:
 *   Waiting  = Yellow (caution)
 *   Serving  = Cyan (active) 
 *   Served   = Green (done)
 *   Cancelled= Red (error)
 */
inline std::string statusColor(int status) {
    switch (status) {
        case STATUS_WAITING:   return Color::YELLOW;
        case STATUS_SERVING:   return Color::CYAN;
        case STATUS_SERVED:    return Color::GREEN;
        case STATUS_CANCELLED: return Color::RED;
        default:               return Color::WHITE;
    }
}

/*
 * formatTokenId - Creates a token ID string like "TKN-0001"
 *
 * We use a string stream with setw(4) and setfill('0') to 
 * pad the number with leading zeros.
 *
 * EXAMPLE:
 *   formatTokenId(42)  returns "TKN-0042"
 */
inline std::string formatTokenId(int id) {
    std::stringstream ss;
    ss << "TKN-" << std::setw(4) << std::setfill('0') << id;
    return ss.str();
}

/*
 * getCurrentTimestamp - Gets the current date and time as a string
 *
 * Uses C's time functions to get the current time, then formats
 * it as "YYYY-MM-DD HH:MM:SS".
 */
inline std::string getCurrentTimestamp() {
    time_t now = time(0);
    struct tm* t = localtime(&now);
    char buf[20];
    // strftime formats the time struct into a string
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    return std::string(buf);
}

/*
 * toLower - Converts a string to lowercase
 *
 * Used for case-insensitive searching. We loop through each
 * character and convert it individually.
 */
inline std::string toLower(const std::string& s) {
    std::string result = s;
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = tolower(result[i]);
    }
    return result;
}

/*
 * trim - Removes whitespace from both ends of a string
 */
inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

/*
 * printBoxLine - Draws a horizontal box line
 * 
 * Creates lines like: +--------------------+
 * Used to build box-drawn borders around menus and tables.
 */
inline void printBoxLine(int width, char corner = '+', char fill = '-') {
    std::cout << corner;
    for (int i = 0; i < width - 2; i++) std::cout << fill;
    std::cout << corner << std::endl;
}

/*
 * printBoxText - Prints text inside a box border
 *
 * Creates lines like: | Some text here     |
 * The text is left-aligned and padded to fill the box width.
 */
inline void printBoxText(const std::string& text, int width, 
                          const std::string& color = "") {
    std::cout << "| " << color << std::left << std::setw(width - 4) 
              << text << Color::RESET << " |" << std::endl;
}

/*
 * clearScreen - Clears the console screen
 *
 * On Windows, we use the "cls" system command.
 */
inline void clearScreen() {
    system("cls");
}

/*
 * pauseScreen - Waits for user to press Enter
 */
inline void pauseScreen() {
    std::cout << "\n" << Color::DIM << "Press Enter to continue..." 
              << Color::RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

#endif // UTILS_H
