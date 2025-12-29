// program.cpp
// Implementation of Program class for managing BASIC program structure

#include "program.h"
#include "statement.h"

// Constructor: initialize program with no lines and not ended
Program::Program() : nextLine(-1) {
    ended = false;
}

// Destructor: clean up all statements
Program::~Program() {
    clear();
}

// Add or update a source line at given line number
void Program::addSourceLine(int lineNumber, const std::string &line) {
    sourceLines[lineNumber] = line;
}

// Remove source line and its parsed statement
void Program::removeSourceLine(int lineNumber) {
    sourceLines.erase(lineNumber);

    // Also delete parsed statement if exists
    if (parsedStatements.count(lineNumber)) {
        delete parsedStatements[lineNumber];
        parsedStatements.erase(lineNumber);
    }
}

// Retrieve raw source code line by line number
std::string Program::getSourceLine(int lineNumber) const {
    if (sourceLines.count(lineNumber))
        return sourceLines.at(lineNumber);
    return "";
}

// Store parsed statement AST for a line number
void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Delete old statement if exists
    if (parsedStatements.count(lineNumber))
        delete parsedStatements[lineNumber];

    parsedStatements[lineNumber] = stmt;
}

// Fetch parsed statement
Statement* Program::getParsedStatement(int lineNumber) const {
    if (parsedStatements.count(lineNumber))
        return parsedStatements.at(lineNumber);
    return nullptr;
}

// First line number (or -1 if empty)
int Program::getFirstLineNumber() const {
    if (sourceLines.empty()) return -1;
    return sourceLines.begin()->first;
}

// Get the next line number in sorted order
int Program::getNextLineNumber(int lineNumber) const {
    auto it = sourceLines.upper_bound(lineNumber);
    if (it == sourceLines.end()) return -1;
    return it->first;
}

// Set next line number for execution
void Program::setNextLine(int lineNumber) {

    // TODO : legal checking
    if (sourceLines.count(lineNumber)) nextLine = lineNumber;
    else throw std::runtime_error("Goto none-exsiting line");
}

// Read next line number
int Program::getNextLine() const {
    return nextLine;
}

// Clear the program completely
void Program::clear() {
    // 1. delete parsedStatements Statement*
    for (auto &p : parsedStatements) {
        delete p.second;   // 重要：释放 AST 节点
    }
    parsedStatements.clear();

    // 2. clear all source lines
    sourceLines.clear();

    // 3. reset running state
    nextLine = -1;
    ended = false;
}


void Program::setEnd(){
    ended = true;
    nextLine = -1 ;
}

bool Program::isEnded() const{
    return ended;
}


std::string Program::getDisplayText()
{
    std::string rawText;
    for (const auto &entry : sourceLines){
        int lineNumber = entry.first;
        const std::string &line = entry.second;

        rawText += std::to_string(lineNumber) + " " + line + "\n";
    }
    return rawText;
}

void Program::resetStateCount(){
    for (auto &p : parsedStatements) {
        p.second->resetCount();   // Important reset count
    }
}
