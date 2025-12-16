#pragma once

#include <functional>
#include <QString>

#include "../core/statement.h"
#include "../core/program.h"
#include "../runtime/evalstate.h"
//#include "../runtime/parser.h"

/**
 * Interpreter class
 * 
 * Drives program execution by iterating through statements and calling Statement::execute()
 * Features:
 * - Maintains EvalState for variable storage
 * - Supports full run() and single-step step() execution modes
 * - Provides I/O callbacks for UI integration
 * - Does not directly access statement internals, uses polymorphic execute() interface
 * 
 * Note: For proper decoupling from Statement/Program, statements access I/O callbacks
 * through EvalState or global registry. Callbacks injected via setters.
 */
class Interpreter {
public:
    // Constructor and destructor
    Interpreter();
    ~Interpreter();

    // Execution modes
    
    // Run program until completion or END statement
    void run(Program& program);

    // Execute exactly one statement
    // Returns: true if statement was executed, false if program ended or no statement
    bool step(Program &program);

    // Reset interpreter state: clears all variables and resets execution
    void reset();

    // I/O callback configuration
    
    // Set input provider callback (called by INPUT statements)
    void setInputProvider(std::function<QString()> f) {
        state.inputProvider = std::move(f);
    }
    
    // Set output consumer callback (called by PRINT statements)
    void setOutputConsumer(std::function<void(const QString&)> f) {
        state.outputConsumer = std::move(f);
    }

    // State access
    
    // Get evaluation state (for direct access if needed by UI or tests)
    EvalState &getState();

    // Debugging support
    
    // Get syntax tree representation (use after running program)
    // Shows execution counts and structure
    std::string toSyntaxTree(Program &program) const;

private:
    EvalState state;                                // Variable bindings and runtime state
    
    // I/O callbacks (may be nullptr if not configured)
    std::function<int()> inputProvider;             // Provides input for INPUT statement
    std::function<void(const QString&)> outputConsumer;  // Consumes output from PRINT statement

    // Internal helper method
    // Advance to next line if needed (used after conditional branches)
    void defaultAdvanceIfNeeded(Program &program, int currentLine);
};
