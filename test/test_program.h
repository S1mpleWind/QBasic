#pragma once

#include <cassert>
#include <iostream>
#include "program.h"
#include "statement.h"
#include "exp.h"


using namespace std;

void testAddAndGetSourceLine() {
    Program prog;
    prog.addSourceLine(10, "LET X = 5");
    prog.addSourceLine(20, "PRINT X");

    assert(prog.getSourceLine(10) == "LET X = 5");
    assert(prog.getSourceLine(20) == "PRINT X");

    cout << "[PASS] testAddAndGetSourceLine" << endl;
}

void testRemoveSourceLine() {
    Program prog;
    prog.addSourceLine(10, "LET X = 5");
    prog.removeSourceLine(10);

    assert(prog.getSourceLine(10) == "");
    cout << "[PASS] testRemoveSourceLine" << endl;
}

void testParsedStatement() {
    Program prog;
    Statement* stmt = new LetStmt("X", new ConstantExp(42));
    prog.setParsedStatement(10, stmt);

    assert(prog.getParsedStatement(10) == stmt);
    cout << "[PASS] testParsedStatement" << endl;

    //delete stmt;
}

void testLineNavigation() {
    Program prog;

    prog.addSourceLine(10, "LET X = 5");

    prog.addSourceLine(20, "PRINT X");

    prog.addSourceLine(30, "END");


    assert(prog.getFirstLineNumber() == 10);
    assert(prog.getNextLineNumber(10) == 20);
    assert(prog.getNextLineNumber(20) == 30);
    assert(prog.getNextLineNumber(30) == -1);

    cout << "[PASS] testLineNavigation" << endl;
}

void testExecutionControl() {

    Program prog;
    prog.setNextLine(10);
    assert(prog.getNextLine() == 10);

    prog.setEnd();
    assert(prog.isEnded());

    cout << "[PASS] testExecutionControl" << endl;
}

void testGetDisplayText() {
    Program prog;
    prog.addSourceLine(10, "LET X = 5");
    prog.addSourceLine(20, "PRINT X");

    string display = prog.getDisplayText();
    assert(display.find("10") != string::npos);
    assert(display.find("20") != string::npos);

    cout << "[PASS] testGetDisplayText" << endl;
}

void runProgramTests() {
    testAddAndGetSourceLine();
    testRemoveSourceLine();
    testParsedStatement();
    testLineNavigation();
    testExecutionControl();
    testGetDisplayText();

    cout << "All Program tests passed!" << endl;
}


