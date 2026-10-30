/*
 * queue.h - ServiceQueue Class Definition
 *
 * PURPOSE:
 *   Defines a queue (waiting line) for one type of bank service.
 *   Implements FIFO (First In, First Out) with priority support.
 *
 * BEGINNER EXPLANATION:
 *   Think of a queue like a line at a bank counter. The person
 *   who arrives first gets served first (FIFO). But if a senior
 *   citizen arrives, they go to the front of the line (priority).
 *
 *   We use a std::deque (double-ended queue) internally because
 *   it supports efficient insertion at both front and back.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "token.h"
#include <deque>
#include <vector>

class ServiceQueue {
public:
    int serviceType;                // Which service this queue is for
    std::deque<int> waitingTokenIds; // Token IDs in order (front = next to serve)

    // Constructor
    ServiceQueue();
    ServiceQueue(int serviceType);

    // Add a token to the queue
    // If isPriority is true, the token goes to the front
    // If isPriority is false, the token goes to the back (normal FIFO)
    void enqueue(int tokenId, bool isPriority);

    // Remove and return the token ID at the front of the queue
    // Returns -1 if the queue is empty
    int dequeue();

    // Look at the front token without removing it
    // Returns -1 if the queue is empty
    int peek() const;

    // Check if the queue is empty
    bool isEmpty() const;

    // Get the number of tokens waiting
    int size() const;

    // Remove a specific token ID from the queue (e.g., for cancellation)
    bool removeToken(int tokenId);

    // Get all token IDs currently in the queue
    std::vector<int> getAllTokenIds() const;
};

#endif // QUEUE_H
