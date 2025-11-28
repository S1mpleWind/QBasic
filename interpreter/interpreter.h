#pragma once

#include <functional>
#include <QString>

#include "../core/statement.h"
#include "../core/program.h"
#include "../runtime/evalstate.h"
//#include "../runtime/parser.h"

/**
 * Interpreter
 *
 * Drive Program execution using Statement::execute(EvalState&, Program&).
 * - Holds its own EvalState
 * - Supports run (full execution) and step (single statement)
 * - Provides hooks for input/output (callbacks) to be integrated with UI
 *
 * 注意：为了与现有 Statement/Program 解耦，本 Interpreter 不直接访问语句内部成员，
 * 而是通过 Statement::execute(...) 触发语句行为；语句若要使用 I/O 回调，需要从
 * 全局或 EvalState 中取得回调（可在语句实现中调用 Interpreter 提供的回调，或把回调注入到 EvalState）。
 */
class Interpreter {
public:
    Interpreter();
    ~Interpreter();

    // 装载源码
    //void loadProgram(const std::vector<std::string> &source);

    // Run the program until end (returns when finished)
    void run(Program& program);

    // Execute exactly one statement (return true if executed, false if program ended or no statement)
    bool step(Program &program);

    // Reset interpreter state (clears variable table)
    void reset();


    //TODO
    void setInputProvider(std::function<QString()> f) {
        state.inputProvider = std::move(f);
    }
    void setOutputConsumer(std::function<void(const QString&)> f) {
        state.outputConsumer = std::move(f);
    }

    // Access EvalState (if UI or tests need direct access)
    EvalState &getState();


    //syntax tree
    //NOTE:only after running the program we can get the right syntax tree.
    std::string toSyntaxTree(Program &program) const;

private:
    EvalState state;
    //Program program;
    //Parser parser;

    // IO callbacks (may be nullptr)
    std::function<int()> inputProvider;
    std::function<void(const QString&)> outputConsumer;

    // Internal helpers
    void defaultAdvanceIfNeeded(Program &program, int currentLine);
};
