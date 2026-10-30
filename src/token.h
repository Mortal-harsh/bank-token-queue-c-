/*
 * token.h - Token Class Definition
 *
 * PURPOSE:
 *   Defines the Token class, which represents a single customer's 
 *   bank token. Each token stores all information about one customer's
 *   visit to the bank.
 *
 * BEGINNER EXPLANATION:
 *   A "class" is like a blueprint for creating objects. The Token class
 *   is a blueprint for creating token objects. Each token object will
 *   have its own copy of all the data fields (id, name, phone, etc.)
 *   and can use all the methods (functions) defined here.
 *
 *   "public:" means anyone can access these members.
 *   "private:" would mean only the class itself can access them.
 *   We use all public here for simplicity (struct-like).
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    // === Data Fields ===
    int id;                     // Unique numeric ID (e.g., 1, 2, 3...)
    std::string tokenId;        // Formatted ID string (e.g., "TKN-0001")
    std::string customerName;   // Customer's full name
    std::string phone;          // Customer's phone number
    int serviceType;            // Which service (0=Deposit, 1=Withdrawal, etc.)
    bool isPriority;            // true if senior citizen or specially-abled
    int status;                 // Current status (Waiting, Serving, Served, Cancelled)
    std::string issuedAt;       // Timestamp when token was issued
    std::string servedAt;       // Timestamp when service started (empty if not yet)
    std::string completedAt;    // Timestamp when service completed (empty if not yet)
    int counterNumber;          // Which counter served this token (-1 if not yet)

    // === Constructors ===
    // Default constructor - creates an empty token
    Token();

    // Parameterized constructor - creates a fully filled token
    Token(int id, const std::string& name, const std::string& phone,
          int serviceType, bool isPriority);

    // === Methods ===

    // Convert this token's data to a CSV line for saving to file
    std::string toCSV() const;

    // Create a Token object from a CSV line (reverse of toCSV)
    static Token fromCSV(const std::string& csvLine);

    // Display this token's information in a formatted way
    void display() const;
};

#endif // TOKEN_H
