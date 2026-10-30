/*
 * test_main.cpp - Automated Test Suite
 *
 * PURPOSE:
 *   This file contains automated tests that verify the core
 *   functionality of the Bank Token Management System.
 *
 * BEGINNER EXPLANATION:
 *   "Testing" means writing code that checks if other code works
 *   correctly. Each test function:
 *   1. Sets up some data
 *   2. Calls a function
 *   3. Checks if the result is what we expected
 *
 *   If a test passes, it prints a green checkmark.
 *   If a test fails, it prints a red X and what went wrong.
 *
 *   We use a simple ASSERT macro. A macro is like a text 
 *   shortcut — the compiler replaces ASSERT(x) with the 
 *   full if-else block before compiling.
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>

// Include all project headers
#include "../src/utils.h"
#include "../src/token.h"
#include "../src/token.cpp"
#include "../src/queue.h"
#include "../src/queue.cpp"
#include "../src/counter.h"
#include "../src/counter.cpp"
#include "../src/bank_system.h"
#include "../src/bank_system.cpp"
#include "../src/dashboard.h"
#include "../src/dashboard.cpp"
#include "../src/report.h"
#include "../src/report.cpp"
#include "../src/persist.h"
#include "../src/persist.cpp"

// Enable colors on Windows
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
void enableTestColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    SetConsoleOutputCP(65001);
}
#else
void enableTestColors() {}
#endif

// ============================================================
// Test Framework (Minimal)
// ============================================================

int totalTests = 0;
int passedTests = 0;
int failedTests = 0;

// ASSERT macro: checks a condition and reports pass/fail
#define ASSERT(condition, message) \
    totalTests++; \
    if (condition) { \
        passedTests++; \
        std::cout << Color::GREEN << "  PASS " << Color::RESET \
                  << message << std::endl; \
    } else { \
        failedTests++; \
        std::cout << Color::RED << "  FAIL " << Color::RESET \
                  << message << std::endl; \
    }

// ============================================================
// Test 1: Token Creation
// ============================================================
void testTokenCreation() {
    std::cout << "\n" << Color::BOLD << "Test: Token Creation" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    Token t(1, "Alice Smith", "1234567890", SERVICE_DEPOSIT, false);

    ASSERT(t.id == 1, "Token ID is 1");
    ASSERT(t.tokenId == "TKN-0001", "Formatted ID is TKN-0001");
    ASSERT(t.customerName == "Alice Smith", "Customer name stored correctly");
    ASSERT(t.phone == "1234567890", "Phone stored correctly");
    ASSERT(t.serviceType == SERVICE_DEPOSIT, "Service type is Deposit");
    ASSERT(t.isPriority == false, "Not a priority token");
    ASSERT(t.status == STATUS_WAITING, "Initial status is Waiting");
    ASSERT(t.counterNumber == -1, "No counter assigned initially");
    ASSERT(!t.issuedAt.empty(), "Issue timestamp is set");

    // Test priority token
    Token pt(2, "Bob Jones", "0987654321", SERVICE_LOAN, true);
    ASSERT(pt.isPriority == true, "Priority flag is set");
    ASSERT(pt.tokenId == "TKN-0002", "Second token ID is TKN-0002");
}

// ============================================================
// Test 2: Token CSV Serialization
// ============================================================
void testTokenCSV() {
    std::cout << "\n" << Color::BOLD << "Test: Token CSV Serialization" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    Token original(5, "Charlie Brown", "5551234567", SERVICE_WITHDRAWAL, true);
    std::string csv = original.toCSV();

    ASSERT(!csv.empty(), "CSV string is not empty");

    Token restored = Token::fromCSV(csv);
    ASSERT(restored.id == original.id, "ID survives round-trip");
    ASSERT(restored.tokenId == original.tokenId, "Token ID survives round-trip");
    ASSERT(restored.customerName == original.customerName, "Name survives round-trip");
    ASSERT(restored.phone == original.phone, "Phone survives round-trip");
    ASSERT(restored.serviceType == original.serviceType, "Service type survives round-trip");
    ASSERT(restored.isPriority == original.isPriority, "Priority survives round-trip");
    ASSERT(restored.status == original.status, "Status survives round-trip");
}

// ============================================================
// Test 3: Queue FIFO Order
// ============================================================
void testQueueFIFO() {
    std::cout << "\n" << Color::BOLD << "Test: Queue FIFO Order" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    ServiceQueue q(SERVICE_DEPOSIT);

    ASSERT(q.isEmpty(), "New queue is empty");
    ASSERT(q.size() == 0, "New queue size is 0");

    // Add three normal tokens
    q.enqueue(10, false);
    q.enqueue(20, false);
    q.enqueue(30, false);

    ASSERT(q.size() == 3, "Queue has 3 items after 3 enqueues");
    ASSERT(!q.isEmpty(), "Queue is not empty");

    // Dequeue should return in FIFO order
    ASSERT(q.dequeue() == 10, "First dequeue returns 10 (first in)");
    ASSERT(q.dequeue() == 20, "Second dequeue returns 20");
    ASSERT(q.dequeue() == 30, "Third dequeue returns 30 (last in)");

    ASSERT(q.isEmpty(), "Queue is empty after all dequeues");
    ASSERT(q.dequeue() == -1, "Dequeue on empty queue returns -1");
}

// ============================================================
// Test 4: Queue Priority Handling
// ============================================================
void testQueuePriority() {
    std::cout << "\n" << Color::BOLD << "Test: Queue Priority Handling" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    ServiceQueue q(SERVICE_DEPOSIT);

    // Add normal tokens first
    q.enqueue(100, false);
    q.enqueue(200, false);

    // Add a priority token — should go to front
    q.enqueue(999, true);

    ASSERT(q.peek() == 999, "Priority token is at the front");
    ASSERT(q.dequeue() == 999, "Priority token is served first");
    ASSERT(q.dequeue() == 100, "Normal tokens follow in FIFO order");
    ASSERT(q.dequeue() == 200, "Last normal token served last");
}

// ============================================================
// Test 5: Queue Remove Token
// ============================================================
void testQueueRemove() {
    std::cout << "\n" << Color::BOLD << "Test: Queue Remove Token" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    ServiceQueue q(SERVICE_ACCOUNT);
    q.enqueue(1, false);
    q.enqueue(2, false);
    q.enqueue(3, false);

    ASSERT(q.removeToken(2), "Remove existing token returns true");
    ASSERT(q.size() == 2, "Queue size is 2 after removal");
    ASSERT(!q.removeToken(99), "Remove non-existing token returns false");

    ASSERT(q.dequeue() == 1, "Token 1 still first");
    ASSERT(q.dequeue() == 3, "Token 3 is next (2 was removed)");
}

// ============================================================
// Test 6: Counter Creation & Lifecycle
// ============================================================
void testCounter() {
    std::cout << "\n" << Color::BOLD << "Test: Counter Lifecycle" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    Counter c(0, SERVICE_DEPOSIT);

    ASSERT(c.counterNumber == 0, "Counter number is 0");
    ASSERT(c.serviceType == SERVICE_DEPOSIT, "Service type is Deposit");
    ASSERT(!c.isBusy, "New counter is free");
    ASSERT(c.currentTokenId == -1, "No token assigned");

    c.startServing(42);
    ASSERT(c.isBusy, "Counter is busy after startServing");
    ASSERT(c.currentTokenId == 42, "Current token is 42");

    c.finishServing();
    ASSERT(!c.isBusy, "Counter is free after finishServing");
    ASSERT(c.currentTokenId == -1, "No token after finishing");
}

// ============================================================
// Test 7: BankSystem - Issue and Serve
// ============================================================
void testBankSystemIssueAndServe() {
    std::cout << "\n" << Color::BOLD << "Test: BankSystem Issue & Serve" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    BankSystem sys;

    // Issue tokens
    int id1 = sys.issueToken("Alice", "111", SERVICE_DEPOSIT, false);
    int id2 = sys.issueToken("Bob", "222", SERVICE_DEPOSIT, false);
    int id3 = sys.issueToken("Charlie", "333", SERVICE_WITHDRAWAL, false);

    ASSERT(id1 == 1, "First token ID is 1");
    ASSERT(id2 == 2, "Second token ID is 2");
    ASSERT(id3 == 3, "Third token ID is 3");
    ASSERT(sys.getTotalIssued() == 3, "Total issued is 3");
    ASSERT(sys.getTotalWaiting() == 3, "All 3 are waiting");

    // Call next at counter 0 (Deposit counter)
    int served = sys.callNextToken(0);
    ASSERT(served == id1, "First deposit token called (FIFO)");
    ASSERT(sys.getTotalServing() == 1, "1 being served");
    ASSERT(sys.getTotalWaiting() == 2, "2 still waiting");

    // Complete service
    int completed = sys.completeService(0);
    ASSERT(completed == id1, "Token 1 completed");
    ASSERT(sys.getTotalServed() == 1, "1 served");
    ASSERT(sys.getTotalServing() == 0, "0 being served");
}

// ============================================================
// Test 8: BankSystem - Priority Serving
// ============================================================
void testBankSystemPriority() {
    std::cout << "\n" << Color::BOLD << "Test: BankSystem Priority Serving" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    BankSystem sys;

    int id1 = sys.issueToken("Normal1", "111", SERVICE_LOAN, false);
    int id2 = sys.issueToken("Normal2", "222", SERVICE_LOAN, false);
    int id3 = sys.issueToken("Senior", "333", SERVICE_LOAN, true);  // Priority!

    // Counter 5 is the Loan counter (after 2 deposit + 2 withdrawal + 1 account)
    int served = sys.callNextToken(5);
    ASSERT(served == id3, "Priority token served first, before normal tokens");

    sys.completeService(5);
    served = sys.callNextToken(5);
    ASSERT(served == id1, "Normal tokens served in FIFO order after priority");
}

// ============================================================
// Test 9: Search
// ============================================================
void testSearch() {
    std::cout << "\n" << Color::BOLD << "Test: Search" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    BankSystem sys;
    sys.issueToken("Aarav Sharma", "9876543210", SERVICE_DEPOSIT, false);
    sys.issueToken("Priya Patel", "9123456789", SERVICE_WITHDRAWAL, false);
    sys.issueToken("Aarav Kumar", "9988776655", SERVICE_ACCOUNT, false);

    // Search by name
    std::vector<Token> results = sys.searchTokens("aarav");
    ASSERT(results.size() == 2, "Search 'aarav' finds 2 results");

    // Search by phone
    results = sys.searchTokens("9876543210");
    ASSERT(results.size() == 1, "Search by phone finds 1 result");
    ASSERT(results[0].customerName == "Aarav Sharma", "Correct person found by phone");

    // Search by token ID
    results = sys.searchTokens("TKN-0002");
    ASSERT(results.size() == 1, "Search by token ID finds 1 result");

    // Search by service type name
    results = sys.searchTokens("deposit");
    ASSERT(results.size() == 1, "Search 'deposit' finds 1 deposit token");

    // Search with no results
    results = sys.searchTokens("nonexistent");
    ASSERT(results.size() == 0, "Search for nonexistent returns 0");
}

// ============================================================
// Test 10: Cancel Token
// ============================================================
void testCancelToken() {
    std::cout << "\n" << Color::BOLD << "Test: Cancel Token" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    BankSystem sys;
    int id1 = sys.issueToken("Alice", "111", SERVICE_DEPOSIT, false);
    int id2 = sys.issueToken("Bob", "222", SERVICE_DEPOSIT, false);

    ASSERT(sys.cancelToken(id1), "Cancel waiting token succeeds");
    ASSERT(sys.getTotalCancelled() == 1, "Cancelled count is 1");
    ASSERT(sys.getWaitingForService(SERVICE_DEPOSIT) == 1, "Queue has 1 after cancel");

    // Call next should skip cancelled and serve id2
    int served = sys.callNextToken(0);
    ASSERT(served == id2, "Cancelled token is skipped, id2 served");

    // Can't cancel a serving token
    ASSERT(!sys.cancelToken(id2), "Cannot cancel a token being served");
}

// ============================================================
// Test 11: Persistence (Save & Load)
// ============================================================
void testPersistence() {
    std::cout << "\n" << Color::BOLD << "Test: Persistence (Save & Load)" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    // Create a system with data
    BankSystem original;
    original.issueToken("PersistTest1", "111", SERVICE_DEPOSIT, false);
    original.issueToken("PersistTest2", "222", SERVICE_WITHDRAWAL, true);
    original.issueToken("PersistTest3", "333", SERVICE_DEPOSIT, false);
    original.callNextToken(0);  // Serve first deposit token

    // Save
    bool saved = Persistence::saveAll(original);
    ASSERT(saved, "Save succeeds");

    // Load into a new system
    BankSystem loaded;
    bool loadResult = Persistence::loadAll(loaded);
    ASSERT(loadResult, "Load succeeds");
    ASSERT(loaded.allTokens.size() == original.allTokens.size(), 
           "Same number of tokens after reload");
    ASSERT(loaded.nextTokenId == original.nextTokenId, 
           "Next token ID preserved");

    // Verify a specific token survived
    const Token* t = loaded.getToken(2);
    ASSERT(t != 0, "Token 2 exists after reload");
    if (t) {
        ASSERT(t->customerName == "PersistTest2", "Token 2 name preserved");
        ASSERT(t->isPriority == true, "Token 2 priority preserved");
    }
}

// ============================================================
// Test 12: Statistics
// ============================================================
void testStatistics() {
    std::cout << "\n" << Color::BOLD << "Test: Statistics" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    BankSystem sys;
    sys.issueToken("A", "1", SERVICE_DEPOSIT, false);
    sys.issueToken("B", "2", SERVICE_DEPOSIT, false);
    sys.issueToken("C", "3", SERVICE_WITHDRAWAL, false);
    sys.issueToken("D", "4", SERVICE_LOAN, false);
    sys.issueToken("E", "5", SERVICE_ACCOUNT, true);

    ASSERT(sys.getTotalIssued() == 5, "5 tokens issued");
    ASSERT(sys.getTotalWaiting() == 5, "5 waiting initially");
    ASSERT(sys.getWaitingForService(SERVICE_DEPOSIT) == 2, "2 waiting for Deposit");
    ASSERT(sys.getWaitingForService(SERVICE_WITHDRAWAL) == 1, "1 waiting for Withdrawal");
    ASSERT(sys.getWaitingForService(SERVICE_LOAN) == 1, "1 waiting for Loan");
    ASSERT(sys.getWaitingForService(SERVICE_ACCOUNT) == 1, "1 waiting for Account");

    sys.callNextToken(0); // Serve deposit
    ASSERT(sys.getTotalServing() == 1, "1 serving");
    ASSERT(sys.getTotalWaiting() == 4, "4 waiting");

    sys.completeService(0);
    ASSERT(sys.getTotalServed() == 1, "1 served");
    ASSERT(sys.getTotalServing() == 0, "0 serving");
}

// ============================================================
// Test 13: Edge Cases
// ============================================================
void testEdgeCases() {
    std::cout << "\n" << Color::BOLD << "Test: Edge Cases" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    BankSystem sys;

    // Serve from empty queue
    int result = sys.callNextToken(0);
    ASSERT(result == -1, "Calling next from empty queue returns -1");

    // Complete on free counter
    result = sys.completeService(0);
    ASSERT(result == -1, "Completing on free counter returns -1");

    // Invalid counter index
    result = sys.callNextToken(99);
    ASSERT(result == -1, "Invalid counter index returns -1");

    // Invalid service type
    result = sys.issueToken("Test", "123", 99, false);
    ASSERT(result == -1, "Invalid service type returns -1");

    // Cancel non-existent token
    ASSERT(!sys.cancelToken(999), "Cancel non-existent token returns false");
}

// ============================================================
// Test 14: Utility Functions
// ============================================================
void testUtilities() {
    std::cout << "\n" << Color::BOLD << "Test: Utility Functions" 
              << Color::RESET << "\n";
    std::cout << std::string(40, '-') << "\n";

    ASSERT(formatTokenId(1) == "TKN-0001", "formatTokenId(1) = TKN-0001");
    ASSERT(formatTokenId(42) == "TKN-0042", "formatTokenId(42) = TKN-0042");
    ASSERT(formatTokenId(9999) == "TKN-9999", "formatTokenId(9999) = TKN-9999");

    ASSERT(toLower("Hello WORLD") == "hello world", "toLower works");
    ASSERT(trim("  hello  ") == "hello", "trim works");
    ASSERT(trim("") == "", "trim empty string works");

    ASSERT(serviceTypeName(SERVICE_DEPOSIT) == "Deposit", "serviceTypeName Deposit");
    ASSERT(serviceTypeName(SERVICE_LOAN) == "Loan", "serviceTypeName Loan");
    ASSERT(statusName(STATUS_WAITING) == "Waiting", "statusName Waiting");
    ASSERT(statusName(STATUS_SERVED) == "Served", "statusName Served");

    std::string ts = getCurrentTimestamp();
    ASSERT(ts.length() == 19, "Timestamp has correct length (YYYY-MM-DD HH:MM:SS)");
}

// ============================================================
// Main Test Runner
// ============================================================
int main() {
    enableTestColors();

    std::cout << "\n";
    std::cout << Color::BOLD << Color::CYAN;
    printBoxLine(56, '+', '=');
    printBoxText("BANK TOKEN MANAGEMENT SYSTEM - TEST SUITE", 56, Color::BOLD + Color::CYAN);
    printBoxLine(56, '+', '=');
    std::cout << Color::RESET << "\n";

    // Run all tests
    testTokenCreation();
    testTokenCSV();
    testQueueFIFO();
    testQueuePriority();
    testQueueRemove();
    testCounter();
    testBankSystemIssueAndServe();
    testBankSystemPriority();
    testSearch();
    testCancelToken();
    testPersistence();
    testStatistics();
    testEdgeCases();
    testUtilities();

    // Summary
    std::cout << "\n";
    printBoxLine(56, '+', '=');
    std::cout << "  " << Color::BOLD << "RESULTS: " << Color::RESET;
    std::cout << Color::GREEN << passedTests << " passed" << Color::RESET;
    std::cout << ", ";
    if (failedTests > 0) {
        std::cout << Color::RED << failedTests << " failed" << Color::RESET;
    } else {
        std::cout << Color::GREEN << "0 failed" << Color::RESET;
    }
    std::cout << ", " << totalTests << " total" << std::endl;
    printBoxLine(56, '+', '=');

    if (failedTests > 0) {
        std::cout << "\n  " << Color::RED << Color::BOLD 
                  << "SOME TESTS FAILED!" << Color::RESET << "\n\n";
        return 1;
    } else {
        std::cout << "\n  " << Color::GREEN << Color::BOLD 
                  << "ALL TESTS PASSED!" << Color::RESET << "\n\n";
        return 0;
    }
}
