#ifndef TASK4_RISK_HPP
#define TASK4_RISK_HPP

#include "constants.hpp"
#include "shared_models.hpp"
#include "global_registry.hpp"
#include "shared_utils.hpp"

// ============================================================
//  TASK 4 – AT-RISK LEARNER PRIORITY & RECOMMENDATION ENGINE
//  Data Structure: Array-Based Max-Heap (Priority Queue)
// ============================================================

class RiskPriorityQueue {
private:
    AtRiskEntry heap[MAX_HEAP_SIZE];
    int         heapSize;

    // Returns the index of the parent node
    int parent(int i);
    // Returns the index of the left child
    int leftChild(int i);
    // Returns the index of the right child
    int rightChild(int i);

    // Swaps two heap entries
    void swapEntries(int i, int j);

    // Restores heap property upward after insertion
    void heapifyUp(int i);
    
    // Restores heap property downward after extraction
    void heapifyDown(int i);

public:
    RiskPriorityQueue();

    bool isEmpty() const;
    int  size()    const;

    // Insert a new at-risk entry into the heap
    void insert(const AtRiskEntry& entry);
    
    // Returns the highest-risk entry without removing it
    AtRiskEntry peekMax() const;
    
    // Removes and returns the highest-risk entry
    AtRiskEntry extractMax();
    
    // Checks whether a learner ID already exists in the heap
    bool contains(int learnerId) const;
    
    // Display all at-risk learners (heap order = highest risk first)
    void displayAll() const;
    
    // Export at-risk list to CSV
    void exportCSV(const string& filename = "at_risk_learners.csv") const;
};

// ---- Task 4 global priority queue ----
extern RiskPriorityQueue riskQueue;

// Calculates a risk score (0–100) for a given learner
float calculateRiskScore(const Learner& l, float avgDifficulty = 2.5f);

// Converts a numeric risk score to a label
string riskLabel(float score);

// Generates a recommendation string based on learner performance
string generateRecommendation(const Learner& l, float riskScore);

// ---- Task 4 menu handler ----
void task4Menu();

#endif