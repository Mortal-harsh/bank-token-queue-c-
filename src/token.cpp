/*
 * token.cpp - Token Class Implementation
 *
 * PURPOSE:
 *   Implements all the methods declared in token.h.
 *
 * BEGINNER EXPLANATION:
 *   In C++, we usually DECLARE things (say what exists) in .h files
 *   and DEFINE things (say how they work) in .cpp files.
 *   The "::" syntax (e.g., Token::Token) means "this function belongs
 *   to the Token class".
 */

#include "token.h"
#include "utils.h"
#include <iostream>
#include <sstream>

// ============================================================
// Default Constructor
// ============================================================
// Creates a token with default (empty/zero) values.
// This is needed when we create arrays or vectors of tokens
// before filling them with real data.

Token::Token() 
    : id(0), serviceType(0), isPriority(false), 
      status(STATUS_WAITING), counterNumber(-1) {
    // All string fields are automatically empty
    // counterNumber = -1 means "not assigned to any counter yet"
}

// ============================================================
// Parameterized Constructor
// ============================================================
// Creates a fully populated token. Called when a new customer
// walks in and requests a token.
//
// The ": field(value)" syntax after the parentheses is called
// an "initializer list" - it's the preferred C++ way to set
// member variables during construction.

Token::Token(int id, const std::string& name, const std::string& phone,
             int serviceType, bool isPriority)
    : id(id), customerName(name), phone(phone),
      serviceType(serviceType), isPriority(isPriority),
      status(STATUS_WAITING), counterNumber(-1) {
    
    // Generate the formatted token ID (e.g., "TKN-0042")
    tokenId = formatTokenId(id);
    
    // Record the current time as the issue time
    issuedAt = getCurrentTimestamp();
}

// ============================================================
// toCSV - Serialize token data to a CSV string
// ============================================================
// Converts all fields into a single comma-separated line.
// This is how we save token data to a file.
//
// EXAMPLE OUTPUT:
//   "42,TKN-0042,John Doe,9876543210,0,1,0,2026-01-15 10:30:00,,,-1"
//
// We use a pipe '|' as delimiter instead of comma because
// customer names might contain commas.

std::string Token::toCSV() const {
    std::stringstream ss;
    ss << id << "|"
       << tokenId << "|"
       << customerName << "|"
       << phone << "|"
       << serviceType << "|"
       << (isPriority ? 1 : 0) << "|"
       << status << "|"
       << issuedAt << "|"
       << servedAt << "|"
       << completedAt << "|"
       << counterNumber;
    return ss.str();
}

// ============================================================
// fromCSV - Deserialize a CSV string back into a Token
// ============================================================
// This is the reverse of toCSV(). It reads a saved line and
// recreates the Token object.
//
// "static" means you call it on the class itself, not on an
// object: Token::fromCSV(line)   not   myToken.fromCSV(line)

Token Token::fromCSV(const std::string& csvLine) {
    Token t;
    std::stringstream ss(csvLine);
    std::string field;

    // std::getline with '|' delimiter reads one field at a time
    if (std::getline(ss, field, '|')) t.id = std::stoi(field);
    if (std::getline(ss, field, '|')) t.tokenId = field;
    if (std::getline(ss, field, '|')) t.customerName = field;
    if (std::getline(ss, field, '|')) t.phone = field;
    if (std::getline(ss, field, '|')) t.serviceType = std::stoi(field);
    if (std::getline(ss, field, '|')) t.isPriority = (field == "1");
    if (std::getline(ss, field, '|')) t.status = std::stoi(field);
    if (std::getline(ss, field, '|')) t.issuedAt = field;
    if (std::getline(ss, field, '|')) t.servedAt = field;
    if (std::getline(ss, field, '|')) t.completedAt = field;
    if (std::getline(ss, field, '|')) t.counterNumber = std::stoi(field);

    return t;
}

// ============================================================
// display - Print token information in a formatted way
// ============================================================
// Shows all the important details about this token with colors.

void Token::display() const {
    std::string priorityStr = isPriority 
        ? (Color::MAGENTA + " [PRIORITY]" + Color::RESET) 
        : "";
    
    std::cout << "  " << Color::BOLD << Color::CYAN << tokenId << Color::RESET
              << priorityStr << std::endl;
    std::cout << "    Name    : " << customerName << std::endl;
    std::cout << "    Phone   : " << phone << std::endl;
    std::cout << "    Service : " << serviceTypeName(serviceType) << std::endl;
    std::cout << "    Status  : " << statusColor(status) << statusName(status) 
              << Color::RESET << std::endl;
    std::cout << "    Issued  : " << issuedAt << std::endl;
    
    if (!servedAt.empty()) {
        std::cout << "    Served  : " << servedAt << std::endl;
    }
    if (!completedAt.empty()) {
        std::cout << "    Done    : " << completedAt << std::endl;
    }
    if (counterNumber >= 0) {
        std::cout << "    Counter : #" << (counterNumber + 1) << std::endl;
    }
}
