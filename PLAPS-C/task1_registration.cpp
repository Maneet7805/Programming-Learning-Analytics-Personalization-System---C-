#include "task1_registration.hpp"
#include <iostream>
#include <cctype>
using namespace std;

// ============================================================
//  TASK 1 IMPLEMENTATION
// ============================================================

// ---- Task 1 global queues ----
LearnerQueue registrationQueue;
LearnerQueue activeSessionQueue(MAX_SESSION_CAPACITY);

// Constructor
LearnerQueue::LearnerQueue(int capacity)
    : frontPtr(NULL), rearPtr(NULL), queueSize(0), maxCapacity(capacity) {}

// Destructor – free all nodes
LearnerQueue::~LearnerQueue() {
    while (!isEmpty()) dequeue();
}

// Returns true if the queue has no elements
bool LearnerQueue::isEmpty() const { 
    return frontPtr == NULL; 
}

// Returns true if the queue has reached its capacity limit
bool LearnerQueue::isFull() const {
    return (maxCapacity > 0 && queueSize >= maxCapacity);
}

// Returns the current number of elements
int LearnerQueue::size() const { 
    return queueSize; 
}

// Adds a learner to the rear of the queue
bool LearnerQueue::enqueue(const Learner& l) {
    if (isFull()) return false;

    QueueNode* newNode = new QueueNode;
    newNode->data = l;
    newNode->next = NULL;

    if (rearPtr == NULL) {
        frontPtr = rearPtr = newNode;
    } else {
        rearPtr->next = newNode;
        rearPtr = newNode;
    }
    queueSize++;
    return true;
}

// Removes and returns the learner at the front
Learner LearnerQueue::dequeue() {
    if (isEmpty()) {
        Learner empty = {0, "N/A", "", "", 0.0f, 0, 0, false};
        return empty;
    }
    QueueNode* temp = frontPtr;
    Learner    l    = temp->data;
    frontPtr = frontPtr->next;
    if (frontPtr == NULL) rearPtr = NULL;
    delete temp;
    queueSize--;
    return l;
}

// Returns the front learner without removing it
Learner LearnerQueue::peek() const {
    if (isEmpty()) {
        Learner empty = {0, "N/A", "", "", 0.0f, 0, 0, false};
        return empty;
    }
    return frontPtr->data;
}

// Checks whether a learner with the given ID is in the queue
bool LearnerQueue::contains(int learnerId) const {
    QueueNode* curr = frontPtr;
    while (curr) {
        if (curr->data.id == learnerId) return true;
        curr = curr->next;
    }
    return false;
}

// Displays all learners currently in the queue
void LearnerQueue::display(const string& label) const {
    printBanner(label);
    if (isEmpty()) {
        cout << "  (Queue is empty)" << endl;
        printDivider();
        return;
    }
    cout << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(25) << "Email"
         << setw(14) << "Level"
         << setw(8)  << "Avg Score"
         << endl;
    printDivider();
    QueueNode* curr = frontPtr;
    int pos = 1;
    while (curr) {
        cout << left
             << setw(6)  << curr->data.id
             << setw(20) << curr->data.name
             << setw(25) << curr->data.email
             << setw(14) << curr->data.level
             << setw(8)  << fixed << setprecision(1) << curr->data.averageScore
             << "  [Position " << pos++ << "]"
             << endl;
        curr = curr->next;
    }
    printDivider();
    cout << "  Total in queue: " << queueSize;
    if (maxCapacity > 0)
        cout << " / " << maxCapacity;
    cout << endl;
    printDivider();
}

