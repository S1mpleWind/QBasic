#include "parser.h"
#include <iostream>
#include <stdexcept>

// Parse one line of BASIC source code and return corresponding Statement
// Dispatches to specific statement parsers based on keyword
Statement* Parser::parseLine(int lineNum, const std::string &line) {
    Tokenizer tokenizer(line);
    
    // Empty line: return null statement
    if (!tokenizer.hasMoreToken()) return nullptr;

    // Get first token to identify statement type
    Token first = tokenizer.getNextToken();

    if (first.type == TokenType::KEYWORD) {
        // Dispatch to appropriate statement parser
        if (first.text == "REM") {
            return parseRem(tokenizer);
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
        else if (first.text == "END") {
            return parseEnd(tokenizer);
        }
        else {
            throw std::runtime_error("Unknown keyword: " + first.text);
        }
    } else {
        // Implicit LET: allow variable assignment without LET keyword
        tokenizer.reset();  // Rewind to start
        return parseLet(tokenizer);
    }
}

Statement* Parser::parseLet(Tokenizer &tokenizer) {
    // Parse variable name
    Token var = tokenizer.getNextToken();
    if (var.type != TokenType::IDENTIFIER)
        throw std::runtime_error("Expected identifier in LET");

    // Parse assignment operator
    Token eq = tokenizer.getNextToken();
    if (eq.type != TokenType::OPERATOR || eq.text != "=")
        throw std::runtime_error("Expected '=' in LET");

    // Parse right-hand side expression
    Expression* exp = parseExpression(tokenizer);
    return new LetStmt(var.text, exp);
}

// Parse PRINT statement
Statement* Parser::parsePrint(Tokenizer &tokenizer) {
    // Parse expression to print
    Expression* exp = parseExpression(tokenizer);
    return new PrintStmt(exp);
}

// Parse INPUT statement
Statement* Parser::parseInput(Tokenizer &tokenizer) {
    // Parse target variable
    Token var = tokenizer.getNextToken();
    if (var.type != TokenType::IDENTIFIER)
        throw std::runtime_error("Expected identifier in INPUT");
    return new InputStmt(var.text);
}

// Parse GOTO statement
Statement* Parser::parseGoto(Tokenizer &tokenizer) {
    // Parse target line number
    Token lineToken = tokenizer.getNextToken();

    if (lineToken.type != TokenType::NUMBER)
        throw std::runtime_error("Expected line number in GOTO");
    int target = std::stoi(lineToken.text);
    return new GotoStmt(target);
}

// Parse IF/THEN statement
Statement* Parser::parseIf(Tokenizer &tokenizer) {
    // Parse left operand
    Expression* left = parseExpression(tokenizer);

    // Parse relational operator
    Token op = tokenizer.getNextToken();
    if (op.type != TokenType::OPERATOR)
        throw std::runtime_error("Expected operator in IF");

    // Parse right operand
    Expression* right = parseExpression(tokenizer);

    // Parse THEN keyword
    Token thenToken = tokenizer.getNextToken();
    if (thenToken.type != TokenType::KEYWORD || thenToken.text != "THEN")
        throw std::runtime_error("Expected THEN in IF");

    // Parse target line number
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


Statement* Parser::parseEnd(Tokenizer &tokenizer){
    return new EndStmt();
}

//----------------- parse the expression -----------------

// Recursive Descent Parsing

Expression* Parser::parseExpression(Tokenizer &tk) {
    Expression* exp = parseTerm(tk);
    // call parseTerm so the , this will construct a kind of lower 'lhs'

    while (tk.hasMoreToken()) {
        Token t = tk.peekToken();

        if (t.text == "+" || t.text == "-") {
            tk.getNextToken();  // consume operator
            Expression* rhs = parseTerm(tk);
            exp = new CompoundExp(t.text, exp, rhs);
        } else break;
    }
    return exp;
}



/* base case */

Expression* Parser::parseTerm(Tokenizer &tk) {
    Expression* exp = parsePower(tk);

    while (tk.hasMoreToken()) {
        Token t = tk.peekToken();

        if (t.text == "*" || t.text == "/" || t.text == "MOD") {
            tk.getNextToken();  // consume operator
            Expression* rhs = parseFactor(tk);
            exp = new CompoundExp(t.text, exp, rhs);
        } else break;
    }
    return exp;
}



//Note: 'power' is right-combined

Expression* Parser::parsePower(Tokenizer &tokenizer ){

    Expression* base = parseFactor(tokenizer);

    Token t = tokenizer.peekToken(); // do not cosume
    if (t.text == "^" || t.text == "**") {
        tokenizer.getNextToken();

        Expression* exp = parsePower(tokenizer);   // right-recursive
        base = new CompoundExp("^", base, exp);
    }

    return base;

}



Expression* Parser::parseFactor(Tokenizer &tk) // highest precedence
{
    Token t = tk.getNextToken();

    // numbers/identifier
    if (t.type == TokenType::NUMBER) {
        return new ConstantExp(std::stoi(t.text));
    }
    if (t.type == TokenType::IDENTIFIER) {
        return new IdentifierExp(t.text);
    }

    // handle the Parentheses
    if (t.text == "(") {
        Expression* exp = parseExpression(tk);
        Token r = tk.getNextToken();
        if (r.text != ")") {
            throw std::runtime_error("Missing ')'");
        }
        return exp;
    }

    throw std::runtime_error("Invalid factor: " + t.text);
}

