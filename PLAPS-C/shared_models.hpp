#ifndef SHARED_MODELS_HPP
#define SHARED_MODELS_HPP

#include <string>
using namespace std;

// ============================================================
//  SHARED DATA MODELS
// ============================================================

// Stores core learner profile information
struct Learner {
    int    id;
    string name;
    string email;
    string level;          // "Beginner", "Intermediate", "Advanced"
    float  averageScore;   // Running average score (0.0 - 100.0)
    int    totalAttempts;  // Total activities attempted
    int    failedAttempts; // Attempts where score < 50
    bool   isActive;       // Currently in an active session
};

// Stores a single learning activity definition
struct Activity {
    int    activityId;
    string topic;
    string description;
    int    difficulty;     // 1 (Easy) to 5 (Hard)
    int    score;          // Score achieved (0 - 100), -1 if not yet done
    bool   isCompleted;
};

// Stores a single activity log entry (used in Task 3)
struct ActivityLog {
    int    learnerId;
    string learnerName;
    string topic;
    int    difficulty;
    int    score;
    bool   passed;         // score >= 50
    string timestamp;
};

// At-risk entry structure (used in Task 4)
struct AtRiskEntry {
    int    learnerId;
    string learnerName;
    float  riskScore;          // 0.0 (low risk) to 100.0 (high risk)
    string riskLevel;          // "Low", "Medium", "High", "Critical"
    string recommendation;
    float  averageScore;
    int    failedAttempts;
    int    totalAttempts;
};

#endif