// ---- Task 1 menu handler ----
void task1Menu() {
    int choice;
    do {
        printBanner("TASK 1: REGISTRATION & SESSION MANAGEMENT");
        cout << "  1. Register New Learner" << endl;
        cout << "  2. Move Next Learner into Active Session" << endl;
        cout << "  3. Remove Learner from Active Session (Exit)" << endl;
        cout << "  4. View Registration Queue" << endl;
        cout << "  5. View Active Session List" << endl;
        cout << "  6. Search Learner by ID" << endl;
        cout << "  0. Back to Main Menu" << endl;
        printDivider();
        
        choice = getValidatedInt("  Enter choice (0-6): ", 0, 6);

        switch (choice) {
            case 1: {
                printBanner("REGISTER NEW LEARNER");
                Learner l;

                // Validate Learner ID
                while (true) {
                    l.id = getValidatedInt("  Enter Learner ID (1000-9999): ", MIN_ID, MAX_ID);
                    
                    if (findLearnerById(l.id) != NULL ||
                        registrationQueue.contains(l.id) ||
                        activeSessionQueue.contains(l.id)) {
                        cout << "  [ERROR] Learner ID " << l.id << " already exists. Please choose another ID.\n";
                    } else {
                        break;
                    }
                }

                // Validate Name
                l.name = getValidatedString("  Enter Full Name: ", MAX_NAME_LENGTH, false);

                // Validate Email
                l.email = getValidatedEmail("  Enter Email: ");

                // Validate Level
                int lvl = getValidatedInt("  Select Level (1=Beginner, 2=Intermediate, 3=Advanced): ", 1, 3);
                l.level = (lvl == 2) ? "Intermediate" : (lvl == 3) ? "Advanced" : "Beginner";

                l.averageScore  = 0.0f;
                l.totalAttempts = 0;
                l.failedAttempts= 0;
                l.isActive      = false;

                if (registryCount < MAX_LEARNERS) {
                    globalRegistry[registryCount++] = l;
                }

                registrationQueue.enqueue(l);
                cout << "\n  [SUCCESS] Learner \"" << l.name
                     << "\" registered and added to the queue.\n";
                break;
            }

            case 2: {
                printBanner("MOVE LEARNER TO ACTIVE SESSION");
                if (registrationQueue.isEmpty()) {
                    cout << "  [INFO] Registration queue is empty.\n";
                    break;
                }
                if (activeSessionQueue.isFull()) {
                    cout << "  [WARNING] Active session is full ("
                         << MAX_SESSION_CAPACITY << " learners).\n";
                    break;
                }
                
                // Show who will be moved
                Learner nextLearner = registrationQueue.peek();
                cout << "  Next learner in queue: " << nextLearner.name << " (ID: " << nextLearner.id << ")\n";
                
                char confirm;
                cout << "  Move this learner to active session? (y/n): ";
                cin >> confirm;
                cin.ignore();
                
                if (tolower(confirm) == 'y') {
                    Learner l = registrationQueue.dequeue();
                    l.isActive = true;

                    Learner* reg = findLearnerById(l.id);
                    if (reg) reg->isActive = true;

                    activeSessionQueue.enqueue(l);
                    cout << "  [SUCCESS] Learner \"" << l.name
                         << "\" (ID: " << l.id << ") moved to Active Session.\n";
                } else {
                    cout << "  Operation cancelled.\n";
                }
                break;
            }

            case 3: {
                printBanner("REMOVE LEARNER FROM SESSION");
                if (activeSessionQueue.isEmpty()) {
                    cout << "  [INFO] Active session is empty.\n";
                    break;
                }
                
                // Show current active session
                activeSessionQueue.display("CURRENT ACTIVE SESSION");
                
                int learnerId = getValidatedInt("  Enter Learner ID to remove: ", MIN_ID, MAX_ID);
                
                // Check if learner is in active session
                if (!activeSessionQueue.contains(learnerId)) {
                    cout << "  [ERROR] Learner ID " << learnerId << " is not in the active session.\n";
                    break;
                }
                
                char confirm;
                cout << "  Remove this learner from session? (y/n): ";
                cin >> confirm;
                cin.ignore();
                
                if (tolower(confirm) == 'y') {
                    Learner l = activeSessionQueue.dequeue();
                    l.isActive = false;

                    Learner* reg = findLearnerById(l.id);
                    if (reg) reg->isActive = false;

                    cout << "  [SUCCESS] Learner \"" << l.name
                         << "\" (ID: " << l.id << ") has exited the session.\n";
                }
                break;
            }

            case 4:
                registrationQueue.display("REGISTRATION QUEUE");
                break;

            case 5:
                activeSessionQueue.display("ACTIVE SESSION LIST");
                break;

            case 6: {
                printBanner("SEARCH LEARNER BY ID");
                int searchId = getValidatedInt("  Enter Learner ID to search (1000-9999): ", MIN_ID, MAX_ID);

                Learner* found = findLearnerById(searchId);
                if (found) {
                    printDivider();
                    cout << "  ID           : " << found->id           << endl;
                    cout << "  Name         : " << found->name         << endl;
                    cout << "  Email        : " << found->email        << endl;
                    cout << "  Level        : " << found->level        << endl;
                    cout << "  Avg Score    : " << fixed << setprecision(2)
                                                << found->averageScore << endl;
                    cout << "  Total Attempts : " << found->totalAttempts  << endl;
                    cout << "  Failed Attempts: " << found->failedAttempts << endl;
                    cout << "  Session Status : " << (found->isActive ? "Active" : "Inactive") << endl;
                    
                    // Check if in queues
                    if (registrationQueue.contains(searchId)) {
                        cout << "  Queue Status  : In Registration Queue\n";
                    } else if (activeSessionQueue.contains(searchId)) {
                        cout << "  Queue Status  : In Active Session\n";
                    }
                    printDivider();
                } else {
                    cout << "  [INFO] Learner ID " << searchId << " not found.\n";
                }
                break;
            }

            case 0:
                cout << "  Returning to Main Menu...\n";
                break;
        }
    } while (choice != 0);
}