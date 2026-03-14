#include "constants.hpp"
#include "shared_models.hpp"
#include "shared_utils.hpp"
#include "global_registry.hpp"
#include "task1_registration.hpp"
#include "task2_navigation.hpp"
#include "task3_logging.hpp"
#include "task4_risk.hpp"

// ============================================================
//  MAIN ENTRY POINT
// ============================================================
int main() {
    printBanner("WELCOME TO PLAPS");
    cout << "  Programming Learning Analytics & Personalization System" << endl;
    cout << "  Asia Pacific University (APU)" << endl;
    printDivider();

    initializeDummyData();
    printDivider();

    int choice;
    do {
        cout << "\n";
        printBanner("MAIN MENU");
        cout << "  1. Task 1 - Learner Registration & Session Management" << endl;
        cout << "  2. Task 2 - Activity Navigation & Session Flow" << endl;
        cout << "  3. Task 3 - Recent Activity Logging & Performance History" << endl;
        cout << "  4. Task 4 - At-Risk Learner Priority & Recommendation Engine" << endl;
        cout << "  0. Exit System" << endl;
        printDivider();
        
        choice = getValidatedInt("  Enter choice (0-4): ", 0, 4);

        switch (choice) {
            case 1: task1Menu();       break;
            case 2: task2Menu();       break;
            case 3: task3Menu();       break;
            case 4: task4Menu();       break;
            case 0:
                printBanner("GOODBYE");
                cout << "  Thank you for using PLAPS. Exiting...\n";
                
                cout << "\n  Session Summary:\n";
                cout << "  - Total Learners Registered: " << registryCount << " (including 10 dummy)" << endl;
                
                int totalActivities = 0;
                for (int i = 0; i < progressCount; i++) {
                    totalActivities += learnerProgress[i].navigationStack.size();
                }
                cout << "  - Total Activities Completed: " << totalActivities << endl;
                cout << "  - Total Activities Logged: " << globalLog.size() << endl;
                cout << "  - At-Risk Learners Tracked: " << riskQueue.size() << endl;
                printDivider();
                break;
        }
    } while (choice != 0);

    return 0;
}