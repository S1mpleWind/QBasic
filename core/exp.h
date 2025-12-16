/**
 * @file    exp.h
 * @brief   Expression classes representing and evaluating expressions in BASIC
 *          Forms the core of the expression AST (Abstract Syntax Tree)
 *          Supports constants, identifiers (variables), and compound expressions
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include "../runtime/evalstate.h"

// Expression type enumeration for distinguishing expression subclasses
enum ExpressionType {
    CONSTANT,       // Literal numeric value
    IDENTIFIER,     // Variable reference
    COMPOUND        // Binary operation (e.g., a + b)
};

// Abstract base class for all expressions
// Allows polymorphic evaluation and conversion to string representation
class Expression {
public:
    virtual ~Expression() = default;

    // Evaluate expression using runtime state and return integer result
    virtual int eval(EvalState &state) = 0;

    // Convert expression to human-readable string (for debugging output)
    virtual std::string toString() const = 0;

    // Get type of expression (CONSTANT, IDENTIFIER, or COMPOUND)
    virtual ExpressionType type() = 0;

    // Convert expression to syntax tree representation (indented for visualization)
    virtual std::string toSyntaxTree(int indent = 0) const = 0;

    // Getter methods for convenience (throw if not applicable to this expression type)
    
    // Get constant value (only valid for ConstantExp)
    virtual int getConstantValue() {
        throw std::runtime_error("getConstantValue() not implemented for this expression type");
    }

    // Get identifier name (only valid for IdentifierExp)
    virtual std::string getIdentifierName() {
        throw std::runtime_error("getIdentifierName() not implemented for this expression type");
    }

    // Get operator string (only valid for CompoundExp)
    virtual std::string getOperator() {
        throw std::runtime_error("getOperator() not implemented for this expression type");
    }

    // Get left-hand side operand (only valid for CompoundExp)
    virtual Expression* getLHS() {
        throw std::runtime_error("getLHS() not implemented for this expression type");
    }

    // Get right-hand side operand (only valid for CompoundExp)
    virtual Expression* getRHS() {
        throw std::runtime_error("getRHS() not implemented for this expression type");
    }

protected:
    // Helper method to generate indentation for tree visualization
    std::string indentHelper(int n) const {
        return std::string(n * 2, ' ');
    }
};

// Constant expression: represents a literal numeric value
class ConstantExp : public Expression {
public:
    // Constructor: create constant expression with given value
    ConstantExp(int value);

    // Evaluate: returns the constant value regardless of state
    int eval(EvalState &state) override;
    
    // Convert to string representation
    std::string toString() const override;
    
    // Type is CONSTANT
    ExpressionType type() override;

    // Get the constant value
    int getConstantValue() override;

    // Syntax tree representation
    std::string toSyntaxTree(int) const override;

private:
    int value;          // Numeric value of the constant
};

// Identifier expression: represents a variable reference
class IdentifierExp : public Expression {
public:
    // Constructor: create identifier expression with variable name
    IdentifierExp(const std::string &name);

    // Evaluate: look up variable value in EvalState
    int eval(EvalState &state) override;
    
    // Convert to string representation
    std::string toString() const override;

    // Type is IDENTIFIER
    ExpressionType type() override;

    // Get the identifier name
    std::string getIdentifierName() override;

    // Syntax tree representation
    std::string toSyntaxTree(int) const override;

private:
    std::string name;   // Variable name
};

// Compound expression: represents a binary operation (e.g., a + b, x * 2)
class CompoundExp : public Expression {
public:
    // Constructor: create binary expression with operator and operands
    CompoundExp(const std::string &op, Expression *lhs, Expression *rhs);

    // Destructor: clean up operand expressions
    ~CompoundExp();

    // Evaluate: recursively evaluate operands and apply operator
    int eval(EvalState &state) override;
    
    // Convert to string representation
    std::string toString() const override;

    // Type is COMPOUND
    ExpressionType type() override;

    // Get the operator
    std::string getOperator() override;
    
    // Get left operand
    Expression* getLHS() override;
    
    // Get right operand
    Expression* getRHS() override;

    // Syntax tree representation
    std::string toSyntaxTree(int) const override;

private:
    std::string op;         // Operator (e.g., "+", "-", "*", "/", "MOD", "**", "=", "<", ">")
    Expression *lhs;        // Left-hand side operand
    Expression *rhs;        // Right-hand side operand
};

