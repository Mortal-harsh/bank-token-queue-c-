# ACADEMIC PROJECT REPORT

## Bank Token Management System in C++

**Course:** Computing Aptitude / Object-Oriented Programming with C++  
**Project Title:** Multi-Counter Bank Token Management System with Priority Queue Handling  
**Language:** C++ (C++11 standard)  
**Compiler:** MinGW GNU C++ 6.3.0  
**Operating Environment:** Windows (x64)  

---

### Executive Summary

In traditional banking halls, disorganized queues cause long waiting times, customer dissatisfaction, and inefficient teller allocation. The **Bank Token Management System** is a software solution designed and implemented in C++ to simulate and automate the queue allocation, prioritization, counter dispatching, customer tracking, dashboard analytics, and audit logging of a retail banking facility.

The system incorporates robust Data Structures and Algorithms (Queue, Deque, Map, Vector), Object-Oriented Programming (Encapsulation, Class separation, Composition), file persistence, multi-counter scheduling, and automated unit testing with 100% test pass rate across 102 individual assertions.

---

### 1. Problem Statement & Objectives

#### 1.1 Problem Statement
Banks serve diverse customer needs (cash deposits, cash withdrawals, general account queries, and loan applications). Without an automated queuing system:
1. Priority customers (such as senior citizens or specially-abled individuals) are forced to wait in long standard queues.
2. Tellering workload is unbalanced across physical counters.
3. Bank management lacks real-time visibility into queue depths, counter utilization, and customer throughput.
4. Sudden power outages or system shutdowns can cause total loss of queued customer order.

#### 1.2 System Objectives
- **Unique Identification:** Generate non-colliding, persistent alphanumeric token identifiers (`TKN-0001`, `TKN-0002`).
- **Fair Queuing with Priority Override:** Implement First-In-First-Out (FIFO) queuing per service type while prioritizing senior citizens and specially-abled patrons.
- **Multi-Counter Dispatching:** Route tokens from specific service queues to designated physical teller counters.
- **Real-Time Analytics:** Display live counts of issued, waiting, serving, and served tokens with utilization rates.
- **Audit & Reporting:** Export machine-readable and Excel-compatible CSV reports for operational audit.
- **Fail-Safe Persistence:** Automatically serialize and deserialize queue states and customer histories to disk.
- **Production-Quality Quality Assurance:** Accompany the software with an automated test suite verifying edge cases.

---

### 2. System Architecture & Modular Design

The project is structured according to the Single Responsibility Principle (SRP) and separation of interface (`.h`) from implementation (`.cpp`):

```
                               ┌───────────────────────────┐
                               │         main.cpp          │
                               │  (Console UI & Menu Loop) │
                               └─────────────┬─────────────┘
                                             │
                                             ▼
                               ┌───────────────────────────┐
                               │     BankSystem Class      │
                               │   (Central Orchestrator)  │
                               └──┬──────┬──────┬───────┬──┘
                                  │      │      │       │
             ┌────────────────────┘      │      │       └───────────────────┐
             ▼                           ▼      ▼                           ▼
    ┌─────────────────┐       ┌────────────┐  ┌─────────────┐     ┌──────────────────┐
    │  ServiceQueue   │       │   Counter  │  │ Persistence │     │ ReportGenerator  │
    │ (FIFO+Priority) │       │ (6 Tellers)│  │ (Save/Load) │     │  (CSV Exporter)  │
    └────────┬────────┘       └────────────┘  └─────────────┘     └──────────────────┘
             │
             ▼
      ┌─────────────┐
      │ Token Class │
      └─────────────┘
```

#### Module Responsibilities

1. **`Token` (`token.h`, `token.cpp`):**
   - Represents the atomic customer record.
   - Encapsulates: ID, Token String, Customer Name, Phone Number, Service Category, Priority Flag, Lifecycle Status, and Timestamps (`issuedAt`, `servedAt`, `completedAt`).
   - Implements pipeline serialization: `toCSV()` and deserialization: `fromCSV()`.

