/**
 * @file test_parser
 */

#pragma once



#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "parser.h"
#include "tokenizer.h"
#include "exp.h"

using std::cout;
using std::endl;

/* Helper: convert Expression AST into a string */
std::string toString(Expression* exp) {
    if (!exp) return "<null>";

    if (auto c = dynamic_cast<ConstantExp*>(exp)) {
        return std::to_string(c->getConstantValue());
    }
    if (auto id = dynamic_cast<IdentifierExp*>(exp)) {
        return id->getIdentifierName();
    }
    if (auto comp = dynamic_cast<CompoundExp*>(exp)) {
        return "(" + toString(comp->getLHS()) +
               " " + comp->getOperator() + " " +
               toString(comp->getRHS()) + ")";
    }
    return "<unknown>";
}

void test(const std::string &src) {
    Parser parser;

    //cout<<src;

    //cout<<"000000"<<endl;

    Statement* stmt = parser.parseLine(10, src);

    //cout << "1111"<<endl;

    cout << "INPUT : " << src << endl;
    cout << "OUTPUT: " << stmt->toString() << endl;
    cout << "---------------------------------" << endl;

    delete stmt;
}

int testParser() {
    //LET 测试
    test("LET X = 1 + 2 * 3");

    // PRINT 测试
    test("PRINT X + 5");

    // INPUT 测试
    test("INPUT A");

    // IF 测试
    test("IF X < 10 THEN 200");

    // GOTO 测试
    test("GOTO 120");

    //REM 测试

    //FIXME: no entering? fixed , REM missing in tokenizer
    test("REM this is a comment");

    return 0;
}
