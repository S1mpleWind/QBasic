#include "parser.h"
#include <iostream>
#include <stdexcept>

Statement* Parser::parseLine(int lineNum, const std::string &line) {
    Tokenizer tokenizer(line);
    if (!tokenizer.hasMoreToken()) return nullptr;

    Token first = tokenizer.getNextToken();

    if (first.type == TokenType::KEYWORD) {
        if (first.text == "REM") {
            return parseRem(tokenizer);  // REM 处理
        }
        else if (first.text == "LET") {
            return parseLet(tokenizer);
        }
        else if (first.text == "PRINT") {
            return parsePrint(tokenizer);
        }
        else if (first.text == "INPUT") {
            return parseInput(tokenizer);
        }
        else if (first.text == "GOTO") {
            return parseGoto(tokenizer);
        }
        else if (first.text == "IF") {
            return parseIf(tokenizer);
        }
        else {
            throw std::runtime_error("Unknown keyword: " + first.text);
        }
    } else {
        // 允许省略 LET，直接 var = expr
        tokenizer.reset();  // rewind
        return parseLet(tokenizer);
    }
}

//----------------- 各语句解析框架 -----------------

Statement* Parser::parseLet(Tokenizer &tokenizer) {
    Token var = tokenizer.getNextToken();
    if (var.type != TokenType::IDENTIFIER)
        throw std::runtime_error("Expected identifier in LET");

    Token eq = tokenizer.getNextToken();
    if (eq.type != TokenType::OPERATOR || eq.text != "=")
        throw std::runtime_error("Expected '=' in LET");

    Expression* exp = parseExpression(tokenizer);
    return new LetStmt(var.text, exp);
}

Statement* Parser::parsePrint(Tokenizer &tokenizer) {
    Expression* exp = parseExpression(tokenizer);
    return new PrintStmt(exp);
}

Statement* Parser::parseInput(Tokenizer &tokenizer) {
    Token var = tokenizer.getNextToken();
    if (var.type != TokenType::IDENTIFIER)
        throw std::runtime_error("Expected identifier in INPUT");
    return new InputStmt(var.text);
}

Statement* Parser::parseGoto(Tokenizer &tokenizer) {
    Token lineToken = tokenizer.getNextToken();
    if (lineToken.type != TokenType::NUMBER)
        throw std::runtime_error("Expected line number in GOTO");
    int target = std::stoi(lineToken.text);
    return new GotoStmt(target);
}

Statement* Parser::parseIf(Tokenizer &tokenizer) {
    Expression* left = parseExpression(tokenizer);

    Token op = tokenizer.getNextToken();
    if (op.type != TokenType::OPERATOR)
        throw std::runtime_error("Expected operator in IF");

    Expression* right = parseExpression(tokenizer);

    Token thenToken = tokenizer.getNextToken();
    if (thenToken.type != TokenType::KEYWORD || thenToken.text != "THEN")
        throw std::runtime_error("Expected THEN in IF");

    Token lineToken = tokenizer.getNextToken();
    if (lineToken.type != TokenType::NUMBER)
        throw std::runtime_error("Expected line number after THEN");

    int target = std::stoi(lineToken.text);
    return new IfStmt(left, op.text, right, target);
}

//----------------- REM 语句 -----------------

Statement* Parser::parseRem(Tokenizer &tokenizer) {
    // REM 后面剩余的内容当作注释文本
    std::string comment;
    while (tokenizer.hasMoreToken()) {
        Token t = tokenizer.getNextToken();
        if (!comment.empty()) comment += " ";
        comment += t.text;
    }
    return new RemStmt(comment);
}


//----------------- parse the expression -----------------

Expression* Parser::parseExpression(Tokenizer &tokenizer) {
    Token t = tokenizer.getNextToken();
    if (t.type == TokenType::NUMBER) {
        return new ConstantExp(std::stoi(t.text));
    } else if (t.type == TokenType::IDENTIFIER) {
        return new IdentifierExp(t.text);
    } else {
        throw std::runtime_error("Invalid expression token: " + t.text);
    }
}
