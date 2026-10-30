/*
 * bank_system.h - BankSystem Orchestrator Class
 *
 * PURPOSE:
 *   This is the "brain" of the application. It coordinates all
 *   other classes: tokens, queues, counters, persistence, etc.
 *
 * BEGINNER EXPLANATION:
 *   The BankSystem class is like the bank manager. It doesn't do
 *   any one specific job, but it tells all the other parts what
 *   to do and when. It holds the master list of all tokens, all
 *   queues, and all counters.
 *
 *   We use a std::map to store tokens by their ID for fast lookup.
 *   std::map is like a dictionary: given a key (token ID), you
 *   can instantly find the corresponding Token object.
 */

#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include "token.h"
#include "queue.h"
#include "counter.h"
#include <map>
#include <vector>
#include <string>

class BankSystem {
public:
    // === Data Members ===
    std::map<int, Token> allTokens;           // All tokens ever issued (key = id)
    ServiceQueue queues[4];                   // One queue per service type (4 total)
    std::vector<Counter> counters;            // All service counters
    int nextTokenId;                          // Next available token number

    // Configuration: how many counters per service type
    // Default: 2 Deposit, 2 Withdrawal, 1 Account, 1 Loan = 6 counters
    static const int COUNTERS_PER_SERVICE[];

    // === Constructor ===
    BankSystem();

    // === Core Operations ===

    // Issue a new token to a customer
    // Returns the token ID, or -1 on failure
    int issueToken(const std::string& name, const std::string& phone,
                   int serviceType, bool isPriority);

    // Call the next token for a specific counter
    // Returns the token ID that was called, or -1 if queue is empty
    int callNextToken(int counterIndex);

    // Complete service at a specific counter
    // Returns the token ID that was completed, or -1 if counter was free
    int completeService(int counterIndex);

    // Cancel a waiting token
    bool cancelToken(int tokenId);

    // === Search ===

    // Search tokens by any field (ID, name, phone)
    // Returns a vector of matching Token objects
    std::vector<Token> searchTokens(const std::string& query) const;

    // === Statistics ===

    int getTotalIssued() const;
    int getTotalWaiting() const;
    int getTotalServing() const;
    int getTotalServed() const;
    int getTotalCancelled() const;
    int getWaitingForService(int serviceType) const;

    // === Getters ===

    // Get a token by its numeric ID (returns pointer, nullptr if not found)
    Token* getToken(int tokenId);
    const Token* getToken(int tokenId) const;

    // Get the counter count
    int getCounterCount() const;
};

#endif // BANK_SYSTEM_H
