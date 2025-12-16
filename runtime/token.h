#ifndef TOKEN_H
#define TOKEN_H

#endif // TOKEN_H
#include <string>

// Token types for lexical analysis - classifies tokens recognized by tokenizer
enum class TokenType {
    NUMBER,         // Numeric literals (e.g., 42, 3.14)
    IDENTIFIER,     // Variable names
    KEYWORD,        // Reserved keywords (LET, PRINT, IF, THEN, GOTO, INPUT, REM, END, etc.)
    OPERATOR,       // Operators (+ - * / = < > MOD **)
    STRING,         // String literals (optional feature)
    END_OF_LINE,    // End of line marker
    INVALID         // Invalid/unknown token
};

// Token structure: represents a single lexical element
// Contains the token type and its raw text representation
struct Token {
    TokenType type;         // Classification of token (NUMBER, IDENTIFIER, etc.)
    std::string text;       // Raw text content of the token

    // Constructor for creating Token objects with default invalid token
    Token(TokenType t = TokenType::INVALID, const std::string &s = "")
        : type(t), text(s) {}
};
