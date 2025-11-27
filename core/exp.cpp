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

ExpressionType ConstantExp::type() {
    return ExpressionType::CONSTANT;
}

int ConstantExp::getConstantValue(){
    return value;
}




// -------- IdentifierExp --------

IdentifierExp::IdentifierExp(const std::string &n) : name(n) {}

int IdentifierExp::eval(EvalState &state) {
    // If variable not defined, throw error
    if (!state.isDefined(name)) {
        throw std::runtime_error("VARIABLE NOT DEFINED: " + name);
    }
    useCount++;
    return state.getValue(name);
}

std::string IdentifierExp::toString() const {
    return name;
}

ExpressionType IdentifierExp::type() {
    return ExpressionType::IDENTIFIER;
}


std::string IdentifierExp::getIdentifierName(){
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

    // recursion here
    int left = lhs->eval(state);
    int right = rhs->eval(state);

    // operation at the current stage
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) {
            throw std::runtime_error("DIVIDE BY ZERO");
        }
        return left / right;
    }

    //TODO: implement the operator "MOD" and "**"
    // NOTE: 'MOD' should handle the negative situations properly

    if (op == "**") return std::pow(left,right);

    if (op == "MOD") {
        if (right == 0) return 0;  // 避免除零

        int result = left % right;

        // adjust the sign
        if ((right > 0 && result < 0) || (right < 0 && result > 0)) {
            result += right;
        }

        return result;
    }

    throw std::runtime_error("UNKNOWN OPERATOR: " + op);
}

std::string CompoundExp::toString() const {
    return "(" + lhs->toString() + " " + op + " " + rhs->toString() + ")";
}

ExpressionType CompoundExp::type() {
    return ExpressionType::COMPOUND;
}

std::string CompoundExp::getOperator(){
    return op;
}

Expression* CompoundExp::getLHS(){
    return lhs;
}

Expression* CompoundExp::getRHS(){
    return rhs;
}
