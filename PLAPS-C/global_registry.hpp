#ifndef GLOBAL_REGISTRY_HPP
#define GLOBAL_REGISTRY_HPP

#include "constants.hpp"
#include "shared_models.hpp"
#include "task1_registration.hpp"
#include "task3_logging.hpp"

// ============================================================
//  GLOBAL LEARNER REGISTRY
// ============================================================
extern Learner globalRegistry[MAX_LEARNERS];
extern int     registryCount;

// Finds a learner in the global registry by ID.
Learner* findLearnerById(int id);

// ============================================================
//  DUMMY DATA INITIALIZATION
// ============================================================
void initializeDummyData();

#endif