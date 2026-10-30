/*
 * counter.cpp - Counter Class Implementation
 */

#include "counter.h"
#include "utils.h"
#include <iostream>
#include <sstream>

// Default constructor
Counter::Counter() 
    : counterNumber(0), serviceType(0), isBusy(false), currentTokenId(-1) {}

// ============================================================
// Parameterized Constructor
// ============================================================
// Creates a counter with a number and assigned service type.
// Automatically generates a display name like "Counter 1 - Deposits"

Counter::Counter(int number, int serviceType)
    : counterNumber(number), serviceType(serviceType), 
      isBusy(false), currentTokenId(-1) {
    
    std::stringstream ss;
    ss << "Counter " << (number + 1) << " - " << serviceTypeName(serviceType);
    counterName = ss.str();
}

// ============================================================
// startServing - Begin serving a customer
// ============================================================
// Called when a token is pulled from the queue and assigned
// to this counter.

void Counter::startServing(int tokenId) {
    isBusy = true;
    currentTokenId = tokenId;
}

// ============================================================
// finishServing - Done with current customer
// ============================================================
// Called when the counter staff completes the service.

void Counter::finishServing() {
    isBusy = false;
    currentTokenId = -1;
}

// ============================================================
// display - Show counter status in the console
// ============================================================
void Counter::display() const {
    std::string statusStr;
    if (isBusy) {
        statusStr = Color::CYAN + "BUSY" + Color::RESET + " - Serving " 
                  + Color::BOLD + formatTokenId(currentTokenId) + Color::RESET;
    } else {
        statusStr = Color::GREEN + "FREE" + Color::RESET + " - Ready";
    }
    
    std::cout << "  [" << Color::BOLD << (counterNumber + 1) << Color::RESET 
              << "] " << std::left << std::setw(30) << counterName 
              << " | " << statusStr << std::endl;
}

// ============================================================
// CSV Serialization
// ============================================================
std::string Counter::toCSV() const {
    std::stringstream ss;
    ss << counterNumber << "|" << serviceType << "|" 
       << (isBusy ? 1 : 0) << "|" << currentTokenId;
    return ss.str();
}

Counter Counter::fromCSV(const std::string& csvLine) {
    Counter c;
    std::stringstream ss(csvLine);
    std::string field;

    if (std::getline(ss, field, '|')) c.counterNumber = std::stoi(field);
    if (std::getline(ss, field, '|')) c.serviceType = std::stoi(field);
    if (std::getline(ss, field, '|')) c.isBusy = (field == "1");
    if (std::getline(ss, field, '|')) c.currentTokenId = std::stoi(field);

    // Rebuild the display name
    std::stringstream nameStream;
    nameStream << "Counter " << (c.counterNumber + 1) << " - " 
               << serviceTypeName(c.serviceType);
    c.counterName = nameStream.str();

    return c;
}
