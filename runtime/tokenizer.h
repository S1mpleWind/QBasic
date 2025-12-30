/**
 * @file    tokenizer.h
 * @brief   Lexical analyzer that converts source code into tokens
 *          Responsible for breaking input lines into meaningful lexical units
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include <string>
#include <vector>
#include "token.h"

// Tokenizer: Lexical analysis class
// Converts input source code string into a sequence of tokens
class Tokenizer {
public:
    // Constructor: initialize tokenizer with source code line
    explicit Tokenizer(const std::string &line);

    // Get next token and advance cursor (consume token)
    Token getNextToken();
    
    // Peek at next token without consuming (lookahead without consuming)
    Token peekToken();

    // Reset tokenizer cursor to beginning of input
    void reset();
    
    // Check if there are more tokens to process
    bool hasMoreToken();

    // Static helper: check if a string is a valid C++ style identifier
    static bool isValidIdentifier(const std::string &name);

private:
    std::string src;        // Source code string being tokenized
    int pos;                // Current position/cursor in source string

    // Helper method: get current character without consuming
    char current() const;
    
    // Helper method: get current character and advance cursor (consume character)
    char get();
    
    // Helper method: skip whitespace characters
    void skipSpaces();

    // Parse numeric literal and return NUMBER token
    Token readNumber();
    
    // Parse identifier or keyword and return appropriate token
    Token readIdentifier();
    
    // Parse operator and return OPERATOR token
    Token readOperator();
};
