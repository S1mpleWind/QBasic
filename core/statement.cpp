// statement.cpp
#include "statement.h"
#include <iostream>

/* ============================
 * REM Statement
 * ============================ */
RemStmt::RemStmt(const std::string &text) : text(text) {}

void RemStmt::execute(EvalState &, Program &) {
    // No effect during runtime
}


/* ============================
 * LET Statement
 * ============================ */
LetStmt::LetStmt(const std::string &varName, Expression *exp)
    : var(varName), exp(exp) {}

LetStmt::~LetStmt() {
    delete exp;
}

void LetStmt::execute(EvalState &state, Program &) {

    execCount++;

    int value = exp->eval(state);
    state.setValue(var, value);
}


/* ============================
 * PRINT Statement
 * ============================ */
PrintStmt::PrintStmt(Expression *exp) : exp(exp) {}

PrintStmt::~PrintStmt() {
    delete exp;
}

void PrintStmt::execute(EvalState &state, Program &) {
    //std::cout << "print execute"<< std::endl;

    execCount++;

    int value = exp->eval(state);
    std::cout << value << std::endl;
}


/* ============================
 * INPUT Statement
 * ============================ */
InputStmt::InputStmt(const std::string &varName) : var(varName) {}

void InputStmt::execute(EvalState &state, Program &) {
    int value;

    execCount++;

    while (true) {
        std::cout << " ? ";
        std::string temp;
        std::getline(std::cin, temp);

        try {
            value = std::stoi(temp);
            break;
        } catch (...) {
            std::cout << "INVALID NUMBER" << std::endl;
        }
    }

    state.setValue(var, value);
}


/* ============================
 * GOTO Statement
 * ============================ */
GotoStmt::GotoStmt(int targetLine) : target(targetLine) {}

void GotoStmt::execute(EvalState &, Program &program) {
    // Change program count
    program.setNextLine(target); // check is set in program
}


/* ============================
 * IF Statement
 * ============================ */
IfStmt::IfStmt(Expression *lhs, const std::string &op,
               Expression *rhs, int targetLine)
    : left(lhs), right(rhs), op(op), target(targetLine) {}

IfStmt::~IfStmt() {
    delete left;
    delete right;
}

void IfStmt::execute(EvalState &state, Program &program) {
    int l = left->eval(state);
    int r = right->eval(state);

    execCount++;

    bool cond = false;
    if (op == "=") cond = (l == r);
    else if (op == ">") cond = (l > r);
    else if (op == "<") cond = (l < r);

    if (cond) {

        // TODO: syntax tree part
        thenCount++;
        program.setNextLine(target);
    }

    else{
        ifCount++;
    }
}


/* ============================
 * END Statement
 * ============================ */
void EndStmt::execute(EvalState &, Program &program) {
    program.setEnd();
    //rogram.setNextLine(-1);
}
