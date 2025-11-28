/**
 * @file    parser.h
 * @brief   this file is responsible for parser,
 *          which is the most important one
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once
#include <string>
#include "tokenizer.h"
#include "statement.h"
#include "program.h"
#include "evalstate.h"

class Parser {
public:
    Parser() = default;

    // 解析一行源码，返回 Statement 对象
    // lineNum：行号
    // line：源码文本
    Statement* parseLine(int lineNum, const std::string &line);

private:

    Statement* parseLet(Tokenizer &tokenizer);
    Statement* parsePrint(Tokenizer &tokenizer);
    Statement* parseInput(Tokenizer &tokenizer);
    Statement* parseGoto(Tokenizer &tokenizer);
    Statement* parseIf(Tokenizer &tokenizer);
    Statement* parseRem(Tokenizer &tokenizer);

    Statement* parseEnd(Tokenizer &tokenizer);
    /*  To parse a expression
     *  most important, using recusion here to construct the syntax tree
     *  here providing 3 functions to recursively build the expression tree
     */
    Expression* parseExpression(Tokenizer &tokenizer); // "+/-" , the general interface
    Expression* parseTerm      (Tokenizer &tokenizer); // "*,/,mod"
    Expression* parsePower     (Tokenizer &tokenizer); // "**"
    Expression* parseFactor    (Tokenizer &tokenizer); // fundementals
};
