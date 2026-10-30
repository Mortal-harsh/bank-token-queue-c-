/*
 * persist.cpp - Data Persistence Implementation
 *
 * BEGINNER EXPLANATION:
 *   "Persistence" means making data last beyond the program's
 *   lifetime. Without persistence, everything is lost when you
 *   close the program.
 *
 *   We save data to text files in a "data/" folder. When the
 *   program starts, it checks if these files exist and loads
 *   them back.
 *
 *   ifstream = Input File Stream (reading from a file)
 *   ofstream = Output File Stream (writing to a file)
 */

#include "persist.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

// File paths
const std::string Persistence::DATA_DIR      = "data";
const std::string Persistence::TOKENS_FILE   = "data/tokens.dat";
const std::string Persistence::COUNTERS_FILE = "data/counters.dat";
const std::string Persistence::META_FILE     = "data/meta.dat";
const std::string Persistence::QUEUE_FILE    = "data/queues.dat";

// Helper: ensure the data directory exists
static void ensureDataDir() {
    std::string cmd = "if not exist \"data\" mkdir \"data\"";
    system(cmd.c_str());
}

// ============================================================
// saveAll - Save all system state to files
// ============================================================
bool Persistence::saveAll(const BankSystem& system) {
    ensureDataDir();

    // ---- Save Metadata (next token ID) ----
    {
        std::ofstream file(META_FILE.c_str());
        if (!file.is_open()) return false;
        file << system.nextTokenId << "\n";
        file.close();
    }

    // ---- Save All Tokens ----
    {
        std::ofstream file(TOKENS_FILE.c_str());
        if (!file.is_open()) return false;

        std::map<int, Token>::const_iterator it;
        for (it = system.allTokens.begin(); it != system.allTokens.end(); ++it) {
            file << it->second.toCSV() << "\n";
        }
        file.close();
    }

    // ---- Save Counter States ----
    {
        std::ofstream file(COUNTERS_FILE.c_str());
        if (!file.is_open()) return false;

        for (size_t i = 0; i < system.counters.size(); i++) {
            file << system.counters[i].toCSV() << "\n";
        }
        file.close();
    }

    // ---- Save Queue States ----
    {
        std::ofstream file(QUEUE_FILE.c_str());
        if (!file.is_open()) return false;

        for (int q = 0; q < SERVICE_COUNT; q++) {
            std::vector<int> ids = system.queues[q].getAllTokenIds();
            file << q << ":";
            for (size_t i = 0; i < ids.size(); i++) {
                if (i > 0) file << ",";
                file << ids[i];
            }
            file << "\n";
        }
        file.close();
    }

    return true;
}

// ============================================================
// loadAll - Load all system state from files
// ============================================================
bool Persistence::loadAll(BankSystem& system) {
    if (!hasSavedData()) return false;

    // ---- Load Metadata ----
    {
        std::ifstream file(META_FILE.c_str());
        if (file.is_open()) {
            file >> system.nextTokenId;
            file.close();
        }
    }

    // ---- Load Tokens ----
    {
        std::ifstream file(TOKENS_FILE.c_str());
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty()) continue;
                Token t = Token::fromCSV(line);
                system.allTokens[t.id] = t;
            }
            file.close();
        }
    }

    // ---- Load Counter States ----
    {
        std::ifstream file(COUNTERS_FILE.c_str());
        if (file.is_open()) {
            std::string line;
            size_t idx = 0;
            while (std::getline(file, line) && idx < system.counters.size()) {
                if (line.empty()) continue;
                Counter c = Counter::fromCSV(line);
                // Keep the existing counter structure, update state only
                system.counters[idx].isBusy = c.isBusy;
                system.counters[idx].currentTokenId = c.currentTokenId;
                idx++;
            }
            file.close();
        }
    }

    // ---- Load Queue States ----
    {
        std::ifstream file(QUEUE_FILE.c_str());
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty()) continue;
                
                // Format: "serviceType:id1,id2,id3"
                size_t colonPos = line.find(':');
                if (colonPos == std::string::npos) continue;
                
                int queueIdx = std::stoi(line.substr(0, colonPos));
                if (queueIdx < 0 || queueIdx >= SERVICE_COUNT) continue;

                // Clear existing queue
                system.queues[queueIdx].waitingTokenIds.clear();

                // Parse the comma-separated IDs
                std::string idsPart = line.substr(colonPos + 1);
                if (idsPart.empty()) continue;
                
                std::stringstream ss(idsPart);
                std::string idStr;
                while (std::getline(ss, idStr, ',')) {
                    if (!idStr.empty()) {
                        int tokenId = std::stoi(idStr);
                        // Add to back to preserve order
                        system.queues[queueIdx].waitingTokenIds.push_back(tokenId);
                    }
                }
            }
            file.close();
        }
    }

    return true;
}

// ============================================================
// hasSavedData - Check if save files exist
// ============================================================
bool Persistence::hasSavedData() {
    std::ifstream file(TOKENS_FILE.c_str());
    bool exists = file.is_open();
    if (exists) file.close();
    return exists;
}
