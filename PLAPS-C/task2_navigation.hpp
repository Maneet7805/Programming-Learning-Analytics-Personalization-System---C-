#ifndef TASK2_NAVIGATION_HPP
#define TASK2_NAVIGATION_HPP

#include "constants.hpp"
#include "shared_models.hpp"
#include "global_registry.hpp"
#include "shared_utils.hpp"

// ============================================================
//  TASK 2 – ACTIVITY NAVIGATION & SESSION FLOW
//  Data Structure: Linked-List Stack (LIFO)
// ============================================================

// Node for the linked-list stack
struct StackNode {
    Activity   data;
    StackNode* next;
};

// Linked-list stack for Activity objects
class ActivityStack {
private:
    StackNode* topPtr;
    int        stackSize;

public:
    ActivityStack();
    ~ActivityStack();

    bool isEmpty() const;
    int  size() const;
    void push(const Activity& a);
    Activity pop();
    Activity peek() const;
    void displayHistory() const;
};

// Structure to track each learner's progress
struct LearnerProgress {
    int learnerId;
    int currentActivityIdx;
    ActivityStack navigationStack;
};

// ---- Predefined activity bank ----
extern Activity activityBank[MAX_ACTIVITIES];
extern int activityBankSize;

// ---- Task 2 global state ----
extern LearnerProgress learnerProgress[MAX_LEARNERS];
extern int progressCount;

// Helper functions
LearnerProgress* findLearnerProgress(int learnerId);
void initializeLearnerProgress(int learnerId);

// Forward declaration (Task 3 log function)
void logActivity(int learnerId, const string& learnerName,
                 const string& topic, int difficulty, int score);

// ---- Task 2 menu handler ----
void task2Menu();

#endif