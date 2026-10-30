/*
 * persist.h - Data Persistence (Save/Load)
 *
 * PURPOSE:
 *   Handles saving all system data to files and loading it back.
 *   This ensures data survives program restarts.
 */

#ifndef PERSIST_H
#define PERSIST_H

#include "bank_system.h"
#include <string>

class Persistence {
public:
    static const std::string DATA_DIR;
    static const std::string TOKENS_FILE;
    static const std::string COUNTERS_FILE;
    static const std::string META_FILE;
    static const std::string QUEUE_FILE;

    // Save all system state to files
    static bool saveAll(const BankSystem& system);

    // Load all system state from files
    static bool loadAll(BankSystem& system);

    // Check if saved data exists
    static bool hasSavedData();
};

#endif // PERSIST_H
