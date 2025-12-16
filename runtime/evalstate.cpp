// evalstate.cpp
// Implementation of runtime state management
#include "evalstate.h"

// Constructor: initialize runtime state with empty symbol table
EvalState::EvalState() {
    runtimeStats = new RuntimeStats();
}

// Destructor: clean up runtime statistics
EvalState::~EvalState() {
    delete runtimeStats;
}

// Set variable value in symbol table
// Creates variable if not exists, updates if exists
void EvalState::setValue(const std::string &var, int value) {
    symbolTable[var] = value;
}

// Get variable value from symbol table
// Throws error if variable not defined
int EvalState::getValue(const std::string &var) const {
    auto it = symbolTable.find(var);
    if (it == symbolTable.end()) {
        throw std::runtime_error("VARIABLE NOT DEFINED: " + var);
    }
    return it->second;
}

// Check if variable is defined in symbol table
bool EvalState::isDefined(const std::string &var) const {
    return symbolTable.count(var) > 0;
}


/*
 * Clear all variables
 */
void EvalState::clear() {
    symbolTable.clear();
}
