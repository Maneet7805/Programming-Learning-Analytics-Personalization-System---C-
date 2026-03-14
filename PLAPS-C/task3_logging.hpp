#ifndef TASK3_LOGGING_HPP
#define TASK3_LOGGING_HPP

#include "constants.hpp"
#include "shared_models.hpp"
#include "global_registry.hpp"
#include "shared_utils.hpp"

// ============================================================
//  TASK 3 – RECENT ACTIVITY LOGGING & PERFORMANCE HISTORY
//  Data Structure: Array-Based Circular Queue
// ============================================================

class CircularActivityLog {
private:
    ActivityLog* buffer;      // Fixed-size array (ring buffer)
    int          frontIdx;    // Index of the oldest entry
    int          rearIdx;     // Index of the most recent entry
    int          capacity;    // Maximum number of entries
    int          count;       // Current number of entries

public:
    // Constructor – allocate the ring buffer
    CircularActivityLog(int size = LOG_CAPACITY);
    
    // Destructor
    ~CircularActivityLog();

    bool isEmpty() const;
    bool isFull()  const;
    int  size()    const;
    int  getCapacity() const;

    // Add a new log entry; overwrites the oldest if full
    void addLog(int learnerId, const string& learnerName,
                const string& topic, int difficulty, int score);
    
    // Display all log entries (oldest → newest)
    void displayAll() const;
    
    // Display logs filtered by a specific learner ID
    void displayByLearner(int learnerId) const;
    
    // Export all logs to a CSV file
    void exportCSV(const string& filename = "activity_logs.csv") const;
    
    // Export logs for a specific learner to a text file
    void exportLearnerReport(int learnerId,
                             const string& filename = "learner_report.txt") const;
};

// ---- Task 3 global circular log ----
extern CircularActivityLog globalLog;

// Log function called by Task 2
void logActivity(int learnerId, const string& learnerName,
                 const string& topic, int difficulty, int score);

// ---- Task 3 menu handler ----
void task3Menu();

#endif