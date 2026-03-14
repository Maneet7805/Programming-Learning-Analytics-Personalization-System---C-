#include "task3_logging.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
using namespace std;

// ============================================================
//  TASK 3 IMPLEMENTATION
// ============================================================

// ---- Task 3 global circular log ----
CircularActivityLog globalLog(LOG_CAPACITY);

// Constructor – allocate the ring buffer
CircularActivityLog::CircularActivityLog(int size)
    : capacity(size), frontIdx(0), rearIdx(-1), count(0) {
    buffer = new ActivityLog[capacity];
}

// Destructor
CircularActivityLog::~CircularActivityLog() { 
    delete[] buffer; 
}

bool CircularActivityLog::isEmpty() const { 
    return count == 0; 
}

bool CircularActivityLog::isFull()  const { 
    return count == capacity; 
}

int  CircularActivityLog::size()    const { 
    return count; 
}

int  CircularActivityLog::getCapacity() const { 
    return capacity; 
}

// Add a new log entry; overwrites the oldest if full
void CircularActivityLog::addLog(int learnerId, const string& learnerName,
                const string& topic, int difficulty, int score) {
    rearIdx = (rearIdx + 1) % capacity;
    buffer[rearIdx] = {learnerId, learnerName, topic,
                       difficulty, score, (score >= 50),
                       getCurrentTimestamp()};
    if (isFull()) {
        frontIdx = (frontIdx + 1) % capacity;
    } else {
        count++;
    }
}

// Display all log entries (oldest → newest)
void CircularActivityLog::displayAll() const {
    printBanner("RECENT ACTIVITY LOG (Circular Buffer)");
    if (isEmpty()) {
        cout << "  (Log is empty)" << endl;
        printDivider();
        return;
    }
    cout << left
         << setw(6)  << "LID"
         << setw(18) << "Learner"
         << setw(22) << "Topic"
         << setw(12) << "Difficulty"
         << setw(7)  << "Score"
         << setw(8)  << "Result"
         << "Timestamp"
         << endl;
    printDivider();
    for (int i = 0; i < count; i++) {
        int idx = (frontIdx + i) % capacity;
        cout << left
             << setw(6)  << buffer[idx].learnerId
             << setw(18) << buffer[idx].learnerName
             << setw(22) << buffer[idx].topic
             << setw(12) << difficultyLabel(buffer[idx].difficulty)
             << setw(7)  << buffer[idx].score
             << setw(8)  << (buffer[idx].passed ? "PASS" : "FAIL")
             << buffer[idx].timestamp
             << endl;
    }
    printDivider();
    cout << "  Entries: " << count << " / " << capacity
         << "  (Buffer " << (isFull() ? "FULL – overwriting oldest" : "has space")
         << ")" << endl;
    printDivider();
}

// Display logs filtered by a specific learner ID
void CircularActivityLog::displayByLearner(int learnerId) const {
    printBanner("LOGS FOR LEARNER ID: " + to_string(learnerId));
    bool found = false;
    cout << left
         << setw(22) << "Topic"
         << setw(12) << "Difficulty"
         << setw(7)  << "Score"
         << setw(8)  << "Result"
         << "Timestamp"
         << endl;
    printDivider();
    
    int total = 0, passed = 0, scoreSum = 0;
    
    for (int i = 0; i < count; i++) {
        int idx = (frontIdx + i) % capacity;
        if (buffer[idx].learnerId == learnerId) {
            cout << left
                 << setw(22) << buffer[idx].topic
                 << setw(12) << difficultyLabel(buffer[idx].difficulty)
                 << setw(7)  << buffer[idx].score
                 << setw(8)  << (buffer[idx].passed ? "PASS" : "FAIL")
                 << buffer[idx].timestamp
                 << endl;
            found = true;
            total++;
            if (buffer[idx].passed) passed++;
            scoreSum += buffer[idx].score;
        }
    }
    
    if (!found) {
        cout << "  No logs found for Learner ID " << learnerId << endl;
    } else {
        printDivider();
        cout << "  Summary for Learner ID " << learnerId << ":\n";
        cout << "  Total Activities: " << total << endl;
        cout << "  Passed: " << passed << " | Failed: " << (total - passed) << endl;
        if (total > 0) {
            cout << "  Average Score: " << fixed << setprecision(2) 
                 << (float)scoreSum / total << endl;
        }
    }
    printDivider();
}

