#include <iostream>

#include "test_statement.h"
#include "test_expression.h"
#include "test_program.h"

#include "test_tokenizer.h"
#include "test_parser.h"

#include "test_interpreter.h"

int main() {
    std::cout << "Running Expression tests..." << std::endl;
    runExpressionTests();

    std::cout << "\nRunning Program tests..." << std::endl;
    //runProgramTests();

    std::cout << "\nRunning Statement tests..." << std::endl;
    runStatementTests();


    std::cout << "\nRunning tokenizer tests..." << std::endl;
    testTokenizer();

    std::cout << "\nRunning parser tests..." << std::endl;
    int i = testParser();
    assert(i==0);

    std::cout<< "\nRunning interpreter test" <<std::endl;
    testInterpreter();

    std::cout << "\nAll tests completed successfully!" << std::endl;
    return 0;
}
