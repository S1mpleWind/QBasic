/**
 * @file    tokenizer.h
 * @brief   this file is responsible for Lexing/Tokenizing
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include <string>
#include <vector>
#include "token.h"

class Tokenizer {
public:
    explicit Tokenizer(const std::string &line);

    Token getNextToken();      // fetch one token (read and consume)
    Token peekToken();         // look into the next token & go back (not consume)

    void reset();              // reset the cursor
    bool hasMoreToken();

private:
    std::string src;
    int pos;                   // current index


    char current() const;
    char get();                // fetch the char and move to the next one (consume)
    void skipSpaces();

    Token readNumber();
    Token readIdentifier();
    Token readOperator();
};
