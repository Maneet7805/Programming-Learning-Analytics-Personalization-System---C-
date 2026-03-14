#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// ============================================================
//  CONSTANTS
// ============================================================
const int MAX_SESSION_CAPACITY  = 20;   // Max learners in one session
const int LOG_CAPACITY          = 20;   // Circular log buffer size
const int MAX_HEAP_SIZE         = 50;   // Max at-risk learners in heap
const int MAX_LEARNERS          = 100;  // Global learner registry size
const int MAX_ACTIVITIES        = 50;   // Max activities per session

// Validation Constants
const int MIN_ID                = 1000;  // Minimum learner ID
const int MAX_ID                = 9999;  // Maximum learner ID
const int MIN_SCORE             = 0;     // Minimum score
const int MAX_SCORE             = 100;   // Maximum score
const int MIN_DIFFICULTY        = 1;     // Minimum difficulty
const int MAX_DIFFICULTY        = 5;     // Maximum difficulty
const int MIN_LEVEL             = 1;     // Minimum level choice
const int MAX_LEVEL             = 3;     // Maximum level choice
const int MAX_NAME_LENGTH       = 50;    // Maximum name length
const int MAX_EMAIL_LENGTH      = 100;   // Maximum email length
const int MAX_TOPIC_LENGTH      = 50;    // Maximum topic length

#endif