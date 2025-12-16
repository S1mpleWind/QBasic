/**
 * @file    statement.h
 * @brief   Statement classes for BASIC language execution
 *          Represents different BASIC statement types (LET, PRINT, GOTO, IF, etc.)
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include <QObject>
#include "../runtime/evalstate.h"
#include "exp.h"
#include "program.h"

// Abstract base class for all BASIC statements
// Each subclass implements execute() to define runtime behavior
class Statement {
public:
    virtual ~Statement() {}
    
    // Execute statement: perform runtime action with given state and program
    virtual void execute(EvalState &state, Program &program) = 0;

    // Convert statement to text representation
    virtual std::string toString() const = 0;

    // Convert statement to syntax tree visualization
    virtual std::string toSyntaxTree(const RuntimeStats * stats, int indent = 0) const = 0;

protected:
    int execCount = 0;  // Track execution count for debugging

    // Helper method to generate indentation for tree visualization
    std::string indentFunc(int n) const {
        return std::string(n * 2, ' ');
    }
};

// REM Statement: Comment line
// No runtime behavior, purely for documentation
class RemStmt : public Statement {
public:
    // Constructor: store comment text
    RemStmt(const std::string &text);
    
    // Execute: comment statements do nothing
    void execute(EvalState &state, Program &program) override;

    // String representation
    std::string toString() const override {
        return "REM " + text;
    }

    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;

private:
    std::string text;   // Comment text
};

// LET Statement: Variable assignment
// LET <variable> = <expression>
class LetStmt : public Statement {
public:
    // Constructor: create assignment statement
    LetStmt(const std::string &varName, Expression *exp);
    
    // Destructor: clean up expression
    ~LetStmt();
    
    // Execute: evaluate expression and assign to variable
    void execute(EvalState &state, Program &program) override;

    // String representation
    std::string toString() const override {
        return "LET " + var + " = " + exp->toString();
    }

    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;

private:
    std::string var;    // Variable name
    Expression *exp;    // Right-hand side expression
};

// PRINT Statement: Output to console
// PRINT <expression>
class PrintStmt : public Statement {
public:
    // Constructor: create print statement with expression
    PrintStmt(Expression *exp);
    
    // Destructor: clean up expression
    ~PrintStmt();
    
    // Execute: evaluate expression and output result
    void execute(EvalState &state, Program &program) override;

    // String representation
    std::string toString() const override {
        return "PRINT " + exp->toString();
    }

    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;

private:
    Expression *exp;    // Expression to print
};

// INPUT Statement: Read user input
// INPUT <variable>
// Reads a single integer from user input and stores in variable
class InputStmt : public Statement {
public:
    // Constructor: create input statement for variable
    InputStmt(const std::string &varName);
    
    // Execute: read integer from input and assign to variable
    void execute(EvalState &state, Program &program) override;

    // String representation
    std::string toString() const override {
        return "INPUT " + var;
    }

    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;

private:
    std::string var;    // Target variable name
};

// GOTO Statement: Unconditional branch
// GOTO <line_number>
// Transfers control to specified line number
class GotoStmt : public Statement {
public:
    // Constructor: create goto statement with target line
    GotoStmt(int targetLine);
    
    // Execute: set next line to target line
    void execute(EvalState &state, Program &program) override;

    // String representation
    std::string toString() const override {
        return "GOTO " + std::to_string(target);
    }

    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;

private:
    int target;         // Target line number
};

// IF Statement: Conditional branch
// IF <expression1> <relational_op> <expression2> THEN <line_number>
// Jumps to line if condition is true
class IfStmt : public Statement {
public:
    // Constructor: create conditional statement
    IfStmt(Expression *lhs, const std::string &op, Expression *rhs, int targetLine);
    
    // Destructor: clean up expressions
    ~IfStmt();
    
    // Execute: evaluate condition, jump to target if true
    void execute(EvalState &state, Program &program) override;

    // String representation
    std::string toString() const override {
        return "IF " + left->toString() + " " + op + " " + right->toString()
        + " THEN " + std::to_string(target);
    }

    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;

private:
    Expression *left;   // Left-hand side expression
    Expression *right;  // Right-hand side expression
    std::string op;     // Relational operator (=, <, >, etc.)
    int target;         // Target line number if condition true

    int ifCount = 0;    // Debug counter
    int thenCount = 0;  // Debug counter
};

// END Statement: Program termination
// END
// Stops execution of the program
class EndStmt : public Statement {
public:
    // Constructor
    EndStmt() {}
    
    // Execute: mark program as ended
    void execute(EvalState &state, Program &program) override;
    
    // String representation
    std::string toString() const override {
        return "END";
    }
    
    // Syntax tree representation
    std::string toSyntaxTree(const RuntimeStats * stats, int indent) const override;
};
