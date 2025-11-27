// program.cpp

#include "program.h"
#include "statement.h"


// Constructor
Program::Program() : nextLine(-1) {}

// Destructor
Program::~Program() {
    clear();
}

// Add or replace a source line
void Program::addSourceLine(int lineNumber, const std::string &line) {
    sourceLines[lineNumber] = line;
}

// Remove line and parsed statement
void Program::removeSourceLine(int lineNumber) {
    sourceLines.erase(lineNumber);

    if (parsedStatements.count(lineNumber)) {
        delete parsedStatements[lineNumber];
        parsedStatements.erase(lineNumber);
    }
}

// Get raw source line
std::string Program::getSourceLine(int lineNumber) const {
    if (sourceLines.count(lineNumber))
        return sourceLines.at(lineNumber);
    return "";
}

// Store parsed statement
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
    sourceLines.clear();
    for (auto &p : parsedStatements) {
        delete p.second;
    }
    parsedStatements.clear();
    nextLine = -1;
}

void Program::setEnd(){
    ended = true;
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
