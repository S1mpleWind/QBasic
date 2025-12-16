/**
 * @file    evalstate.h
 * @brief   Runtime evaluation state for variable storage and runtime statistics
 *          Manages variable bindings and tracks runtime information
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include<QObject>

// Runtime statistics: tracks variable usage during execution
struct RuntimeStats {
    // Maps variable name to count of identifier uses
    std::map<std::string, int> identifierUseCount;
};

// EvalState: Stores runtime state and execution context
// Manages variable bindings and I/O callbacks for the interpreter
class EvalState {
public:
    // Constructor and destructor
    EvalState();
    ~EvalState();

    // Variable binding operations
    
    // Set the value of a variable (creates if not exists)
    void setValue(const std::string &var, int value);

    // Get the value of a variable (throws if not defined)
    int getValue(const std::string &var) const;

    // Check whether a variable is defined
    bool isDefined(const std::string &var) const;

    // Clear all variables
    void clear();

    // Runtime statistics management
    
    // Set runtime statistics object
    void setRuntimeStats(RuntimeStats* stats) { runtimeStats = stats; }
    
    // Get runtime statistics object
    RuntimeStats* getRuntimeStats() const { return runtimeStats; }


    // I/O callback functions for UI integration
    // inputProvider: called by INPUT statements to get user input
    std::function<QString()> inputProvider;
    
    // outputConsumer: called by PRINT statements to send output to UI
    std::function<void(const QString&)> outputConsumer;

private:
    std::map<std::string, int> symbolTable;   // Variable binding table: name -> integer value

    RuntimeStats* runtimeStats = nullptr;     // Pointer to runtime statistics object
};



