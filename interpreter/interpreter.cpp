#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter()
    : state(),inputProvider(nullptr),
    outputConsumer(nullptr){
    ;
}

Interpreter::~Interpreter() = default;





EvalState &Interpreter::getState() {
    return state;
}

void Interpreter::reset() {
    // Try to clear EvalState: we call clear() if exists, otherwise reassign a fresh EvalState.
    // Many EvalState implementations provide clear() or constructor default behavior.
    // We use placement: reinitialize by assignment.
    state = EvalState();
}



void Interpreter::run(Program &program) {
    //FIXME: wrong logic,updated

    int first = program.getFirstLineNumber();
    program.setNextLine(first);


    int current = program.getNextLine();


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
