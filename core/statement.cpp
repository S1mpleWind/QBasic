// statement.cpp
// Implementation of BASIC statement classes
#include "statement.h"
#include "../runtime/tokenizer.h"
#include <iostream>

#include <QDebug>

// ============ REM Statement Implementation ============
// REM: comment line with no runtime effect

RemStmt::RemStmt(const std::string &text) : text(text) {}

// Execute: comment statements have no effect
void RemStmt::execute(EvalState &state, Program &) {
    // No effect during runtime
}

// Syntax tree representation for REM
std::string RemStmt::toSyntaxTree(const RuntimeStats * , int indent) const {
    std::string s;
    s += indentFunc(indent) + "REM " + text;
    return s;
}

// ============ LET Statement Implementation ============
// LET: variable assignment statement

// Constructor: create assignment with variable name and expression
LetStmt::LetStmt(const std::string &varName, Expression *exp)
    : var(varName), exp(exp) {
    if (!Tokenizer::isValidIdentifier(varName)) {
        throw std::runtime_error("INVALID IDENTIFIER: " + varName);
    }
}

// Destructor: clean up expression
LetStmt::~LetStmt() {
    delete exp;
}

// Execute: evaluate expression and assign to variable
void LetStmt::execute(EvalState &state, Program &) {
    execCount++;

    // Evaluate the expression and store result in variable
    int value = exp->eval(state);
    state.setValue(var, value);
}

// Syntax tree representation for LET
std::string LetStmt::toSyntaxTree(const RuntimeStats * stats, int indent) const {
    std::string s;
    int useCount = 0;
    if (stats) {
        auto it = stats->identifierUseCount.find(var);
        if (it != stats->identifierUseCount.end()) useCount = it->second;
    }
    s += indentFunc(indent) + "LET = " + std::to_string(execCount) + "\n";
    s += indentFunc(indent + 1) + var + " " + std::to_string(useCount) + "\n";
    s += exp->toSyntaxTree(indent + 1) + "\n";
    return s;
}

// ============ PRINT Statement Implementation ============
// PRINT: output expression result to console

// Constructor: create print statement
PrintStmt::PrintStmt(Expression *exp) : exp(exp) {}

// Destructor: clean up expression
PrintStmt::~PrintStmt() {
    delete exp;
}

// Execute: evaluate expression and print result
void PrintStmt::execute(EvalState &state, Program &) {
    execCount++;

    // Evaluate expression
    int value = exp->eval(state);

    // Output result (using Qt debug output for now)
    qDebug() << value;

    std::cout << value << std::endl;
}

// Syntax tree representation for PRINT
std::string PrintStmt::toSyntaxTree(const RuntimeStats *, int indent) const {
    std::string s = indentFunc(indent) + "PRINT " + std::to_string(execCount) + "\n";
    s += indentFunc(indent + 1) + exp->toSyntaxTree() + "\n";
    return s;
}

// ============ INPUT Statement Implementation ============
// INPUT: read integer from user input

// Constructor: create input statement for variable
InputStmt::InputStmt(const std::string &varName) : var(varName) {
    if (!Tokenizer::isValidIdentifier(varName)) {
        throw std::runtime_error("INVALID IDENTIFIER: " + varName);
    }
}

// Execute: read integer from input and store in variable
void InputStmt::execute(EvalState &state, Program &) {
    execCount++;

    // Check if input provider is configured
    if (!state.inputProvider) {
        if (state.outputConsumer) state.outputConsumer("No input provider set\n");
        throw std::runtime_error("Input provider not set");
    }

    // Loop until valid integer is entered
    while (true) {
        // Call input provider callback
        QString qinput = state.inputProvider();
        std::string temp = qinput.toStdString();

        try {
            // Parse as integer
            int value = std::stoi(temp);
            state.setValue(var, value);
            break;
        } catch (...) {
            // Invalid input, ask again
            if (state.outputConsumer) state.outputConsumer("INVALID NUMBER\n");
        }
    }
}

// Syntax tree representation for INPUT
std::string InputStmt::toSyntaxTree(const RuntimeStats *, int indent) const {
    std::string s = "INPUT " + var;
    s += "\n";
    return s;
}

// ============ GOTO Statement Implementation ============
// GOTO: unconditional branch to another line

// Constructor: create goto statement with target line
GotoStmt::GotoStmt(int targetLine) : target(targetLine) {}

// Execute: set next line to target (unconditional jump)
void GotoStmt::execute(EvalState &, Program &program) {
    execCount++;
    // Transfer control to target line
    program.setNextLine(target);
}

// Syntax tree representation for GOTO
std::string GotoStmt::toSyntaxTree(const RuntimeStats *, const int indent) const {
    std::string s;
    s += indentFunc(indent) + "GOTO " + std::to_string(execCount) + "\n";
    s += indentFunc(indent + 1) + std::to_string(target) + "\n";
    return s;
}

// Constructor: create IF statement
IfStmt::IfStmt(Expression *lhs, const std::string &op,
               Expression *rhs, int targetLine)
    : left(lhs), right(rhs), op(op), target(targetLine) {}

// Destructor: clean up expressions
IfStmt::~IfStmt() {
    delete left;
    delete right;
}

// Execute: evaluate condition and jump to target if true
void IfStmt::execute(EvalState &state, Program &program) {
    // Evaluate both sides
    int l = left->eval(state);
    int r = right->eval(state);

    execCount++;

    // Evaluate condition based on operator
    bool cond = false;
    if (op == "=") cond = (l == r);
    else if (op == ">") cond = (l > r);
    else if (op == "<") cond = (l < r);

    // If condition true, jump to target line
    if (cond) {
        thenCount++;
        program.setNextLine(target);
    } else {
        ifCount++;
    }
}

// Syntax tree representation for IF
std::string IfStmt::toSyntaxTree(const RuntimeStats * stats, int indent) const {
    std::string s;
    s += indentFunc(indent) + "IF THEN " + std::to_string(ifCount) + " " + std::to_string(thenCount) + "\n";
    s += left->toSyntaxTree(indent + 1) + "\n";
    s += indentFunc(indent + 1) + op + "\n";
    s += right->toSyntaxTree(indent + 1) + "\n";
    s += indentFunc(indent + 1) + std::to_string(target) + "\n";
    return s;
}

// ============ END Statement Implementation ============
// END: terminate program execution

// Execute: mark program as ended
void EndStmt::execute(EvalState &, Program &program) {
    // Set program end flag
    program.setEnd();
}

// Syntax tree representation for END
std::string EndStmt::toSyntaxTree(const RuntimeStats *, int indent) const {
    std::string s;
    s+=indentFunc(indent) +"END" + "\n";
    return s;
}
