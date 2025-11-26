#pragma once

#include <iostream>
#include <cassert>
#include "tokenizer.h"

void testTokenizer() {
    {
        std::string line = "x = 42 + y";
        Tokenizer tokenizer(line);

        Token t = tokenizer.getNextToken();
        assert(t.type == TokenType::IDENTIFIER && t.text == "x");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::OPERATOR && t.text == "=");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::NUMBER && t.text == "42");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::OPERATOR && t.text == "+");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::IDENTIFIER && t.text == "y");

        assert(!tokenizer.hasMoreToken());
    }

    {
        std::string line = "  sum123 - 5*var2 ";
        Tokenizer tokenizer(line);

        Token t = tokenizer.getNextToken();
        assert(t.type == TokenType::IDENTIFIER && t.text == "sum123");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::OPERATOR && t.text == "-");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::NUMBER && t.text == "5");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::OPERATOR && t.text == "*");

        t = tokenizer.getNextToken();
        assert(t.type == TokenType::IDENTIFIER && t.text == "var2");

        bool x = tokenizer.hasMoreToken();

        assert(!x);
    }

    {
        // test reset
        std::string line = "a+b";
        Tokenizer tokenizer(line);

        assert(tokenizer.hasMoreToken());
        tokenizer.getNextToken();
        tokenizer.reset();  // reset cursor
        Token t = tokenizer.getNextToken();
        assert(t.type == TokenType::IDENTIFIER && t.text == "a");
    }

    std::cout << "[PASS] testTokenizer" << std::endl;
}