// Export all logs to a CSV file
void CircularActivityLog::exportCSV(const string& filename) const {
    // Check if filename is valid
    if (filename.empty()) {
        cout << "  [ERROR] Invalid filename.\n";
        return;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "  [ERROR] Could not open file for export. Check permissions and path.\n";
        return;
    }
    
    // CSV header
    file << "LearnerID,LearnerName,Topic,Difficulty,Score,Result,Timestamp\n";
    
    int exported = 0;
    for (int i = 0; i < count; i++) {
        int idx = (frontIdx + i) % capacity;
        file << buffer[idx].learnerId    << ","
             << "\"" << buffer[idx].learnerName << "\","  // Quotes to handle commas in name
             << "\"" << buffer[idx].topic << "\","        // Quotes to handle commas in topic
             << buffer[idx].difficulty   << ","
             << buffer[idx].score        << ","
             << (buffer[idx].passed ? "PASS" : "FAIL") << ","
             << buffer[idx].timestamp    << "\n";
        exported++;
    }
    
    file.close();
    cout << "  [SUCCESS] " << exported << " log entries exported to \"" << filename << "\"\n";
}

// Export logs for a specific learner to a text file
void CircularActivityLog::exportLearnerReport(int learnerId, const string& filename) const {
    // Check if filename is valid
    if (filename.empty()) {
        cout << "  [ERROR] Invalid filename.\n";
        return;
    }
    
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "  [ERROR] Could not open file.\n";
        return;
    }
    
    file << "========================================\n";
    file << "ACTIVITY REPORT FOR LEARNER ID: " << learnerId << "\n";
    file << "========================================\n";
    file << "Generated: " << getCurrentTimestamp() << "\n";
    file << string(70, '=') << "\n\n";
    
    file << left
         << setw(25) << "Topic"
         << setw(15) << "Difficulty"
         << setw(10) << "Score"
         << setw(10) << "Result"
         << "Timestamp\n";
    file << string(70, '-') << "\n";

    int total = 0, passed = 0, scoreSum = 0;
    for (int i = 0; i < count; i++) {
        int idx = (frontIdx + i) % capacity;
        if (buffer[idx].learnerId == learnerId) {
            file << left
                 << setw(25) << buffer[idx].topic
                 << setw(15) << difficultyLabel(buffer[idx].difficulty)
                 << setw(10) << buffer[idx].score
                 << setw(10) << (buffer[idx].passed ? "PASS" : "FAIL")
                 << buffer[idx].timestamp << "\n";
            total++;
            if (buffer[idx].passed) passed++;
            scoreSum += buffer[idx].score;
        }
    }
    
    file << string(70, '-') << "\n\n";
    file << "SUMMARY STATISTICS:\n";
    file << string(30, '-') << "\n";
    
    if (total > 0) {
        file << "Total Attempts    : " << total  << "\n";
        file << "Activities Passed : " << passed << " (" 
             << fixed << setprecision(1) << (passed * 100.0 / total) << "%)\n";
        file << "Activities Failed : " << (total - passed) << " ("
             << fixed << setprecision(1) << ((total - passed) * 100.0 / total) << "%)\n";
        file << "Average Score     : " << fixed << setprecision(2)
             << (float)scoreSum / total << "\n";
    } else {
        file << "No records found for this learner.\n";
    }
    
    file << "\n========================================\n";
    file.close();
    cout << "  [SUCCESS] Report exported to \"" << filename << "\"\n";
}

// Log function called by Task 2
void logActivity(int learnerId, const string& learnerName,
                 const string& topic, int difficulty, int score) {
    globalLog.addLog(learnerId, learnerName, topic, difficulty, score);
}

