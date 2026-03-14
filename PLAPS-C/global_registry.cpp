#include "global_registry.hpp"
#include "shared_utils.hpp"
#include "task1_registration.hpp"
#include "task3_logging.hpp"
#include <iostream>
using namespace std;

// ============================================================
//  GLOBAL LEARNER REGISTRY IMPLEMENTATION
// ============================================================
Learner globalRegistry[MAX_LEARNERS];
int registryCount = 0;

// Finds a learner in the global registry by ID.
Learner* findLearnerById(int id) {
    for (int i = 0; i < registryCount; i++) {
        if (globalRegistry[i].id == id)
            return &globalRegistry[i];
    }
    return NULL;
}

// ============================================================
//  DUMMY DATA INITIALIZATION
// ============================================================
void initializeDummyData() {
    cout << "  [INFO] Initializing 10 dummy learners with 15 sample activity logs...\n";
    
    // ===== LEARNER 1001 - Alice Johnson (Beginner, Good) =====
    globalRegistry[registryCount] = {
        1001, "Alice Johnson", "alice.j@email.com", "Beginner", 
        85.5f, 8, 1, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1001, "Alice Johnson", "Variables & Data Types", 1, 85);
    logActivity(1001, "Alice Johnson", "Control Flow (if/else)", 1, 92);
    registryCount++;
    
    // ===== LEARNER 1002 - Bob Smith (Beginner, Struggling) =====
    globalRegistry[registryCount] = {
        1002, "Bob Smith", "bob.smith@email.com", "Beginner", 
        45.0f, 10, 7, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1002, "Bob Smith", "Variables & Data Types", 1, 35);
    logActivity(1002, "Bob Smith", "Variables & Data Types", 1, 42);
    registryCount++;
    
    // ===== LEARNER 1003 - Carol Davis (Intermediate, Excellent) =====
    globalRegistry[registryCount] = {
        1003, "Carol Davis", "carol.d@email.com", "Intermediate", 
        92.0f, 15, 0, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1003, "Carol Davis", "Variables & Data Types", 1, 98);
    logActivity(1003, "Carol Davis", "Control Flow (if/else)", 1, 95);
    registryCount++;
    
    // ===== LEARNER 1004 - David Wilson (Intermediate, Average) =====
    globalRegistry[registryCount] = {
        1004, "David Wilson", "david.w@email.com", "Intermediate", 
        68.5f, 12, 4, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1004, "David Wilson", "Variables & Data Types", 1, 75);
    logActivity(1004, "David Wilson", "Control Flow (if/else)", 1, 82);
    registryCount++;
    
    // ===== LEARNER 1005 - Eva Brown (Advanced, Good) =====
    globalRegistry[registryCount] = {
        1005, "Eva Brown", "eva.b@email.com", "Advanced", 
        88.0f, 20, 2, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1005, "Eva Brown", "Variables & Data Types", 1, 95);
    logActivity(1005, "Eva Brown", "Control Flow (if/else)", 1, 93);
    registryCount++;
    
    // ===== LEARNER 1006 - Frank Miller (Beginner, New) =====
    globalRegistry[registryCount] = {
        1006, "Frank Miller", "frank.m@email.com", "Beginner", 
        0.0f, 0, 0, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    registryCount++;
    
    // ===== LEARNER 1007 - Grace Lee (Intermediate, At Risk) =====
    globalRegistry[registryCount] = {
        1007, "Grace Lee", "grace.l@email.com", "Intermediate", 
        35.5f, 8, 6, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1007, "Grace Lee", "Variables & Data Types", 1, 30);
    logActivity(1007, "Grace Lee", "Control Flow (if/else)", 1, 32);
    registryCount++;
    
    // ===== LEARNER 1008 - Henry Taylor (Advanced, Struggling) =====
    globalRegistry[registryCount] = {
        1008, "Henry Taylor", "henry.t@email.com", "Advanced", 
        52.0f, 18, 9, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1008, "Henry Taylor", "Pointers & References", 3, 45);
    logActivity(1008, "Henry Taylor", "Recursion", 4, 40);
    registryCount++;
    
    // ===== LEARNER 1009 - Ivy Chen (Beginner, Making progress) =====
    globalRegistry[registryCount] = {
        1009, "Ivy Chen", "ivy.c@email.com", "Beginner", 
        72.5f, 6, 1, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1009, "Ivy Chen", "Variables & Data Types", 1, 65);
    logActivity(1009, "Ivy Chen", "Loops (for/while)", 2, 75);
    registryCount++;
    
    // ===== LEARNER 1010 - Jack Wilson (Intermediate, Consistent) =====
    globalRegistry[registryCount] = {
        1010, "Jack Wilson", "jack.w@email.com", "Intermediate", 
        78.0f, 14, 3, true
    };
    activeSessionQueue.enqueue(globalRegistry[registryCount]);
    logActivity(1010, "Jack Wilson", "Functions & Scope", 2, 76);
    logActivity(1010, "Jack Wilson", "Arrays & Strings", 3, 81);
    registryCount++;
    
    cout << "  [SUCCESS] " << registryCount << " dummy learners initialized!\n";
    cout << "  [INFO] ALL 10 learners are now ACTIVE in session!\n";
    cout << "  [INFO] Added 15 sample activity logs (within " << LOG_CAPACITY << " capacity)!\n";
    cout << "  [INFO] Active session: " << activeSessionQueue.size() << "/" << MAX_SESSION_CAPACITY << endl;
    cout << "  [INFO] You can now use IDs 1001-1010 immediately in all tasks!\n";
}