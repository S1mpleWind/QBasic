#ifndef TOKEN_H
#define TOKEN_H

#endif // TOKEN_H
#include <string>

enum class TokenType {
    NUMBER,
    IDENTIFIER,
    KEYWORD,        // LET / PRINT / IF / THEN
    OPERATOR,       // + - * / = < >
    STRING,         // optional
    END_OF_LINE,
    INVALID
};

struct Token {
    TokenType type;
    std::string text;   // raw text

    Token(TokenType t = TokenType::INVALID, const std::string &s = "")
        : type(t), text(s) {}
};
