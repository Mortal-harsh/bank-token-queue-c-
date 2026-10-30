# 🏦 Bank Token Management System

> **Advanced Console-Based Queue Management for Banks — Built in C++**

A full-featured Bank Token Management System that simulates real-world bank counter operations with token generation, priority queues, multiple service counters, dashboard analytics, CSV reporting, and persistent data storage.

---

## 📋 Table of Contents

- [Features](#-features)
- [Architecture](#-architecture)
- [File Structure](#-file-structure)
- [How to Compile](#-how-to-compile)
- [How to Run](#-how-to-run)
- [Usage Guide](#-usage-guide)
- [Demo Mode](#-demo-mode)
- [Data Persistence](#-data-persistence)
- [Testing](#-testing)
- [Design Decisions](#-design-decisions)

---

## ✨ Features

| Feature | Description |
|---|---|
| **Unique Token Generation** | Every customer gets a unique token (e.g., `TKN-0001`) that never repeats across sessions |
| **FIFO Queues** | Fair first-in-first-out queuing per service type |
| **Priority Handling** | Senior citizens and specially-abled customers are served first |
| **Multiple Service Counters** | 4 service types: Deposits, Withdrawals, Account Services, Loans |
| **Multi-Counter Serving** | Each service type has configurable number of counters |
| **Dashboard Statistics** | Real-time stats: tokens issued, served, waiting, average wait time |
| **Search** | Search tokens by ID, customer name, phone, or status |
| **CSV Reports** | Export all token data, served data, and statistics to CSV files |
| **Local Persistence** | Data auto-saves to disk; survives program restart |
| **Demo Mode** | Auto-populates the system with sample data for demonstration |
| **Testing Suite** | Built-in automated tests that verify core functionality |
| **Rich Console UI** | Color-coded menus, tables, box-drawn borders, clear layout |

---

## 🏗️ Architecture

```
+-----------------------------------------------------------+
|                      main.cpp                             |
|                (Entry point & Menu Loop)                   |
+-----------------------------------------------------------+
|    Token       |   Queue        |   Counter                |
|    Manager     |   Manager      |   Manager                |
|  (token.h/cpp) | (queue.h/cpp)  | (counter.h/cpp)          |
+-----------------------------------------------------------+
|  Persistence   |  CSV Reports   |  Dashboard               |
|  (persist.h)   |  (report.h)    |  (dashboard.h/cpp)       |
+-----------------------------------------------------------+
|           utils.h  (Colors, Helpers, Constants)            |
+-----------------------------------------------------------+
```

### Core Classes

- **`Token`** — Represents a single customer token with ID, name, phone, service type, priority, timestamps, and status.
- **`ServiceQueue`** — A FIFO queue for one service type. Supports priority insertion.
- **`Counter`** — Represents a physical bank counter assigned to a service type.
- **`BankSystem`** — The orchestrator class managing all tokens, queues, counters, persistence, and search.
- **`Dashboard`** — Computes and displays real-time statistics.
- **`ReportGenerator`** — Exports data to CSV files.

---

## 📁 File Structure

```
mini project on C++/
├── README.md                  # This file
├── PROJECT_REPORT.md          # Detailed academic project report
├── src/
│   ├── main.cpp               # Entry point, main menu loop
│   ├── token.h                # Token class definition
│   ├── token.cpp              # Token class implementation
│   ├── queue.h                # ServiceQueue class (FIFO + priority)
│   ├── queue.cpp              # ServiceQueue implementation
│   ├── counter.h              # Counter class definition
│   ├── counter.cpp            # Counter implementation
│   ├── bank_system.h          # BankSystem orchestrator
│   ├── bank_system.cpp        # BankSystem implementation
│   ├── dashboard.h            # Dashboard statistics
│   ├── dashboard.cpp          # Dashboard implementation
│   ├── report.h               # CSV report generator
│   ├── report.cpp             # Report implementation
│   ├── persist.h              # File I/O for persistence
│   ├── persist.cpp            # Persistence implementation
│   ├── utils.h                # Utility functions, constants, colors
│   └── demo.h                 # Demo mode data generator
├── tests/
│   └── test_main.cpp          # Automated test suite
├── data/                      # Auto-created: persistent data files
├── reports/                   # Auto-created: exported CSV reports
├── build.bat                  # One-click compile script
└── run_tests.bat              # One-click test runner
```

---

## 🔨 How to Compile

### Using the build script (Recommended)

```
build.bat
```

### Manual compilation

```
C:\MinGW\bin\g++.exe -std=c++11 -o BankTokenSystem.exe src/main.cpp src/token.cpp src/queue.cpp src/counter.cpp src/bank_system.cpp src/dashboard.cpp src/report.cpp src/persist.cpp -static
```

---

## 🚀 How to Run

```
BankTokenSystem.exe
```

Or with demo mode (pre-loads sample data):

```
BankTokenSystem.exe --demo
```

---

## 📖 Usage Guide

### Main Menu Options

| Key | Action |
|-----|--------|
| 1 | Issue New Token |
| 2 | Call Next Token (Serve) |
| 3 | Complete Current Service |
| 4 | View All Queues |
| 5 | View Counter Status |
| 6 | Dashboard & Statistics |
| 7 | Search Token |
| 8 | Export CSV Report |
| 9 | Load Demo Data |
| 0 | Save & Exit |

### Issuing a Token
1. Select option `1`
2. Enter customer name and phone number
3. Choose service type (Deposit, Withdrawal, Account, Loan)
4. Indicate if the customer is a priority customer (senior/specially-abled)
5. The system generates a unique token and adds the customer to the appropriate queue

### Serving Customers
1. Select option `2` to call the next token at a specific counter
2. The system picks the next token from that counter's queue (priority first)
3. Select option `3` when service is complete to free the counter

---

## 🎭 Demo Mode

Run with `--demo` flag or select option 9 from the menu to populate the system with 15 sample customers across all service types with a mix of normal and priority tokens.

---

## 💾 Data Persistence

- All data is auto-saved to `data/` on exit
- On startup, the system automatically loads previous session data
- File format is CSV for easy human readability

---

## 🧪 Testing

### Run tests
```
run_tests.bat
```

### What is tested

| Test | Description |
|---|---|
| Token Creation | Verifies unique IDs and correct field storage |
| Queue FIFO | Ensures first-in-first-out order |
| Priority Queue | Verifies priority tokens jump ahead |
| Counter Assignment | Tests counter-service mapping |
| Serve & Complete | End-to-end serve flow |
| Search | Tests search by ID, name, phone |
| Persistence | Save/reload data, verify integrity |
| Statistics | Correct counting of issued, waiting, served tokens |

---

## 🎨 Design Decisions

1. **Console UI**: MinGW 6.3.0 available; no Qt/wxWidgets. Console UI with colors and box-drawing compiles anywhere.
2. **Header + Source separation**: Clean interface/implementation split — standard C++ best practice.
3. **Priority via front-insertion**: Priority tokens inserted at queue front. Simple, sufficient for bank scale.
4. **CSV persistence**: Human-readable, Excel-compatible, no external dependencies.
5. **Unique token IDs**: Running counter persisted to disk; tokens never repeat across restarts.

---

*Built with C++ for Computing Aptitude Mini Project*
---

## 👥 Authors & Contributors

- **Degree:** Master of Computer Applications (MCA)
- **Prince Raj** ([@PrinceRaj-45](https://github.com/PrinceRaj-45)) - *Developer & Contributor*
- **Harsh Raj** ([@Mortal-harsh](https://github.com/Mortal-harsh)) - *Lead Developer*
