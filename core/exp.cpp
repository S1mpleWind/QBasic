#include "exp.h"
#include "../runtime/evalstate.h"
#include "../runtime/tokenizer.h"
#include <stdexcept>
#include <iostream>
#include <sstream>

// ============ ConstantExp Implementation ============

// Constructor: initialize with constant value
ConstantExp::ConstantExp(int v) : value(v) {}

// Evaluation: return the constant value
int ConstantExp::eval(EvalState &state) {
    return value;
}

// String representation: numeric value
std::string ConstantExp::toString() const {
    return std::to_string(value);
}

// Type: always CONSTANT
ExpressionType ConstantExp::type() {
    return ExpressionType::CONSTANT;
}

// Get constant value
int ConstantExp::getConstantValue(){
    return value;
}

// Syntax tree representation
std::string ConstantExp:: toSyntaxTree(int indent) const{
    return indentHelper(indent) + std::to_string(value);
}

// ============ IdentifierExp Implementation ============

// Constructor: initialize with variable name
IdentifierExp::IdentifierExp(const std::string &n) : name(n) {
    if (!Tokenizer::isValidIdentifier(n)) {
        throw std::runtime_error("INVALID IDENTIFIER: " + n);
    }
}

// Evaluation: look up variable value in symbol table
int IdentifierExp::eval(EvalState &state) {
    // Check if variable is defined
    if (!state.isDefined(name)) {
        throw std::runtime_error("VARIABLE NOT DEFINED: " + name);
    }

    // Track identifier usage in runtime statistics
    RuntimeStats *rs = state.getRuntimeStats();
    if (rs) {
        rs->identifierUseCount[name]++;  // Increment usage count
    } else {
        std::cout << "rs not initialized properly" << std::endl;
    }

    // Return variable value
    return state.getValue(name);
}

// String representation: variable name
std::string IdentifierExp::toString() const {
    return name;
}

// Type: always IDENTIFIER
ExpressionType IdentifierExp::type() {
    return ExpressionType::IDENTIFIER;
}

// Get identifier name
std::string IdentifierExp::getIdentifierName() {
    return name;
}

// Syntax tree representation
std::string IdentifierExp::toSyntaxTree(int indent) const {
    return indentHelper(indent) + name;
}

// ============ CompoundExp Implementation ============

// Constructor: create binary expression with operator and operands
CompoundExp::CompoundExp(const std::string &o, Expression *l, Expression *r)
    : op(o), lhs(l), rhs(r) {}

// Destructor: clean up operand expressions
CompoundExp::~CompoundExp() {
    delete lhs;
    delete rhs;
}

// Evaluation: recursively evaluate operands and apply operator
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


std::string CompoundExp::toSyntaxTree(int indent) const {
    std::string s;
    s += indentHelper(indent) + op + "\n";
    s += lhs->toSyntaxTree(indent + 1) + "\n";
    s += rhs->toSyntaxTree(indent + 1);
    return s;
}
