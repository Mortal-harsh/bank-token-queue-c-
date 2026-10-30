/*
 * bank_system.cpp - BankSystem Orchestrator Implementation
 *
 * BEGINNER EXPLANATION:
 *   This file implements the "brain" of the banking system.
 *   
 *   Key concepts used here:
 *   - std::map: A dictionary that maps keys to values.
 *     allTokens[42] gives you the Token with id=42.
 *   - Iterator: A pointer-like object for looping through a map.
 *     std::map<int, Token>::iterator it = allTokens.begin();
 *   - const_iterator: Same, but for read-only access.
 */

#include "bank_system.h"
#include "utils.h"

// ============================================================
// Counter Configuration
// ============================================================
// This defines how many physical counters each service type has.
// Index 0 = Deposits (2 counters)
// Index 1 = Withdrawals (2 counters)
// Index 2 = Account Services (1 counter)
// Index 3 = Loans (1 counter)
// Total = 6 counters

const int BankSystem::COUNTERS_PER_SERVICE[] = {2, 2, 1, 1};

// ============================================================
// Constructor
// ============================================================
// Sets up the initial state: creates queues and counters.

BankSystem::BankSystem() : nextTokenId(1) {
    // Initialize the 4 service queues
    for (int i = 0; i < SERVICE_COUNT; i++) {
        queues[i] = ServiceQueue(i);
    }

    // Create counters based on the configuration
    // Counter numbering is sequential across all service types
    int counterNum = 0;
    for (int svc = 0; svc < SERVICE_COUNT; svc++) {
        for (int c = 0; c < COUNTERS_PER_SERVICE[svc]; c++) {
            counters.push_back(Counter(counterNum, svc));
            counterNum++;
        }
    }
}

// ============================================================
// issueToken - Create a new token for a customer
// ============================================================
// Steps:
// 1. Create a Token object with the next available ID
// 2. Store it in the master map
// 3. Add it to the appropriate service queue
// 4. Increment the ID counter
// 5. Return the new token's ID

int BankSystem::issueToken(const std::string& name, const std::string& phone,
                           int serviceType, bool isPriority) {
    // Validate the service type
    if (serviceType < 0 || serviceType >= SERVICE_COUNT) {
        return -1;  // Invalid service type
    }

    // Create the token
    Token token(nextTokenId, name, phone, serviceType, isPriority);

    // Store in the master map
    allTokens[nextTokenId] = token;

    // Add to the appropriate queue
    queues[serviceType].enqueue(nextTokenId, isPriority);

    // Increment for next time
    int issuedId = nextTokenId;
    nextTokenId++;

    return issuedId;
}

// ============================================================
// callNextToken - Serve the next person at a counter
// ============================================================
// Steps:
// 1. Check that the counter exists and is free
// 2. Pull the next token from that counter's service queue
// 3. Update the token's status to "Serving"
// 4. Mark the counter as busy

int BankSystem::callNextToken(int counterIndex) {
    // Validate counter index
    if (counterIndex < 0 || counterIndex >= (int)counters.size()) {
        return -1;
    }

    Counter& counter = counters[counterIndex];

    // Counter must be free to call next
    if (counter.isBusy) {
        return -1;  // Already serving someone
    }

    // Get the service type this counter handles
    int svc = counter.serviceType;

    // Pull the next token from the queue
    int tokenId = queues[svc].dequeue();
    if (tokenId == -1) {
        return -1;  // Queue is empty, no one waiting
    }

    // Update the token's status
    Token* token = getToken(tokenId);
    if (token) {
        token->status = STATUS_SERVING;
        token->servedAt = getCurrentTimestamp();
        token->counterNumber = counterIndex;
    }

    // Mark the counter as busy
    counter.startServing(tokenId);

    return tokenId;
}

// ============================================================
// completeService - Finish serving the current customer
// ============================================================
// Steps:
// 1. Check that the counter is busy
// 2. Update the token's status to "Served"
// 3. Free the counter

