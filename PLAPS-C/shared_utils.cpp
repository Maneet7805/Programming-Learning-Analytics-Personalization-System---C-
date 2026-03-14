#include "shared_utils.hpp"
#include <sstream>

// ============================================================
//  UTILITY FUNCTIONS IMPLEMENTATION
// ============================================================

// Returns the current date-time as a formatted string
string getCurrentTimestamp() {
    time_t now = time(0);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// Prints a horizontal divider line
void printDivider(char ch, int width) {
    for (int i = 0; i < width; i++) cout << ch;
    cout << endl;
}

// Prints a centred title banner
void printBanner(const string& title) {
    printDivider('=');
    int padding = (60 - (int)title.length()) / 2;
    cout << string(padding, ' ') << title << endl;
    printDivider('=');
}

// Converts difficulty integer to a readable label
string difficultyLabel(int d) {
    switch (d) {
        case 1: return "Very Easy";
        case 2: return "Easy";
        case 3: return "Medium";
        case 4: return "Hard";
        case 5: return "Very Hard";
        default: return "Unknown";
    }
}

// Validates that an integer input is within [lo, hi]
bool validateRange(int value, int lo, int hi) {
    return (value >= lo && value <= hi);
}

// ============================================================
//  VALIDATION FUNCTIONS IMPLEMENTATION
// ============================================================

// Validates learner ID format and range
bool validateLearnerId(int id) {
    return (id >= MIN_ID && id <= MAX_ID);
}

// Validates name (no numbers, proper length)
bool validateName(const string& name) {
    if (name.empty() || name.length() > MAX_NAME_LENGTH) return false;
    
    // Check each character
    for (char c : name) {
        // Allow letters, spaces, hyphens, apostrophes
        if (!isalpha(c) && c != ' ' && c != '-' && c != '\'') {
            return false;
        }
    }
    return true;
}

// Validates email format (must contain @ and .)
bool validateEmail(const string& email) {
    if (email.empty() || email.length() > MAX_EMAIL_LENGTH) return false;
    
    // Must contain @ and at least one . after @
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == email.length() - 1) {
        return false;
    }
    
    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.length() - 1) {
        return false;
    }
    
    // Check for invalid characters
    for (char c : email) {
        if (!isalnum(c) && c != '@' && c != '.' && c != '-' && c != '_') {
            return false;
        }
    }
    
    return true;
}

// Validates level choice
bool validateLevel(int level) {
    return (level >= MIN_LEVEL && level <= MAX_LEVEL);
}

// Validates score
bool validateScore(int score) {
    return (score >= MIN_SCORE && score <= MAX_SCORE);
}

// Validates difficulty
bool validateDifficulty(int difficulty) {
    return (difficulty >= MIN_DIFFICULTY && difficulty <= MAX_DIFFICULTY);
}

// Validates topic name
bool validateTopic(const string& topic) {
    return (!topic.empty() && topic.length() <= MAX_TOPIC_LENGTH);
}

// Clears input buffer and handles invalid input
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Gets validated integer input within range
int getValidatedInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) {
                cin.ignore();
                return value;
            } else {
                cout << "  [ERROR] Value must be between " << min << " and " << max << ". Please try again.\n";
            }
        } else {
            cout << "  [ERROR] Invalid input. Please enter a number.\n";
            clearInput();
        }
    }
}

// Gets validated string input with length limit
string getValidatedString(const string& prompt, int maxLength, bool allowNumbers) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Trim leading/trailing spaces
        size_t start = input.find_first_not_of(" \t");
        size_t end = input.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos) {
            input = input.substr(start, end - start + 1);
        } else {
            input = "";
        }
        
        if (input.empty()) {
            cout << "  [ERROR] Input cannot be empty. Please try again.\n";
            continue;
        }
        
        if (input.length() > maxLength) {
            cout << "  [ERROR] Input too long. Maximum " << maxLength << " characters.\n";
            continue;
        }
        
        if (!allowNumbers) {
            bool hasDigit = false;
            for (char c : input) {
                if (isdigit(c)) {
                    hasDigit = true;
                    break;
                }
            }
            if (hasDigit) {
                cout << "  [ERROR] Name cannot contain numbers. Please try again.\n";
                continue;
            }
        }
        
        return input;
    }
}

// Gets validated email input
string getValidatedEmail(const string& prompt) {
    string email;
    while (true) {
        cout << prompt;
        getline(cin, email);
        
        // Trim spaces
        size_t start = email.find_first_not_of(" \t");
        size_t end = email.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos) {
            email = email.substr(start, end - start + 1);
        }
        
        if (validateEmail(email)) {
            return email;
        } else {
            cout << "  [ERROR] Invalid email format. Please enter a valid email (e.g., name@domain.com).\n";
        }
    }
}