// ---- Task 3 menu handler ----
void task3Menu() {
    int choice;
    do {
        printBanner("TASK 3: RECENT ACTIVITY LOGGING");
        cout << "  Buffer: " << globalLog.size() << " / "
             << globalLog.getCapacity() << " entries" << endl;
        printDivider();
        cout << "  1. Add Manual Activity Log Entry" << endl;
        cout << "  2. View All Activity Logs" << endl;
        cout << "  3. Filter Logs by Learner ID" << endl;
        cout << "  4. Export All Logs to CSV" << endl;
        cout << "  5. Export Learner Report to Text File" << endl;
        cout << "  0. Back to Main Menu" << endl;
        printDivider();
        
        choice = getValidatedInt("  Enter choice (0-5): ", 0, 5);

        switch (choice) {
            case 1: {
                printBanner("ADD MANUAL LOG ENTRY");
                
                // Validate Learner ID
                int learnerId;
                Learner* learner = NULL;
                
                while (true) {
                    learnerId = getValidatedInt("  Enter Learner ID (1000-9999): ", MIN_ID, MAX_ID);
                    learner = findLearnerById(learnerId);
                    
                    if (!learner) {
                        cout << "  [WARNING] Learner ID not found in registry. Log will be saved with 'Unknown' name.\n";
                        char choice;
                        cout << "  Continue anyway? (y/n): ";
                        cin >> choice;
                        cin.ignore();
                        if (tolower(choice) != 'y') {
                            break;
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                
                string learnerName = (learner ? learner->name : "Unknown");

                // Validate Topic
                string topic = getValidatedString("  Enter Topic: ", MAX_TOPIC_LENGTH, true);

                // Validate Difficulty
                int difficulty = getValidatedInt("  Enter Difficulty (1-5): ", MIN_DIFFICULTY, MAX_DIFFICULTY);

                // Validate Score
                int score = getValidatedInt("  Enter Score (0-100): ", MIN_SCORE, MAX_SCORE);

                // Confirm entry
                cout << "\n  Log Entry Summary:\n";
                cout << "  Learner: " << learnerName << " (ID: " << learnerId << ")\n";
                cout << "  Topic: " << topic << "\n";
                cout << "  Difficulty: " << difficultyLabel(difficulty) << "\n";
                cout << "  Score: " << score << " (" << (score >= 50 ? "PASS" : "FAIL") << ")\n";
                
                char confirm;
                cout << "\n  Add this log entry? (y/n): ";
                cin >> confirm;
                cin.ignore();
                
                if (tolower(confirm) == 'y') {
                    globalLog.addLog(learnerId, learnerName, topic, difficulty, score);

                    if (learner) {
                        learner->totalAttempts++;
                        if (score < 50) learner->failedAttempts++;
                        learner->averageScore =
                            ((learner->averageScore * (learner->totalAttempts - 1)) + score)
                            / learner->totalAttempts;
                    }

                    cout << "  [SUCCESS] Log entry added.\n";
                } else {
                    cout << "  Log entry cancelled.\n";
                }
                break;
            }

            case 2:
                globalLog.displayAll();
                break;

            case 3: {
                int id = getValidatedInt("  Enter Learner ID to filter (1000-9999): ", MIN_ID, MAX_ID);
                globalLog.displayByLearner(id);
                break;
            }

            case 4: {
                string filename;
                cout << "  Enter filename for CSV export (default: activity_logs.csv): ";
                getline(cin, filename);
                if (filename.empty()) {
                    filename = "activity_logs.csv";
                }
                globalLog.exportCSV(filename);
                break;
            }

            case 5: {
                int id = getValidatedInt("  Enter Learner ID for report (1000-9999): ", MIN_ID, MAX_ID);
                
                string filename;
                cout << "  Enter filename for report (default: learner_" + to_string(id) + "_report.txt): ";
                getline(cin, filename);
                if (filename.empty()) {
                    filename = "learner_" + to_string(id) + "_report.txt";
                }
                
                globalLog.exportLearnerReport(id, filename);
                break;
            }

            case 0:
                cout << "  Returning to Main Menu...\n";
                break;
        }
    } while (choice != 0);
}