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

char Tokenizer::current()const{
    if (pos >= (int)src.size()) return '\0';
    return src[pos];
}

Token Tokenizer::peekToken() {
    int origin = pos;
    Token t = getNextToken();
    pos = origin;
    return t;
}

char Tokenizer::get() {
    if (pos >= (int)src.size()) return '\0';
    return src[pos++];
}

void Tokenizer::skipSpaces() {
    //jump the extra spaces that might appear
    while (std::isspace(current())) get();
}

/*general call of getting the next token*/
Token Tokenizer::getNextToken() {
    skipSpaces();
    char c = current();
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
    while (std::isdigit(current())) result += get();
    return Token(TokenType::NUMBER, result);
}

Token Tokenizer::readIdentifier() {
    std::string result;
    while (std::isalnum(current())) result += get();

    // recognize the key word:LET, PRINT, INPUT, GOTO, IF, THEN, END
    std::string upper;
    for (char c : result) upper += std::toupper(c);

    if (upper == "LET" || upper == "PRINT" || upper == "INPUT" ||
        upper == "GOTO" || upper == "IF" || upper == "THEN" || upper == "END") {
        return Token(TokenType::KEYWORD, upper);
    }

    //NOTE : because 'mod' is alse a string , it is handled here
    else if (upper == "MOD") return Token(TokenType::OPERATOR, upper);

    return Token(TokenType::IDENTIFIER, result);
}

Token Tokenizer::readOperator() {
    char c = get();
    std::string op(1, c);

    // TODO:<= >= <> "MOD" "**"
    // NOTE: 'MOD' should handle the negative situations properly
    // NOTE: '<>' means not equal
    char next = current();

    if ( c == '*' && next == '*') op+=next;
    if ((c == '<' || c == '>' || c == '=') &&
        (next == '=' || next == '>')) {
        op += get();
    }


    return Token(TokenType::OPERATOR, op);
}
