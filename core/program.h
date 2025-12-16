/**
 * @file    program.h
 * @brief   Program class for managing BASIC program structure
 *          Stores source lines, parsed statements, and execution state
 *          Acts as the central data structure for program representation
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

// Program class: manages program representation and execution flow
// Stores source code lines, parsed statements, and tracks next line to execute
class Program {
public:
    // Constructor and destructor
    Program();
    ~Program();

    // Source line management
    // Add a source line with line number
    void addSourceLine(int lineNumber, const std::string &line);
    
    // Remove a source line by line number
    void removeSourceLine(int lineNumber);
    
    // Retrieve source code text by line number
    std::string getSourceLine(int lineNumber) const;

    // Parsed statement management
    // Store parsed statement AST for a line number
    void setParsedStatement(int lineNumber, Statement *stmt);
    
    // Retrieve parsed statement for a line number
    Statement *getParsedStatement(int lineNumber) const;

    // Line navigation
    // Get first executable line number in program
    int getFirstLineNumber() const;
    
    // Get next line number after given line (for sequential execution)
    int getNextLineNumber(int lineNumber) const;

    // Execution control
    // Set next line to execute (used by GOTO statements)
    void setNextLine(int lineNumber);
    
    // Get next line to execute
    int getNextLine() const;

    // Clear all program data
    void clear();

    // Mark program as finished (for END statement)
    void setEnd();
    
    // Check if program execution should stop
    bool isEnded() const;

    // Get display text for UI
    std::string getDisplayText();

private:
    std::map<int, std::string> sourceLines;      // Raw BASIC source code lines
    std::map<int, Statement*> parsedStatements;  // Parsed Abstract Syntax Tree (AST) nodes
    int nextLine;                                // Next line number to execute
    bool ended;                                  // Flag indicating program termination
};
