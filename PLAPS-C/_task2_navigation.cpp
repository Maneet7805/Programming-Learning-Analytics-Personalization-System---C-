#include "task2_navigation.hpp"
#include "task3_logging.hpp"
#include <iostream>
#include <cctype>
using namespace std;

// ============================================================
//  TASK 2 IMPLEMENTATION
// ============================================================

// ---- Predefined activity bank ----
Activity activityBank[MAX_ACTIVITIES] = {
    {1,  "Variables & Data Types",    "Declare and use basic variables",          1, -1, false},
    {2,  "Control Flow (if/else)",    "Write conditional statements",             1, -1, false},
    {3,  "Loops (for/while)",         "Implement iterative solutions",            2, -1, false},
    {4,  "Functions & Scope",         "Define and call functions",                2, -1, false},
    {5,  "Arrays & Strings",          "Manipulate arrays and string operations",  3, -1, false},
    {6,  "Pointers & References",     "Understand memory addressing",             3, -1, false},
    {7,  "Structs & Classes",         "Design custom data types",                 3, -1, false},
    {8,  "Recursion",                 "Solve problems using recursive calls",     4, -1, false},
    {9,  "Linked Lists",              "Build and traverse linked structures",     4, -1, false},
    {10, "Sorting Algorithms",        "Implement bubble, selection, merge sort",  5, -1, false}
};
int activityBankSize = 10;

// ---- Task 2 per-learner progress tracking ----
LearnerProgress learnerProgress[MAX_LEARNERS];
int progressCount = 0;

// Find progress record for a specific learner
LearnerProgress* findLearnerProgress(int learnerId) {
    for (int i = 0; i < progressCount; i++) {
        if (learnerProgress[i].learnerId == learnerId)
            return &learnerProgress[i];
    }
    return NULL;
}

// Initialize progress for a new learner
void initializeLearnerProgress(int learnerId) {
    if (progressCount < MAX_LEARNERS) {
        learnerProgress[progressCount].learnerId = learnerId;
        learnerProgress[progressCount].currentActivityIdx = 0;  // Start at first activity
        progressCount++;
    }
}

ActivityStack::ActivityStack() : topPtr(NULL), stackSize(0) {}

ActivityStack::~ActivityStack() {
    while (!isEmpty()) pop();
}

bool ActivityStack::isEmpty() const { return topPtr == NULL; }
int ActivityStack::size() const { return stackSize; }

void ActivityStack::push(const Activity& a) {
    StackNode* newNode = new StackNode;
    newNode->data = a;
    newNode->next = topPtr;
    topPtr = newNode;
    stackSize++;
}

Activity ActivityStack::pop() {
    if (isEmpty()) {
        Activity empty = {0, "N/A", "", 0, -1, false};
        return empty;
    }
    StackNode* temp = topPtr;
    Activity   a    = temp->data;
    topPtr = topPtr->next;
    delete temp;
    stackSize--;
    return a;
}

Activity ActivityStack::peek() const {
    if (isEmpty()) {
        Activity empty = {0, "N/A", "", 0, -1, false};
        return empty;
    }
    return topPtr->data;
}

void ActivityStack::displayHistory() const {
    printBanner("ACTIVITY NAVIGATION HISTORY");
    if (isEmpty()) {
        cout << "  (No activities in history)" << endl;
        printDivider();
        return;
    }
    cout << left
         << setw(5)  << "Step"
         << setw(20) << "Topic"
         << setw(14) << "Difficulty"
         << setw(8)  << "Score"
         << setw(12) << "Status"
         << endl;
    printDivider();

    StackNode* curr = topPtr;
    int step = stackSize;
    while (curr) {
        cout << left
             << setw(5)  << step--
             << setw(20) << curr->data.topic
             << setw(14) << difficultyLabel(curr->data.difficulty)
             << setw(8)  << (curr->data.score == -1 ? "N/A" : to_string(curr->data.score))
             << setw(12) << (curr->data.isCompleted ? "Completed" : "In Progress")
             << endl;
        curr = curr->next;
    }
    printDivider();
    cout << "  Total activities in history: " << stackSize << endl;
    printDivider();
}

