#include "task4_risk.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm>
using namespace std;

// ============================================================
//  TASK 4 IMPLEMENTATION
// ============================================================

// ---- Task 4 global priority queue ----
RiskPriorityQueue riskQueue;

RiskPriorityQueue::RiskPriorityQueue() : heapSize(0) {}

bool RiskPriorityQueue::isEmpty() const { return heapSize == 0; }
int  RiskPriorityQueue::size()    const { return heapSize; }

int RiskPriorityQueue::parent(int i) { return (i - 1) / 2; }
int RiskPriorityQueue::leftChild(int i)  { return 2 * i + 1; }
int RiskPriorityQueue::rightChild(int i) { return 2 * i + 2; }

void RiskPriorityQueue::swapEntries(int i, int j) {
    AtRiskEntry temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void RiskPriorityQueue::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)].riskScore < heap[i].riskScore) {
        swapEntries(i, parent(i));
        i = parent(i);
    }
}

void RiskPriorityQueue::heapifyDown(int i) {
    int largest = i;
    int l = leftChild(i);
    int r = rightChild(i);

    if (l < heapSize && heap[l].riskScore > heap[largest].riskScore)
        largest = l;
    if (r < heapSize && heap[r].riskScore > heap[largest].riskScore)
        largest = r;

    if (largest != i) {
        swapEntries(i, largest);
        heapifyDown(largest);
    }
}

void RiskPriorityQueue::insert(const AtRiskEntry& entry) {
    if (heapSize >= MAX_HEAP_SIZE) {
        cout << "  [WARNING] Risk queue is full. Cannot add more learners.\n";
        return;
    }
    
    // Validate entry
    if (entry.learnerId < MIN_ID || entry.learnerId > MAX_ID) {
        cout << "  [ERROR] Invalid learner ID. Cannot add to queue.\n";
        return;
    }
    
    heap[heapSize] = entry;
    heapifyUp(heapSize);
    heapSize++;
}

AtRiskEntry RiskPriorityQueue::peekMax() const {
    if (isEmpty()) {
        AtRiskEntry empty = {0, "N/A", 0.0f, "N/A", "N/A", 0.0f, 0, 0};
        return empty;
    }
    return heap[0];
}

AtRiskEntry RiskPriorityQueue::extractMax() {
    if (isEmpty()) {
        AtRiskEntry empty = {0, "N/A", 0.0f, "N/A", "N/A", 0.0f, 0, 0};
        return empty;
    }
    AtRiskEntry top = heap[0];
    heap[0] = heap[--heapSize];
    heapifyDown(0);
    return top;
}

bool RiskPriorityQueue::contains(int learnerId) const {
    for (int i = 0; i < heapSize; i++)
        if (heap[i].learnerId == learnerId) return true;
    return false;
}

void RiskPriorityQueue::displayAll() const {
    printBanner("AT-RISK LEARNER PRIORITY LIST");
    if (isEmpty()) {
        cout << "  (No at-risk learners identified)" << endl;
        printDivider();
        return;
    }
    
    // Calculate statistics
    int critical = 0, high = 0, medium = 0, low = 0;
    for (int i = 0; i < heapSize; i++) {
        if (heap[i].riskScore >= 75.0f) critical++;
        else if (heap[i].riskScore >= 50.0f) high++;
        else if (heap[i].riskScore >= 25.0f) medium++;
        else low++;
    }
    
    cout << "  Risk Distribution: Critical: " << critical 
         << " | High: " << high << " | Medium: " << medium 
         << " | Low: " << low << endl;
    printDivider();
    
    cout << left
         << setw(5)  << "Rank"
         << setw(6)  << "ID"
         << setw(18) << "Name"
         << setw(10) << "Risk Lvl"
         << setw(10) << "Risk Scr"
         << setw(10) << "Avg Scr"
         << setw(7)  << "Failed"
         << "Recommendation"
         << endl;
    printDivider();

    // Copy heap to a temp array and sort by riskScore descending
    AtRiskEntry temp[MAX_HEAP_SIZE];
    for (int i = 0; i < heapSize; i++) temp[i] = heap[i];

    // Simple selection sort (acceptable for small N)
    for (int i = 0; i < heapSize - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < heapSize; j++)
            if (temp[j].riskScore > temp[maxIdx].riskScore)
                maxIdx = j;
        if (maxIdx != i) {
            AtRiskEntry t = temp[i]; 
            temp[i] = temp[maxIdx]; 
            temp[maxIdx] = t;
        }
    }

    for (int i = 0; i < heapSize; i++) {
        cout << left
             << setw(5)  << (i + 1)
             << setw(6)  << temp[i].learnerId
             << setw(18) << (temp[i].learnerName.length() > 17 ? 
                            temp[i].learnerName.substr(0, 16) + "." : 
                            temp[i].learnerName)
             << setw(10) << temp[i].riskLevel
             << setw(10) << fixed << setprecision(1) << temp[i].riskScore
             << setw(10) << fixed << setprecision(1) << temp[i].averageScore
             << setw(7)  << temp[i].failedAttempts
             << (temp[i].recommendation.length() > 30 ? 
                temp[i].recommendation.substr(0, 29) + "..." : 
                temp[i].recommendation)
             << endl;
    }
    printDivider();
    cout << "  Total at-risk learners: " << heapSize << "/" << MAX_HEAP_SIZE << endl;
    printDivider();
}

