#ifndef SHARED_UTILS_HPP
#define SHARED_UTILS_HPP

#include "constants.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <limits>
using namespace std;

// ============================================================
//  UTILITY FUNCTIONS
// ============================================================

// Returns the current date-time as a formatted string
string getCurrentTimestamp();

// Prints a horizontal divider line
void printDivider(char ch = '-', int width = 60);

// Prints a centred title banner
void printBanner(const string& title);

// Converts difficulty integer to a readable label
string difficultyLabel(int d);

// Validates that an integer input is within [lo, hi]
bool validateRange(int value, int lo, int hi);

// ============================================================
//  VALIDATION FUNCTIONS
// ============================================================

// Validates learner ID format and range
bool validateLearnerId(int id);

// Validates name (no numbers, proper length)
bool validateName(const string& name);

// Validates email format (must contain @ and .)
bool validateEmail(const string& email);

// Validates level choice
bool validateLevel(int level);

// Validates score
bool validateScore(int score);

// Validates difficulty
bool validateDifficulty(int difficulty);

// Validates topic name
bool validateTopic(const string& topic);

// Clears input buffer and handles invalid input
void clearInput();

// Gets validated integer input within range
int getValidatedInt(const string& prompt, int min, int max);

// Gets validated string input with length limit
string getValidatedString(const string& prompt, int maxLength, bool allowNumbers = true);

// Gets validated email input
string getValidatedEmail(const string& prompt);

#endif