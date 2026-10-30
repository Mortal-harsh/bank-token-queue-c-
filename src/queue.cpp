/*
 * queue.cpp - ServiceQueue Class Implementation
 *
 * BEGINNER EXPLANATION:
 *   A deque (pronounced "deck") is like a list where you can 
 *   add/remove from both ends efficiently.
 *
 *   - push_back(x)   : adds x to the end (normal customers)
 *   - push_front(x)  : adds x to the front (priority customers)
 *   - front()        : looks at the first element
 *   - pop_front()    : removes the first element
 */

#include "queue.h"
#include <algorithm>  // for std::find

// Default constructor
ServiceQueue::ServiceQueue() : serviceType(0) {}

// Constructor with service type
ServiceQueue::ServiceQueue(int serviceType) : serviceType(serviceType) {}

// ============================================================
// enqueue - Add a token to the queue
// ============================================================
// Priority tokens go to the front; normal tokens go to the back.
// 
// IMPORTANT: Priority tokens go to the front of normal tokens,
// but BEHIND other priority tokens that are already waiting.
// This ensures fairness among priority customers too.

void ServiceQueue::enqueue(int tokenId, bool isPriority) {
    if (isPriority) {
        // Insert at the front (before all normal tokens)
        // This gives priority customers preferential treatment
        waitingTokenIds.push_front(tokenId);
    } else {
        // Normal FIFO: add to the back of the line
        waitingTokenIds.push_back(tokenId);
    }
}

// ============================================================
// dequeue - Remove and return the front token
// ============================================================
// Returns the token ID of the next person to be served.
// Returns -1 if nobody is waiting.

int ServiceQueue::dequeue() {
    if (waitingTokenIds.empty()) {
        return -1;  // No one waiting
    }
    int frontId = waitingTokenIds.front();  // Get the first person
    waitingTokenIds.pop_front();            // Remove them from the queue
    return frontId;
}

// ============================================================
// peek - Look at the front token without removing
// ============================================================
int ServiceQueue::peek() const {
    if (waitingTokenIds.empty()) return -1;
    return waitingTokenIds.front();
}

// ============================================================
// isEmpty - Check if queue has no waiting tokens
// ============================================================
bool ServiceQueue::isEmpty() const {
    return waitingTokenIds.empty();
}

// ============================================================
// size - Get number of waiting tokens
// ============================================================
int ServiceQueue::size() const {
    return (int)waitingTokenIds.size();
}

// ============================================================
// removeToken - Remove a specific token from anywhere in queue
// ============================================================
// Used when cancelling a token. We search through the queue
// and remove it if found.
//
// std::find searches for a value in a range [begin, end).
// If found, erase() removes it.

bool ServiceQueue::removeToken(int tokenId) {
    std::deque<int>::iterator it = std::find(
        waitingTokenIds.begin(), waitingTokenIds.end(), tokenId
    );
    if (it != waitingTokenIds.end()) {
        waitingTokenIds.erase(it);
        return true;   // Found and removed
    }
    return false;      // Not found
}

// ============================================================
// getAllTokenIds - Get a copy of all waiting token IDs
// ============================================================
std::vector<int> ServiceQueue::getAllTokenIds() const {
    return std::vector<int>(waitingTokenIds.begin(), waitingTokenIds.end());
}