void RiskPriorityQueue::exportCSV(const string& filename) const {
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
    
    file << "Rank,LearnerID,LearnerName,RiskLevel,RiskScore,"
         << "AverageScore,FailedAttempts,TotalAttempts,Recommendation\n";

    // Sort for export
    AtRiskEntry temp[MAX_HEAP_SIZE];
    for (int i = 0; i < heapSize; i++) temp[i] = heap[i];
    
    for (int i = 0; i < heapSize - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < heapSize; j++)
            if (temp[j].riskScore > temp[maxIdx].riskScore)
                maxIdx = j;
        if (maxIdx != i) {
            AtRiskEntry t = temp[i]; 
            temp[i] = temp[maxIdx]; 
            temp[maxIdx] = t;
        }
    }

    for (int i = 0; i < heapSize; i++) {
        file << (i + 1)                  << ","
             << temp[i].learnerId        << ","
             << "\"" << temp[i].learnerName << "\","  // Quotes to handle commas
             << temp[i].riskLevel        << ","
             << fixed << setprecision(2) << temp[i].riskScore       << ","
             << fixed << setprecision(2) << temp[i].averageScore    << ","
             << temp[i].failedAttempts   << ","
             << temp[i].totalAttempts    << ","
             << "\"" << temp[i].recommendation << "\"\n";  // Quotes for recommendation
    }
    
    file.close();
    cout << "  [SUCCESS] At-risk list exported to \"" << filename << "\" (" 
         << heapSize << " learners)\n";
}

// Calculates a risk score (0–100) for a given learner
float calculateRiskScore(const Learner& l, float avgDifficulty) {
    // Handle edge cases
    if (l.totalAttempts == 0) {
        // New learner with no attempts - moderate risk
        return 30.0f;
    }
    
    float failRate      = (float)l.failedAttempts / l.totalAttempts;
    
    // Normalize failRate to 0-1 range (it already is)
    
    float lowAvgPenalty = (l.averageScore < 60.0f)
                          ? (60.0f - l.averageScore) / 60.0f
                          : 0.0f;
    
    // Clamp lowAvgPenalty to 0-1
    if (lowAvgPenalty > 1.0f) lowAvgPenalty = 1.0f;
    if (lowAvgPenalty < 0.0f) lowAvgPenalty = 0.0f;
    
    float diffGap       = (avgDifficulty - 1.0f) / 4.0f;
    // Clamp diffGap
    if (diffGap > 1.0f) diffGap = 1.0f;
    if (diffGap < 0.0f) diffGap = 0.0f;
    
    float attemptPenalty= (l.totalAttempts < 3) ? 1.0f : 0.0f;

    float score = (failRate      * 40.0f)
                + (lowAvgPenalty * 30.0f)
                + (diffGap       * 20.0f)
                + (attemptPenalty* 10.0f);

    // Clamp to [0, 100]
    if (score < 0.0f)   score = 0.0f;
    if (score > 100.0f) score = 100.0f;
    
    return score;
}

// Converts a numeric risk score to a label
string riskLabel(float score) {
    if (score >= 75.0f) return "Critical";
    if (score >= 50.0f) return "High";
    if (score >= 25.0f) return "Medium";
    return "Low";
}

