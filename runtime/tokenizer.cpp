#include "tokenizer.h"
#include <cctype>
//#include <iostream>

Tokenizer::Tokenizer(const std::string &line)
    : src(line), pos(0) {}

void Tokenizer::reset() {
    pos = 0;
}

bool Tokenizer::hasMoreToken()  {
    //Found by unit test
    //std::cout<<pos;
    skipSpaces();
    //std::cout<<pos;
    return pos < (int)src.size();
}

char Tokenizer::peek() const {
    if (pos >= (int)src.size()) return '\0';
    return src[pos];
}

char Tokenizer::get() {
    if (pos >= (int)src.size()) return '\0';
    return src[pos++];
}

void Tokenizer::skipSpaces() {
    //jump the extra spaces that might appear
    while (std::isspace(peek())) get();
}

/*general call of getting the next token*/
Token Tokenizer::getNextToken() {
    skipSpaces();
    char c = peek();
    if (c == '\0') return Token(TokenType::END_OF_LINE, "");

    // number
    if (std::isdigit(c)) return readNumber();

    // sign
    if (std::isalpha(c)) return readIdentifier();

    // operator
    return readOperator();
}

Token Tokenizer::readNumber() {
    std::string result;
    while (std::isdigit(peek())) result += get();
    return Token(TokenType::NUMBER, result);
}

Token Tokenizer::readIdentifier() {
    std::string result;
    while (std::isalnum(peek())) result += get();

    // recognize the key word:LET, PRINT, INPUT, GOTO, IF, THEN, END
    std::string upper;
    for (char c : result) upper += std::toupper(c);

    if (upper == "LET" || upper == "PRINT" || upper == "INPUT" ||
        upper == "GOTO" || upper == "IF" || upper == "THEN" || upper == "END") {
        return Token(TokenType::KEYWORD, upper);
    }

    return Token(TokenType::IDENTIFIER, result);
}

Token Tokenizer::readOperator() {
    char c = get();
    std::string op(1, c);

    // TODO:<= >= <>
    char next = peek();
    if ((c == '<' || c == '>' || c == '=') &&
        (next == '=' || next == '>')) {
        op += get();
    }

    return Token(TokenType::OPERATOR, op);
}
