/*
 * counter.h - Counter Class Definition
 *
 * PURPOSE:
 *   Represents a physical service counter at the bank.
 *   Each counter handles one type of service and can serve
 *   one customer at a time.
 *
 * BEGINNER EXPLANATION:
 *   Think of this as one of those numbered windows at a bank:
 *   "Counter 1 - Deposits", "Counter 2 - Withdrawals", etc.
 *   A counter is either free (waiting for next customer) or
 *   busy (currently serving someone).
 */

#ifndef COUNTER_H
#define COUNTER_H

#include <string>

class Counter {
public:
    int counterNumber;         // Counter index (0-based internally)
    std::string counterName;   // Display name (e.g., "Counter 1 - Deposits")
    int serviceType;           // Which service type this counter handles
    bool isBusy;               // true if currently serving someone
    int currentTokenId;        // ID of the token being served (-1 if free)

    // Constructors
    Counter();
    Counter(int number, int serviceType);

    // Mark this counter as busy with the given token
    void startServing(int tokenId);

    // Mark this counter as free (done serving)
    void finishServing();

    // Display counter status
    void display() const;

    // Serialize to/from CSV
    std::string toCSV() const;
    static Counter fromCSV(const std::string& csvLine);
};

#endif // COUNTER_H
