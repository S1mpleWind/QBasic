/**
 * @file    program.h
 * @brief   this is head file is responsible for the storage and sequence of the
 *          program's raw text
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include "../runtime/evalstate.h"
// Base class of Expression（表达式的基类）

// used to differentiate the type of subclasses
enum ExpressionType {CONSTANT, IDENTIFIER, COMPOUND};

class Expression {
public:
    virtual ~Expression() = default;

    // eval(EvalState&) — Evaluate expression with runtime state（使用运行时状态计算表达式）
    virtual int eval(EvalState &state) = 0;

    // toString() — Convert expression to text（表达式转为字符串，用于调试）
    virtual std::string toString() const = 0;

    virtual ExpressionType type() = 0;


    //syntax tree
    virtual std::string toSyntaxTree(int indent =0) const = 0;


    /* Getter mothods for convenience
     */

    virtual int getConstantValue() {
        throw std::runtime_error("getConstantValue() not implemented for this expression type");
    }

    virtual std::string getIdentifierName() {
        throw std::runtime_error("getIdentifierName() not implemented for this expression type");
    }

    virtual std::string getOperator() {
        throw std::runtime_error("getOperator() not implemented for this expression type");
    }

    virtual Expression* getLHS() {
        throw std::runtime_error("getLHS() not implemented for this expression type");
    }

    virtual Expression* getRHS() {
        throw std::runtime_error("getRHS() not implemented for this expression type");
    }

protected:
    std::string indentHelper (int n) const{
        return std::string( n*2 ,' ');
    }
};


// ConstantExp（常量表达式）
class ConstantExp : public Expression {
public:
    ConstantExp(int value);

    int eval(EvalState &state) override;
    std::string toString() const override;
    ExpressionType type() override;

    int getConstantValue() override;

    std::string toSyntaxTree(int) const override;

private:
    int value;
};


// IdentifierExp（标识符表达式，即变量名）
class IdentifierExp : public Expression {
public:
    IdentifierExp(const std::string &name);

    int eval(EvalState &state) override;
    std::string toString() const override;

    ExpressionType type() override;

    std::string getIdentifierName() override;

    int getCount() {return useCount;}

    std::string toSyntaxTree(int) const override;

private:
    std::string name;
    int useCount = 0;
};


// CompoundExp（二元表达式，如 a+b, x-3）
class CompoundExp : public Expression {
public:
    CompoundExp(const std::string &op, Expression *lhs, Expression *rhs);

    ~CompoundExp();

    int eval(EvalState &state) override;
    std::string toString() const override;

    ExpressionType type() override;

    std::string getOperator() override;
    Expression* getLHS() override;
    Expression* getRHS() override;

    std::string toSyntaxTree(int) const override;

private:
    std::string op;         // Operator — 运算符
    Expression *lhs;        // Left-hand side — 左操作数
    Expression *rhs;        // Right-hand side — 右操作数
};