int BankSystem::completeService(int counterIndex) {
    if (counterIndex < 0 || counterIndex >= (int)counters.size()) {
        return -1;
    }

    Counter& counter = counters[counterIndex];

    if (!counter.isBusy) {
        return -1;  // Counter is already free
    }

    int tokenId = counter.currentTokenId;

    // Update token status
    Token* token = getToken(tokenId);
    if (token) {
        token->status = STATUS_SERVED;
        token->completedAt = getCurrentTimestamp();
    }

    // Free the counter
    counter.finishServing();

    return tokenId;
}

// ============================================================
// cancelToken - Cancel a waiting token
// ============================================================
bool BankSystem::cancelToken(int tokenId) {
    Token* token = getToken(tokenId);
    if (!token) return false;
    
    // Can only cancel tokens that are waiting
    if (token->status != STATUS_WAITING) return false;

    // Remove from the service queue
    queues[token->serviceType].removeToken(tokenId);

    // Update status
    token->status = STATUS_CANCELLED;
    return true;
}

// ============================================================
// searchTokens - Find tokens matching a query string
// ============================================================
// Searches across token ID, customer name, and phone number.
// Case-insensitive matching using the toLower() helper.

std::vector<Token> BankSystem::searchTokens(const std::string& query) const {
    std::vector<Token> results;
    std::string lowerQuery = toLower(trim(query));

    if (lowerQuery.empty()) return results;

    // Loop through all tokens
    std::map<int, Token>::const_iterator it;
    for (it = allTokens.begin(); it != allTokens.end(); ++it) {
        const Token& t = it->second;

        // Check if query matches token ID, name, or phone
        bool match = false;
        if (toLower(t.tokenId).find(lowerQuery) != std::string::npos) match = true;
        if (toLower(t.customerName).find(lowerQuery) != std::string::npos) match = true;
        if (toLower(t.phone).find(lowerQuery) != std::string::npos) match = true;
        if (toLower(statusName(t.status)).find(lowerQuery) != std::string::npos) match = true;
        if (toLower(serviceTypeName(t.serviceType)).find(lowerQuery) != std::string::npos) match = true;

        if (match) {
            results.push_back(t);
        }
    }

    return results;
}

// ============================================================
// Statistics Methods
// ============================================================
// These loop through all tokens and count how many are in each status.

int BankSystem::getTotalIssued() const {
    return (int)allTokens.size();
}

int BankSystem::getTotalWaiting() const {
    int count = 0;
    std::map<int, Token>::const_iterator it;
    for (it = allTokens.begin(); it != allTokens.end(); ++it) {
        if (it->second.status == STATUS_WAITING) count++;
    }
    return count;
}

int BankSystem::getTotalServing() const {
    int count = 0;
    std::map<int, Token>::const_iterator it;
    for (it = allTokens.begin(); it != allTokens.end(); ++it) {
        if (it->second.status == STATUS_SERVING) count++;
    }
    return count;
}

int BankSystem::getTotalServed() const {
    int count = 0;
    std::map<int, Token>::const_iterator it;
    for (it = allTokens.begin(); it != allTokens.end(); ++it) {
        if (it->second.status == STATUS_SERVED) count++;
    }
    return count;
}

int BankSystem::getTotalCancelled() const {
    int count = 0;
    std::map<int, Token>::const_iterator it;
    for (it = allTokens.begin(); it != allTokens.end(); ++it) {
        if (it->second.status == STATUS_CANCELLED) count++;
    }
    return count;
}

int BankSystem::getWaitingForService(int serviceType) const {
    if (serviceType < 0 || serviceType >= SERVICE_COUNT) return 0;
    return queues[serviceType].size();
}

// ============================================================
// Getters
// ============================================================

Token* BankSystem::getToken(int tokenId) {
    std::map<int, Token>::iterator it = allTokens.find(tokenId);
    if (it != allTokens.end()) {
        return &(it->second);
    }
    return 0;  // nullptr equivalent for C++98/11 compatibility
}

const Token* BankSystem::getToken(int tokenId) const {
    std::map<int, Token>::const_iterator it = allTokens.find(tokenId);
    if (it != allTokens.end()) {
        return &(it->second);
    }
    return 0;
}

int BankSystem::getCounterCount() const {
    return (int)counters.size();
}