// ---- Task 2 menu handler ----
void task2Menu() {
    int choice;
    
    // First, get the learner ID
    int learnerId;
    Learner* learner = NULL;
    
    while (true) {
        learnerId = getValidatedInt("\n  Enter your Learner ID (1000-9999): ", MIN_ID, MAX_ID);
        learner = findLearnerById(learnerId);
        
        if (!learner) {
            cout << "  [ERROR] Learner ID not found. Please register first in Task 1.\n";
            char retry;
            cout << "  Do you want to try again? (y/n): ";
            cin >> retry;
            cin.ignore();
            if (tolower(retry) != 'y') {
                return;
            }
        } else {
            break;
        }
    }
    
    // Find or initialize learner progress
    LearnerProgress* progress = findLearnerProgress(learnerId);
    if (!progress) {
        initializeLearnerProgress(learnerId);
        progress = findLearnerProgress(learnerId);
        cout << "  [INFO] Welcome " << learner->name << "! Starting from Activity 1.\n";
    }
    
    do {
        printBanner("TASK 2: ACTIVITY NAVIGATION & SESSION FLOW");
        cout << "  Learner: " << learner->name << " (ID: " << learnerId << ")" << endl;
        cout << "  Current Activity Index : " << progress->currentActivityIdx + 1
             << " / " << activityBankSize << endl;
             
        if (progress->currentActivityIdx < activityBankSize) {
            cout << "  Current Topic          : "
                 << activityBank[progress->currentActivityIdx].topic << endl;
            cout << "  Difficulty             : "
                 << difficultyLabel(activityBank[progress->currentActivityIdx].difficulty) << endl;
        } else {
            cout << "  [INFO] All activities completed!\n";
        }
        printDivider();
        cout << "  1. View Current Activity" << endl;
        cout << "  2. Complete Activity (Enter Score & Move Forward)" << endl;
        cout << "  3. Go Back / Undo Last Activity" << endl;
        cout << "  4. View Navigation History (Stack)" << endl;
        cout << "  5. View All Available Activities" << endl;
        cout << "  0. Back to Main Menu" << endl;
        printDivider();
        
        choice = getValidatedInt("  Enter choice (0-5): ", 0, 5);

        switch (choice) {
            case 1: {
                printBanner("CURRENT ACTIVITY");
                if (progress->currentActivityIdx >= activityBankSize) {
                    cout << "  [INFO] You have completed all activities!\n";
                    break;
                }
                Activity& a = activityBank[progress->currentActivityIdx];
                printDivider();
                cout << "  Activity ID  : " << a.activityId   << endl;
                cout << "  Topic        : " << a.topic        << endl;
                cout << "  Description  : " << a.description  << endl;
                cout << "  Difficulty   : " << difficultyLabel(a.difficulty) << " (" << a.difficulty << "/5)" << endl;
                cout << "  Status       : " << (a.isCompleted ? "Completed" : "Pending") << endl;
                printDivider();
                break;
            }

            case 2: {
                printBanner("COMPLETE ACTIVITY");
                if (progress->currentActivityIdx >= activityBankSize) {
                    cout << "  [INFO] You have completed all activities!\n";
                    break;
                }

                Activity& a = activityBank[progress->currentActivityIdx];
                cout << "\n  Activity: " << a.topic << endl;
                cout << "  Description: " << a.description << endl;
                cout << "  Difficulty: " << difficultyLabel(a.difficulty) << endl;
                
                if (a.isCompleted && a.score < 50) {
                    cout << "  [NOTE] You are retrying this activity (previous score: " << a.score << ")\n";
                }
                
                printDivider();

                int score = getValidatedInt("  Enter score achieved (0 - 100): ", MIN_SCORE, MAX_SCORE);

                a.score = score;
                
                // Update learner statistics
                learner->totalAttempts++;
                if (score < 50) {
                    learner->failedAttempts++;
                }
                
                learner->averageScore =
                    ((learner->averageScore * (learner->totalAttempts - 1)) + score)
                    / learner->totalAttempts;

                logActivity(learnerId, learner->name, a.topic, a.difficulty, score);

                cout << "\n  [SUCCESS] Activity \"" << a.topic << "\" completed.\n";
                cout << "  Score: " << score << " | " << (score >= 50 ? "PASSED" : "FAILED") << endl;

                // ===== FIXED: Pass to progress, Fail to stay =====
                if (score >= 50) {  // PASSED
                    a.isCompleted = true;
                    progress->navigationStack.push(a);
                    progress->currentActivityIdx++;
                    
                    if (progress->currentActivityIdx < activityBankSize) {
                        cout << "  [PROGRESS] Moving to next activity: \""
                             << activityBank[progress->currentActivityIdx].topic << "\"\n";
                    } else {
                        cout << "  [CONGRATULATIONS] You have completed all activities!\n";
                    }
                } 
                else {  
                    a.isCompleted = false;  
                    cout << "  [NOTE] You failed this activity. You need to retry it.\n";
                    cout << "  [HINT] You can attempt this activity as many times as needed.\n";
                    
                    // Helpful hints based on score
                    if (score < 30) {
                        cout << "  [HINT] Review the basic concepts before attempting again.\n";
                    } else if (score < 40) {
                        cout << "  [HINT] You're making progress! Focus on the key points you missed.\n";
                    } else {
                        cout << "  [HINT] So close! Just a few more points needed. Review your mistakes.\n";
                    }
                }
                // ==================================================
                
                break;
            }

            case 3: {
                printBanner("GO BACK / UNDO LAST ACTIVITY");
                if (progress->navigationStack.isEmpty()) {
                    cout << "  [INFO] No passed activities in history to undo.\n";
                    cout << "  [NOTE] Failed activities are not recorded in history.\n";
                    break;
                }
                
                Activity undone = progress->navigationStack.peek();
                cout << "  Undo passed activity: \"" << undone.topic << "\"? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore();
                
                if (tolower(confirm) == 'y') {
                    progress->navigationStack.pop();
                    if (progress->currentActivityIdx > 0) {
                        progress->currentActivityIdx--;
                        activityBank[progress->currentActivityIdx].score = -1;
                        activityBank[progress->currentActivityIdx].isCompleted = false;
                    }
                    cout << "  [SUCCESS] Undone: \"" << undone.topic << "\"\n";
                    cout << "  You are now back at: \""
                         << activityBank[progress->currentActivityIdx].topic << "\"\n";
                } else {
                    cout << "  Undo cancelled.\n";
                }
                break;
            }

            case 4:
                progress->navigationStack.displayHistory();
                break;

            case 5: {
                printBanner("ALL AVAILABLE ACTIVITIES");
                cout << left
                     << setw(5)  << "ID"
                     << setw(28) << "Topic"
                     << setw(14) << "Difficulty"
                     << setw(12) << "Status"
                     << setw(8)  << "Score"
                     << endl;
                printDivider();
                for (int i = 0; i < activityBankSize; i++) {
                    cout << left
                         << setw(5)  << activityBank[i].activityId
                         << setw(28) << activityBank[i].topic
                         << setw(14) << difficultyLabel(activityBank[i].difficulty)
                         << setw(12) << (activityBank[i].isCompleted ? "Done" : "Pending")
                         << setw(8)  << (activityBank[i].score == -1
                                         ? "N/A"
                                         : to_string(activityBank[i].score))
                         << endl;
                }
                printDivider();
                break;
            }

            case 0:
                cout << "  Returning to Main Menu...\n";
                break;
        }
    } while (choice != 0);
}