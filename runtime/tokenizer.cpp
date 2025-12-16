#include "tokenizer.h"
#include <cctype>
//#include <iostream>

// Constructor: initialize tokenizer with source code line
Tokenizer::Tokenizer(const std::string &line)
    : src(line), pos(0) {}

// Reset tokenizer to beginning of source
void Tokenizer::reset() {
    pos = 0;
}

// Check if there are more tokens to process
bool Tokenizer::hasMoreToken() {
    skipSpaces();
    return pos < (int)src.size();
}

// Get current character without consuming
char Tokenizer::current() const {
    if (pos >= (int)src.size()) return '\0';
    return src[pos];
}

// Peek at next token without consuming
Token Tokenizer::peekToken() {
    int origin = pos;
    Token t = getNextToken();
    pos = origin;
    return t;
}

// Get and consume current character
char Tokenizer::get() {
    if (pos >= (int)src.size()) return '\0';
    return src[pos++];
}

// Skip whitespace characters
void Tokenizer::skipSpaces() {
    // Skip all consecutive whitespace
    while (std::isspace(current())) get();
}

// Main entry point: get next token from input
Token Tokenizer::getNextToken() {
    skipSpaces();
    char c = current();
    
    // End of input
    if (c == '\0') return Token(TokenType::END_OF_LINE, "");

    // Numeric literal
    if (std::isdigit(c)) return readNumber();

    // Identifier or keyword
    if (std::isalpha(c)) return readIdentifier();

    // Operator
    return readOperator();
}

// Parse numeric literal (integer)
Token Tokenizer::readNumber() {
    std::string result;
    while (std::isdigit(current())) result += get();
    return Token(TokenType::NUMBER, result);
}

// Parse identifier or keyword
Token Tokenizer::readIdentifier() {
    std::string result;
    while (std::isalnum(current())) result += get();

    // Convert to uppercase for keyword matching
    std::string upper;
    for (char c : result) upper += std::toupper(c);

    // Recognize BASIC keywords
    if (upper == "LET" || upper == "PRINT" || upper == "INPUT" ||
        upper == "GOTO" || upper == "IF" || upper == "THEN" || 
        upper == "END" || upper == "REM") {
        return Token(TokenType::KEYWORD, upper);
    }

    // MOD is an operator, not an identifier
    else if (upper == "MOD") return Token(TokenType::OPERATOR, upper);

    // Regular identifier (variable name)
    return Token(TokenType::IDENTIFIER, result);
}

// Parse operator token
Token Tokenizer::readOperator() {
    char c = get();
    std::string op(1, c);

    // Handle multi-character operators (**, <=, >=, <>)
    char next = current();

    // Power operator: **
    if (c == '*' && next == '*') op += next;
    
    // Comparison operators: <=, >=, <>
    if ((c == '<' || c == '>' || c == '=') &&
        (next == '=' || next == '>')) {
        op += get();
    }


    return Token(TokenType::OPERATOR, op);
}
