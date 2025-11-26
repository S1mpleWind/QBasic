/**
 * @file    statement.h
 * @brief   this file is responsible for statements
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
//class Program; //forward declararion



/**
    This is the base class
*/
/*
 * Class Statement
 * ----------------------------------------------------
 * Base class of all BASIC statements.
 * Each statement should implement its execute() method,
 * describing the runtime behavior.
 */
class Statement {
public:
    virtual ~Statement() {}
    virtual void execute(EvalState &state, Program &program) = 0;

    virtual std::string toString() const = 0;
};


/*
 * REM Statement
 * ----------------------------------------------------
 * Stores a comment line. No runtime behavior.
 */
class RemStmt : public Statement {
public:
    RemStmt(const std::string &text);
    void execute(EvalState &state, Program &program) override;

    std::string toString() const override {
        return "REM " + text;
    }

private:
    std::string text;
};


/*
 * LET Statement
 * ----------------------------------------------------
 * LET <var> = <expression>
 */
class LetStmt : public Statement {
public:
    LetStmt(const std::string &varName, Expression *exp);
    ~LetStmt();
    void execute(EvalState &state, Program &program) override;

    std::string toString() const override {
        return "LET " + var + " = " + exp->toString();
    }

private:
    std::string var;
    Expression *exp;
};


/*
 * PRINT Statement
 * ----------------------------------------------------
 * PRINT <expression>
 */
class PrintStmt : public Statement {
public:
    PrintStmt(Expression *exp);
    ~PrintStmt();
    void execute(EvalState &state, Program &program) override;

    std::string toString() const override {
        return "PRINT " + exp->toString();
    }


private:
    Expression *exp;
};


/*
 * INPUT Statement
 * ----------------------------------------------------
 * INPUT <var>
 * Reads an integer from user input.
 */
class InputStmt : public Statement {
public:
    InputStmt(const std::string &varName);
    void execute(EvalState &state, Program &program) override;

    std::string toString() const override {
        return "INPUT " + var;
    }

private:
    std::string var;
};


/*
 * GOTO Statement
 * ----------------------------------------------------
 * GOTO <line>
 * Changes program counter to another line.
 */
class GotoStmt : public Statement {
public:
    GotoStmt(int targetLine);
    void execute(EvalState &state, Program &program) override;

    std::string toString() const override {
        return "GOTO " + std::to_string(target);
    }

private:
    int target;
};


/*
 * IF Statement
 * ----------------------------------------------------
 * IF <exp1> <op> <exp2> THEN <line>
 */
class IfStmt : public Statement {
public:
    IfStmt(Expression *lhs, const std::string &op, Expression *rhs, int targetLine);
    ~IfStmt();
    void execute(EvalState &state, Program &program) override;

    std::string toString() const override {
        return "IF " + left->toString() + " " + op + " " + right->toString()
        + " THEN " + std::to_string(target);
    }

private:
    Expression *left;
    Expression *right;
    std::string op;
    int target;
};


/*
 * END Statement
 * ----------------------------------------------------
 * END
 * Stops the execution loop.
 */
class EndStmt : public Statement {
public:
    EndStmt() {}
    void execute(EvalState &state, Program &program) override;
};
