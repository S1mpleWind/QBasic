#include "interpreter.h"
#include <iostream>

// Constructor: initialize interpreter with empty state
Interpreter::Interpreter()
    : state(), inputProvider(nullptr),
    outputConsumer(nullptr) {
}

// Destructor
Interpreter::~Interpreter() = default;

// Get reference to evaluation state
EvalState &Interpreter::getState() {
    return state;
}

// Reset interpreter: clear all variables and state
void Interpreter::reset() {
    // Create fresh EvalState by reassignment
    state = EvalState();
}

// Execute program to completion
void Interpreter::run(Program &program) {
    // Get first line number and set as next line to execute
    int first = program.getFirstLineNumber();
    program.setNextLine(first);

    // Get current line
    int current = program.getNextLine();

    // Execute until program ends or END statement
    while (current != -1 && !program.isEnded()) {
        Statement* stmt = program.getParsedStatement(current);

        /*    std::cout << "[DEBUG] current line: " << current
                  << ", stmt ptr: " << stmt
                  << ", state addr:" << &state<< std::endl;

        */

        if (!stmt) {
            current = program.getNextLineNumber(current);
            program.setNextLine(current);
            continue;
        }


        int oldNext = current;

        // 执行语句
        stmt->execute(state, program);

        //std::cout<<"next"<<program.getNextLine()<<std::endl;

        // 执行后如果 nextLine 没变（说明语句没有跳转）
        if (program.getNextLine() == oldNext) {
            int defaultNext = program.getNextLineNumber(current);

            // 如果已经没有下一行 → 程序结束（设置 -1）
            program.setNextLine(defaultNext);
        }

        current = program.getNextLine();
    }
    //TODO: after a round of running, reset the program to 'unend'
    program.recoverEnd();
}


bool Interpreter::step(Program &program) {
    //std::cout<<"0101010101" ;

    if (program.isEnded()) {
        std::cout<<"ended"<<std::endl;
        return false;}

    int current = program.getNextLine();
    std::cout<<current<<std::endl;

    if (current == -1) return false;

    Statement *stmt = program.getParsedStatement(current);

    std::cout<<stmt->toString();

    if (!stmt) {
        int next = program.getNextLineNumber(current);
        program.setNextLine(next);
        return true; // we advanced (no-op statement)
    }

    // ensure default advancement if statement doesn't change it
    defaultAdvanceIfNeeded(program, current);

    stmt->execute(state, program);
    return true;
}

void Interpreter::defaultAdvanceIfNeeded(Program &program, int currentLine) {
    // If nextLine isn't already set to something meaningful, set it to next sequential line.
    // We can't directly query an internal "next line" other than getNextLine(),
    // so we set default to Program::getNextLineNumber(currentLine).
    int defaultNext = program.getNextLineNumber(currentLine);
    program.setNextLine(defaultNext);
}


std::string Interpreter::toSyntaxTree(Program& program) const{
    std::string result;

    int line = program.getFirstLineNumber();
    while(line != -1) {
        Statement* stmt = program.getParsedStatement(line);
        if(stmt) {
            // 每条语句调用其 toSyntaxTree 并传入缩进级别 0
            result += std::to_string(line) + "\n";
            result += stmt->toSyntaxTree(state.getRuntimeStats(),0);
            result += "\n";
        }
        line = program.getNextLineNumber(line);
    }

    return result;
}