2. **`ServiceQueue` (`queue.h`, `queue.cpp`):**
   - Wraps `std::deque<int>` for fast bidirectional operations.
   - Normal customer: Enqueued via `push_back()` (pure FIFO).
   - Priority customer: Enqueued via `push_front()` (preempts standard waitline).
   - Dequeue via `pop_front()` in $O(1)$ constant time complexity.

3. **`Counter` (`counter.h`, `counter.cpp`):**
   - Represents teller windows with dedicated service specialties:
     - Counters 1–2: Deposit Services
     - Counters 3–4: Withdrawal Services
     - Counter 5: Account Services
     - Counter 6: Loan Advisory
   - Tracks busy/free state and current customer token ID.

4. **`BankSystem` (`bank_system.h`, `bank_system.cpp`):**
   - The master orchestrator.
   - Maintains a hash/tree map (`std::map<int, Token>`) for $O(\log N)$ token lookups by unique ID.
   - Coordinates token creation, assignment to counters, service completion, cancellation, fuzzy searching, and mathematical aggregations.

5. **`Dashboard` (`dashboard.h`, `dashboard.cpp`):**
   - Computes live operational indicators: total issued, currently waiting, currently serving, successfully served, cancellation rate, and counter utilization percentage.
   - Renders formatted terminal visuals with ANSI color accents.

6. **`ReportGenerator` (`report.h`, `report.cpp`):**
   - Exports formatted tabular CSV files (`all_tokens_report.csv`, `served_tokens_report.csv`, `statistics_report.csv`) directly into a `reports/` folder.

7. **`Persistence` (`persist.h`, `persist.cpp`):**
   - Persists metadata (next counter ID), tokens table, queue ordering, and teller allocations across program execution sessions into `data/*.dat`.

8. **`Demo Mode` (`demo.h`):**
   - Seeds the database with realistic customer profiles across various categories for live demonstration.

---

### 3. Data Structures & Algorithmic Complexity

| Component | Underlying Data Structure | Operation | Time Complexity | Justification |
|---|---|---|---|---|
| Master Token Registry | `std::map<int, Token>` | Insert, Lookup, Update | $O(\log N)$ | Red-Black Tree provides ordered keys and stable access without rehashing overhead. |
| Waiting Line | `std::deque<int>` | Enqueue Normal | $O(1)$ | Double-ended queue allows constant-time insertion at the tail. |
| Waiting Line | `std::deque<int>` | Enqueue Priority | $O(1)$ | Constant-time insertion at head for expedited servicing. |
| Waiting Line | `std::deque<int>` | Dequeue Next | $O(1)$ | Efficient removal from head when counter calls customer. |
| Multi-Field Search | Linear Vector Scan | Substring matching | $O(N \cdot M)$ | Allows fuzzy substring search across name, phone, token ID, and status. |
| Counters List | `std::vector<Counter>` | Index access | $O(1)$ | Fixed array-like contiguous memory for fixed teller desks. |

---

### 4. Verification & Testing

An independent automated test harness (`tests/test_main.cpp`) was built and executed. The suite comprises 14 functional suites evaluating 102 distinct unit assertions:

1. **Token Instantiation:** Verifies field assignment, string zero-padding (`TKN-0001`), and timestamp formatting.
2. **CSV Serialization:** Asserts bidirectional fidelity (round-trip equality of object $\rightarrow$ string $\rightarrow$ object).
3. **Queue FIFO Invariance:** Validates that sequence of insertion matches sequence of extraction.
4. **Priority Preemption:** Confirms priority tokens bypass waiting standard tokens.
5. **Queue Pruning:** Tests removal of specific tokens upon customer cancellation.
6. **Teller Counter Lifecycle:** Verifies state transitions (`Free` $\rightarrow$ `Busy` $\rightarrow$ `Free`).
7. **Orchestrator End-to-End:** Tests end-to-end issuance, counter calling, and completion.
8. **Multi-Counter Priority Scheduling:** Validates multi-service isolation (deposits do not block loan counters).
9. **Fuzzy Search Engine:** Tests case-insensitive query matching against name, telephone number, and token code.
10. **Cancellation Protocol:** Tests queue removal and status transition without corrupting neighboring tokens.
11. **Persistence & Deserialization:** Simulates application shutdown, loads state from disk, and verifies exact memory equivalence.
12. **Statistical Aggregator:** Verifies numerical accuracy of served, waiting, and cancelled counts.
13. **Defensive Edge Cases:** Asserts safe graceful failure on empty dequeues, invalid counter queries, and out-of-range options.
14. **Helper Utility Functions:** Validates string sanitization, token formatting, and date/time formatting.

