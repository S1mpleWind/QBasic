#pragma once

#include <cassert>
#include <iostream>
#include "statement.h"
#include "program.h"
#include "exp.h"
#include "evalstate.h"



using namespace std;

void testLetStmt() {
    EvalState state;
    Program prog;

    LetStmt stmt("X", new ConstantExp(42));
    stmt.execute(state, prog);

    cout<<stmt.toString()<<endl;

    assert(state.getValue("X") == 42);
    cout << "[PASS] testLetStmt" << endl;
}

void testPrintStmt() {
    EvalState state;
    Program prog;

    LetStmt let("X", new ConstantExp(99));
    let.execute(state, prog);

    Statement* print = new PrintStmt(new IdentifierExp("X"));
    print->execute(state, prog); // 输出 99

    cout << "[PASS] testPrintStmt" << endl;

    delete print;
}

void runStatementTests() {
    testLetStmt();
    testPrintStmt();
    cout << "All Statement tests passed!" << endl;
}


