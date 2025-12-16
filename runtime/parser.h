/**
 * @file    parser.h
 * @brief   Parser for BASIC language syntax
 *          Converts tokens into abstract syntax tree (AST) of statements and expressions
 *          This is the most important component for language processing
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

// Parser: Syntax analysis class
// Parses tokenized input into statement and expression objects forming the AST
class Parser {
public:
    Parser() = default;

    // Parse one line of source code and return corresponding Statement object
    // lineNum: line number in program (for GOTO support)
    // line: raw source code text to parse
    // Returns: pointer to parsed Statement, nullptr if parse fails
    Statement* parseLine(int lineNum, const std::string &line);

private:
    // Parse statements based on keyword
    
    // Parse LET statement (variable assignment)
    Statement* parseLet(Tokenizer &tokenizer);
    
    // Parse PRINT statement (output to console)
    Statement* parsePrint(Tokenizer &tokenizer);
    
    // Parse INPUT statement (read from input)
    Statement* parseInput(Tokenizer &tokenizer);
    
    // Parse GOTO statement (unconditional branch)
    Statement* parseGoto(Tokenizer &tokenizer);
    
    // Parse IF/THEN statement (conditional execution)
    Statement* parseIf(Tokenizer &tokenizer);
    
    // Parse REM statement (comment)
    Statement* parseRem(Tokenizer &tokenizer);

    // Parse END statement (program termination)
    Statement* parseEnd(Tokenizer &tokenizer);
    
    // Expression parsing using recursive descent
    // These functions implement operator precedence through recursion
    // Precedence levels (high to low): Factor -> Power -> Term -> Expression
    
    // Parse expression: handles +/- operators (lowest precedence)
    Expression* parseExpression(Tokenizer &tokenizer);
    
    // Parse term: handles */ operators (middle precedence)
    Expression* parseTerm(Tokenizer &tokenizer);
    
    // Parse power: handles ** operator (high precedence)
    Expression* parsePower(Tokenizer &tokenizer);
    
    // Parse factor: handles primary expressions, parentheses (highest precedence)
    Expression* parseFactor(Tokenizer &tokenizer);
};
