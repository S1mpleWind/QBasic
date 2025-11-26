#pragma once

#include <cassert>
#include <iostream>
#include "exp.h"
#include "evalstate.h"


using namespace std;

// 测试 ConstantExp
void testConstantExp() {
    EvalState state;
    Expression* c = new ConstantExp(100);

    assert(c->eval(state) == 100);
    assert(c->type() == ExpressionType::CONSTANT);

    cout << "[PASS] testConstantExp" << endl;
    delete c;
}

// 测试 IdentifierExp
void testIdentifierExp() {
    EvalState state;
    state.setValue("X", 42);

    Expression* id = new IdentifierExp("X");
    assert(id->eval(state) == 42);
    assert(id->getIdentifierName() == "X");

    cout << "[PASS] testIdentifierExp" << endl;
    delete id;
}

// 测试 CompoundExp
void testCompoundExp() {
    EvalState state;
    state.setValue("A", 10);
    state.setValue("B", 5);

    Expression* exp =
        new CompoundExp("+", new IdentifierExp("A"), new IdentifierExp("B"));

    assert(exp->eval(state) == 15);
    assert(exp->getOperator() == "+");

    cout << "[PASS] testCompoundExp" << endl;

    delete exp;
}

// 暴露给 main 调用
void runExpressionTests() {
    testConstantExp();
    testIdentifierExp();
    testCompoundExp();
    cout << "All Expression tests passed!" << endl;
}


