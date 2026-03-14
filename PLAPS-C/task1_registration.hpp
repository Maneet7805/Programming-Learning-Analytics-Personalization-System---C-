#ifndef TASK1_REGISTRATION_HPP
#define TASK1_REGISTRATION_HPP

#include "constants.hpp"
#include "shared_models.hpp"
#include "global_registry.hpp"
#include "shared_utils.hpp"

// ============================================================
//  TASK 1 – LEARNER REGISTRATION & SESSION MANAGEMENT
//  Data Structure: Linked-List Queue (FIFO)
// ============================================================

// Node for the linked-list queue
struct QueueNode {
    Learner    data;
    QueueNode* next;
};

// Generic linked-list queue for Learner objects
class LearnerQueue {
private:
    QueueNode* frontPtr;   // Points to the front (dequeue end)
    QueueNode* rearPtr;    // Points to the rear  (enqueue end)
    int        queueSize;  // Current number of elements
    int        maxCapacity;// 0 = unlimited

public:
    // Constructor
    LearnerQueue(int capacity = 0);
    
    // Destructor
    ~LearnerQueue();

    // Returns true if the queue has no elements
    bool isEmpty() const;
    
    // Returns true if the queue has reached its capacity limit
    bool isFull() const;
    
    // Returns the current number of elements
    int size() const;
    
    // Adds a learner to the rear of the queue
    bool enqueue(const Learner& l);
    
    // Removes and returns the learner at the front
    Learner dequeue();
    
    // Returns the front learner without removing it
    Learner peek() const;
    
    // Checks whether a learner with the given ID is in the queue
    bool contains(int learnerId) const;
    
    // Displays all learners currently in the queue
    void display(const string& label) const;
};

// ---- Task 1 global queues ----
extern LearnerQueue registrationQueue;
extern LearnerQueue activeSessionQueue;

// ---- Task 1 menu handler ----
void task1Menu();

#endif