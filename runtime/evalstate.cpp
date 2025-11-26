// evalstate.cpp
#include "evalstate.h"

/*
 * Constructor / Destructor
 */
EvalState::EvalState() {}

EvalState::~EvalState() {}


/*
 * Set variable value
 */
void EvalState::setValue(const std::string &var, int value) {
    symbolTable[var] = value;
}


/*
 * Get variable value
 */
int EvalState::getValue(const std::string &var) const {
    auto it = symbolTable.find(var);
    if (it == symbolTable.end()) {
        throw std::runtime_error("VARIABLE NOT DEFINED: " + var);
    }
    return it->second;
}


/*
 * Check if variable exists
 */
bool EvalState::isDefined(const std::string &var) const {
    return symbolTable.count(var) > 0;
}


/*
 * Clear all variables
 */
void EvalState::clear() {
    symbolTable.clear();
}