// Generates a recommendation string based on learner performance
string generateRecommendation(const Learner& l, float riskScore) {
    if (l.totalAttempts == 0) {
        return "New learner - needs to start with basic activities";
    }
    
    if (riskScore >= 75.0f) {
        if (l.failedAttempts > l.totalAttempts / 2) {
            return "URGENT: One-on-one tutoring required - failing consistently";
        } else {
            return "URGENT: Assign tutor & restart from basics";
        }
    }
    
    if (riskScore >= 50.0f) {
        if (l.averageScore < 40.0f) {
            return "Repeat failed topics at lower difficulty - scores very low";
        } else {
            return "Repeat failed topics at lower difficulty";
        }
    }
    
    if (riskScore >= 25.0f) {
        if (l.totalAttempts < 3) {
            return "Complete more activities to establish baseline";
        } else {
            return "Review recent topics & attempt practice quiz";
        }
    }
    
    return "Continue current path; monitor progress";
}

// ---- Task 4 menu handler ----
void task4Menu() {
    int choice;
    do {
        printBanner("TASK 4: AT-RISK LEARNER PRIORITY ENGINE");
        cout << "  At-risk learners tracked: " << riskQueue.size() << "/" << MAX_HEAP_SIZE << endl;
        printDivider();
        cout << "  1. Analyse & Add Learner to Risk Queue" << endl;
        cout << "  2. View All At-Risk Learners (Priority Order)" << endl;
        cout << "  3. View Highest-Risk Learner (Peek)" << endl;
        cout << "  4. Extract & Process Highest-Risk Learner" << endl;
        cout << "  5. Analyse All Registered Learners" << endl;
        cout << "  6. Export At-Risk List to CSV" << endl;
        cout << "  0. Back to Main Menu" << endl;
        printDivider();
        
        choice = getValidatedInt("  Enter choice (0-6): ", 0, 6);

        switch (choice) {
            case 1: {
                printBanner("ANALYSE LEARNER RISK");
                
                // Validate Learner ID
                int id;
                Learner* l = NULL;
                
                while (true) {
                    id = getValidatedInt("  Enter Learner ID (1000-9999): ", MIN_ID, MAX_ID);
                    l = findLearnerById(id);
                    
                    if (!l) {
                        cout << "  [ERROR] Learner ID not found. Please register first in Task 1.\n";
                        char choice;
                        cout << "  Do you want to try again? (y/n): ";
                        cin >> choice;
                        cin.ignore();
                        if (tolower(choice) != 'y') {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                
                if (!l) break;

                // Check if already in queue
                if (riskQueue.contains(id)) {
                    cout << "  [INFO] Learner already in risk queue.\n";
                    
                    // Show current risk info
                    cout << "\n  Current risk status:\n";
                    // We need to find the entry - simplified by just showing current calculation
                    float risk = calculateRiskScore(*l);
                    string rec = generateRecommendation(*l, risk);
                    
                    cout << "  Risk Score: " << fixed << setprecision(2) << risk << endl;
                    cout << "  Risk Level: " << riskLabel(risk) << endl;
                    cout << "  Recommendation: " << rec << endl;
                    break;
                }

                // Validate that learner has attempted activities
                if (l->totalAttempts == 0) {
                    cout << "  [WARNING] Learner has no activity attempts yet. Risk score will be estimated.\n";
                    char choice;
                    cout << "  Continue analysis? (y/n): ";
                    cin >> choice;
                    cin.ignore();
                    if (tolower(choice) != 'y') {
                        break;
                    }
                }

                float risk = calculateRiskScore(*l);
                string rec = generateRecommendation(*l, risk);

                printDivider();
                cout << "  Learner       : " << l->name          << endl;
                cout << "  Avg Score     : " << fixed << setprecision(2)
                                             << l->averageScore  << endl;
                cout << "  Total Attempts: " << l->totalAttempts << endl;
                cout << "  Failed        : " << l->failedAttempts<< endl;
                cout << "  Fail Rate     : " << fixed << setprecision(1)
                                             << (l->totalAttempts > 0 ? 
                                                (l->failedAttempts * 100.0 / l->totalAttempts) : 0)
                                             << "%" << endl;
                cout << "  Risk Score    : " << fixed << setprecision(2) << risk << endl;
                cout << "  Risk Level    : " << riskLabel(risk)  << endl;
                cout << "  Recommendation: " << rec              << endl;
                printDivider();

                char confirm;
                cout << "  Add this learner to risk priority queue? (y/n): ";
                cin >> confirm;
                cin.ignore();
                
                if (tolower(confirm) == 'y') {
                    AtRiskEntry entry = {id, l->name, risk, riskLabel(risk),
                                         rec, l->averageScore,
                                         l->failedAttempts, l->totalAttempts};
                    riskQueue.insert(entry);
                    cout << "  [SUCCESS] Learner added to risk priority queue.\n";
                } else {
                    cout << "  Operation cancelled.\n";
                }
                break;
            }

            case 2:
                riskQueue.displayAll();
                break;

            case 3: {
                printBanner("HIGHEST-RISK LEARNER");
                if (riskQueue.isEmpty()) {
                    cout << "  [INFO] Risk queue is empty.\n";
                    break;
                }
                AtRiskEntry top = riskQueue.peekMax();
                printDivider();
                cout << "  Learner ID    : " << top.learnerId      << endl;
                cout << "  Name          : " << top.learnerName    << endl;
                cout << "  Risk Level    : " << top.riskLevel      << endl;
                cout << "  Risk Score    : " << fixed << setprecision(2)
                                             << top.riskScore      << endl;
                cout << "  Avg Score     : " << fixed << setprecision(2)
                                             << top.averageScore   << endl;
                cout << "  Failed Attempts: " << top.failedAttempts << " / " 
                     << top.totalAttempts << endl;
                cout << "  Recommendation: " << top.recommendation << endl;
                printDivider();
                break;
            }

            case 4: {
                printBanner("PROCESS HIGHEST-RISK LEARNER");
                if (riskQueue.isEmpty()) {
                    cout << "  [INFO] Risk queue is empty.\n";
                    break;
                }
                
                AtRiskEntry top = riskQueue.peekMax();
                cout << "  Next to process: " << top.learnerName << " (Risk Score: " 
                     << fixed << setprecision(2) << top.riskScore << ")\n";
                
                char confirm;
                cout << "  Process this learner now? (y/n): ";
                cin >> confirm;
                cin.ignore();
                
                if (tolower(confirm) == 'y') {
                    top = riskQueue.extractMax();
                    cout << "\n  [PROCESSING] Learner: " << top.learnerName << endl;
                    cout << "  Risk Score   : " << fixed << setprecision(2)
                                                << top.riskScore << endl;
                    cout << "  Action Taken : " << top.recommendation << endl;
                    
                    // Simulate intervention
                    cout << "\n  Intervention Steps:\n";
                    cout << "  1. Notified academic advisor\n";
                    cout << "  2. Scheduled review session\n";
                    cout << "  3. Assigned remedial exercises\n";
                    
                    cout << "\n  [SUCCESS] Learner removed from risk queue after processing.\n";
                } else {
                    cout << "  Processing cancelled.\n";
                }
                break;
            }

            case 5: {
                printBanner("ANALYSE ALL REGISTERED LEARNERS");
                if (registryCount == 0) {
                    cout << "  [INFO] No learners registered yet.\n";
                    break;
                }
                
                int added = 0;
                int skipped = 0;
                int alreadyInQueue = 0;
                
                for (int i = 0; i < registryCount; i++) {
                    Learner& l = globalRegistry[i];
                    
                    if (riskQueue.contains(l.id)) {
                        alreadyInQueue++;
                        continue;
                    }
                    
                    float risk = calculateRiskScore(l);
                    if (risk >= 25.0f) {  // Medium risk and above
                        string rec = generateRecommendation(l, risk);
                        AtRiskEntry entry = {l.id, l.name, risk, riskLabel(risk),
                                             rec, l.averageScore,
                                             l.failedAttempts, l.totalAttempts};
                        riskQueue.insert(entry);
                        added++;
                    } else {
                        skipped++;
                    }
                }
                
                cout << "  [SUCCESS] Analysis complete.\n";
                cout << "  - Added to risk queue: " << added << "\n";
                cout << "  - Already in queue: " << alreadyInQueue << "\n";
                cout << "  - Low risk (skipped): " << skipped << "\n";
                
                if (added > 0) {
                    cout << "\n  Displaying updated risk queue:\n";
                    riskQueue.displayAll();
                }
                break;
            }

            case 6: {
                string filename;
                cout << "  Enter filename for CSV export (default: at_risk_learners.csv): ";
                getline(cin, filename);
                if (filename.empty()) {
                    filename = "at_risk_learners.csv";
                }
                riskQueue.exportCSV(filename);
                break;
            }

            case 0:
                cout << "  Returning to Main Menu...\n";
                break;
        }
    } while (choice != 0);
}