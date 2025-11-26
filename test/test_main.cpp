#include <iostream>

#include "test_statement.h"
#include "test_expression.h"
#include "test_program.h"

#include "test_tokenizer.h"

int main() {
    std::cout << "Running Expression tests..." << std::endl;
    runExpressionTests();

    std::cout << "\nRunning Program tests..." << std::endl;
    runProgramTests();

    std::cout << "\nRunning Statement tests..." << std::endl;
    runStatementTests();


    std::cout << "\nRunning tokenizer tests..." << std::endl;
    testTokenizer();

    std::cout << "\nAll tests completed successfully!" << std::endl;
    return 0;
}