**Test Run Output:**
```
+======================================================+
  RESULTS: 102 passed, 0 failed, 102 total
+======================================================+
  ALL TESTS PASSED!
```

---

### 5. Step-by-Step Compilation and Execution Instructions

#### Prerequisites
- MinGW C++ compiler (`g++`) or Microsoft Visual C++ (`cl`)
- Windows 7, 8, 10, or 11

#### Compiling with the included batch script
Double-click `build.bat` or run in terminal:
```cmd
build.bat
```

#### Running the Application
```cmd
BankTokenSystem.exe
```
Or launch with demo data pre-populated:
```cmd
BankTokenSystem.exe --demo
```

#### Running the Test Harness
Double-click `run_tests.bat` or execute:
```cmd
run_tests.bat
```

---

### 6. Screenshots & Sample Outputs

#### Main Interface Menu
```
+================================================+
| BANK TOKEN MANAGEMENT SYSTEM                   |
+================================================+
  [ Issued: 18 | Waiting: 15 | Serving: 2 | Served: 1 ]

+------------------------------------------------+
| 1. Issue New Token                             |
| 2. Call Next Token (Serve)                     |
| 3. Complete Current Service                    |
| 4. View All Queues                             |
| 5. View Counter Status                         |
| 6. Dashboard & Statistics                      |
| 7. Search Token                                |
| 8. Export CSV Reports                          |
| 9. Load Demo Data                              |
+------------------------------------------------+
| 0. Save & Exit                                 |
+------------------------------------------------+
```

#### Real-Time Dashboard
```
+==========================================================+
| DASHBOARD - Bank Token Management System                 |
+==========================================================+

  OVERALL STATISTICS
  ----------------------------------------
  Total Tokens Issued   : 18
  Currently Waiting     : 15
  Currently Being Served: 2
  Successfully Served   : 1
  Cancelled             : 0

  QUEUE STATUS BY SERVICE
  ----------------------------------------
  Service Type          Waiting   Counters  
  ------------------------------------------
  Deposit               4         2         
  Withdrawal            3         2         
  Account Services      4         1         
  Loan                  4         1         

  COUNTER STATUS
  ----------------------------------------
  [1] Counter 1 - Deposit            | BUSY - Serving TKN-0001
  [2] Counter 2 - Deposit            | BUSY - Serving TKN-0006
  [3] Counter 3 - Withdrawal         | FREE - Ready
  [4] Counter 4 - Withdrawal         | FREE - Ready
  [5] Counter 5 - Account Services   | FREE - Ready
  [6] Counter 6 - Loan               | FREE - Ready

  EFFICIENCY METRICS
  ----------------------------------------
  Completion Rate : 5.6%
  Cancellation Rate: 0.0%
  Counter Utilization: 33.3% (2/6 busy)
+==========================================================+
```

---

### 7. Key Learning Outcomes & Conclusion

Through the conception, design, implementation, and rigorous testing of this system:
1. **Object-Oriented Design Principles:** Learned practical encapsulation, separation of concerns, and clean interface design in C++.
2. **Applied Data Structures:** Demonstrated real-world utility of `std::deque` for priority queuing and `std::map` for relational token indexing.
3. **Data Serialization & File I/O:** Mastered structured stream parsing with `std::ifstream` and `std::ofstream` to implement persistent storage.
4. **Defensive Programming:** Implemented input stream validation, EOF detection, and edge-case testing to safeguard against crashes.
5. **Modern Development Workflow:** Utilized modular compilation, automated unit testing, script-based build pipelines, and documentation.

The Bank Token Management System fulfills all technical and functional specifications required for a robust computing aptitude and C++ programming project.
