# PLAPS-CPP 
## Programming Learning Analytics & Personalization System

![C++](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Data Structures](https://img.shields.io/badge/data%20structures-Queue%20%7C%20Stack%20%7C%20Circular%20Queue%20%7C%20Max--Heap-green)
![APU](https://img.shields.io/badge/university-Asia%20Pacific%20University-orange)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## Overview

**PLAPS** is a comprehensive console-based prototype system developed for **Asia Pacific University (APU)** that simulates how students interact with programming activities and records their learning behaviour. The system helps track student progress, manage learning sessions, log recent activities, and identify learners who may need extra support.

### Key Features
- **Learner Registration & Session Management** - Queue-based waiting list and active session handling
- **Activity Navigation & Session Flow** - Stack-based navigation history with undo functionality
- **Recent Activity Logging** - Circular buffer for maintaining recent activity logs
- **At-Risk Learner Priority Engine** - Max-heap priority queue for identifying struggling students

---

## Data Structures Used

| Task | Data Structure | Implementation | Purpose |
|------|---------------|----------------|---------|
| **Task 1** | Queue (FIFO) | Linked-List | Learner registration and session management |
| **Task 2** | Stack (LIFO) | Linked-List | Activity navigation history with undo |
| **Task 3** | Circular Queue | Array-based | Fixed-size recent activity log |
| **Task 4** | Priority Queue | Array-based Max-Heap | At-risk learner prioritization |

> ⚠️ **Note**: No STL containers (`<list>`, `<vector>`, `<queue>`, `<stack>`, `<priority_queue>`) are used anywhere in the code. All data structures are manually implemented.

---

## Getting Started

### Prerequisites
- C++ compiler with C++11 support (g++, clang++, etc.)
- Basic command line knowledge

### Installation

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/PLAPS-CPP.git
cd PLAPS-CPP
```

### Compile (C++11 required)
g++ -std=c++11 -o plaps *.cpp

### Run
./plaps
