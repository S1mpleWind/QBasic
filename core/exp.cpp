#include "exp.h"
#include "../runtime/evalstate.h"
#include <stdexcept>
#include <sstream>

// -------- ConstantExp --------

ConstantExp::ConstantExp(int v) : value(v) {}

int ConstantExp::eval(EvalState &state) {
    return value;
}

std::string ConstantExp::toString() const {
    return std::to_string(value);
}


// -------- IdentifierExp --------

IdentifierExp::IdentifierExp(const std::string &n) : name(n) {}

int IdentifierExp::eval(EvalState &state) {
    // If variable not defined, throw error（变量未定义时报错）
    if (!state.isDefined(name)) {
        throw std::runtime_error("VARIABLE NOT DEFINED: " + name);
    }
    return state.getValue(name);
}

std::string IdentifierExp::toString() const {
    return name;
}


// -------- CompoundExp --------

CompoundExp::CompoundExp(const std::string &o, Expression *l, Expression *r)
    : op(o), lhs(l), rhs(r) {}

CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

int CompoundExp::eval(EvalState &state) {
    int left = lhs->eval(state);
    int right = rhs->eval(state);

    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) {
            throw std::runtime_error("DIVIDE BY ZERO");
        }
        return left / right;
    }

    throw std::runtime_error("UNKNOWN OPERATOR: " + op);
}

std::string CompoundExp::toString() const {
    return "(" + lhs->toString() + " " + op + " " + rhs->toString() + ")";
}
