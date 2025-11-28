// statement.cpp
#include "statement.h"
#include <iostream>

#include <QDebug>
/* ============================
 * REM Statement
 * ============================ */
RemStmt::RemStmt(const std::string &text) : text(text) {}

void RemStmt::execute(EvalState &state, Program &) {
    // No effect during runtime
}

std::string RemStmt::toSyntaxTree(const RuntimeStats * ,int indent)const{
    std::string s;
    s+= indentFunc(indent) + "REM " + text;
    return s;
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

std::string LetStmt::toSyntaxTree(const RuntimeStats * stats , int indent) const {
    std::string s;
    int useCount = 0;
    if (stats) {
        auto it = stats->identifierUseCount.find(var);
        if (it != stats->identifierUseCount.end()) useCount = it->second;
    }
    s += indentFunc(indent) + "LET = " + std::to_string(execCount) + "\n";
    //TODO the time of var
    s += indentFunc(indent+1) + var + " " + std::to_string(useCount) + "\n";
    s += exp->toSyntaxTree(indent + 1) + "\n";
    return s;

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

    qDebug()<< value;

    std::cout << value << std::endl;
}

std::string PrintStmt::toSyntaxTree(const RuntimeStats * ,int indent) const{
    std::string s = indentFunc(indent) + "PRINT " + std::to_string(execCount) + "\n";
    s+= indentFunc(indent+1) + exp->toSyntaxTree() + "\n";
    return s;
}

/* ============================
 * INPUT Statement
 * ============================ */
InputStmt::InputStmt(const std::string &varName) : var(varName) {}

void InputStmt::execute(EvalState &state, Program &) {
    execCount++;

    if (!state.inputProvider) {
        // 没有提供输入回调，抛异常或输出错误
        if (state.outputConsumer) state.outputConsumer("No input provider set\n");
        throw std::runtime_error("Input provider not set");
    }

    while (true) {
        QString qinput = state.inputProvider(); // 这会在 worker 线程被 blocking 调用到主线程
        std::string temp = qinput.toStdString();

        try {
            int value = std::stoi(temp);
            state.setValue(var, value);
            break;
        } catch (...) {
            if (state.outputConsumer) state.outputConsumer("INVALID NUMBER\n");
            // loop 再次请求
        }
    }
}


std::string InputStmt::toSyntaxTree(const RuntimeStats * ,int indent) const{
    std::string s = "INPUT " + var;
    s+="\n";
    return s;
}


/* ============================
 * GOTO Statement
 * ============================ */
GotoStmt::GotoStmt(int targetLine) : target(targetLine) {}

void GotoStmt::execute(EvalState &, Program &program) {
    // Change program count

    execCount++;

    program.setNextLine(target); // check is set in program
}

std::string GotoStmt::toSyntaxTree(const RuntimeStats * ,const int indent) const{
    std::string s;
    s += indentFunc(indent) + "GOTO " + std::to_string(execCount) + "\n";
    s += indentFunc(indent + 1) +std::to_string(target) + "\n";
    return s;
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

    // NOTE: here nextline equals to currentline

    if (cond) {

        // TODO: syntax tree part? to be deleted
        thenCount++;
        program.setNextLine(target);
    }

    else{
        ifCount++;
    }
}

std::string IfStmt::toSyntaxTree(const RuntimeStats * stats , int indent) const {
    std::string s;
    s += indentFunc(indent) + "IF THEN " + std::to_string(ifCount) + " " + std::to_string(thenCount) + "\n";
    s += left->toSyntaxTree(indent + 1) + "\n";
    s += indentFunc(indent+1) + op +"\n";
    s += right->toSyntaxTree(indent + 1) + "\n";
    s += indentFunc(indent+1) + std::to_string(target) + "\n";
    return s;
}

/* ============================
 * END Statement
 * ============================ */
void EndStmt::execute(EvalState &, Program &program) {

    program.setEnd();
    //rogram.setNextLine(-1);
}

std::string EndStmt :: toSyntaxTree(const RuntimeStats * , int indent) const{
    std::string s ;
    s+=indentFunc(indent) +"END" + "\n";
    return s;
}
