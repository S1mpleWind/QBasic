/**
 * @file    program.h
 * @brief   this is head file is responsible for the storage and sequence of the
 *          program's raw text
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include<QString>
#include <map>
//#include "statement.h"
class Statement;

// Program manages source lines, parsed statements,
// and execution control (next line number).
class Program {
public:
    Program();
    ~Program();

    // Source line operations
    void addSourceLine(int lineNumber, const std::string &line);
    void removeSourceLine(int lineNumber);
    std::string getSourceLine(int lineNumber) const;

    // Parsed statement operations
    void setParsedStatement(int lineNumber, Statement *stmt);
    Statement *getParsedStatement(int lineNumber) const;

    // Line navigation
    int getFirstLineNumber() const;
    int getNextLineNumber(int lineNumber) const;

    // Execution control
    void setNextLine(int lineNumber);
    int getNextLine() const;

    // Clear program
    void clear();

    void setEnd();       // mark program finished
    bool isEnded() const;  // query if execution should stop

    // interact with ui
    std::string getDisplayText();

private:
    std::map<int, std::string> sourceLines;      // Raw BASIC source
    std::map<int, Statement*> parsedStatements;  // Parsed AST nodes
    int nextLine;                                // Next line to execute
    bool ended;
};
