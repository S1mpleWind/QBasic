TEMPLATE = app
TARGET = tests

QT += testlib      # ← 必须
CONFIG += console testcase c++17 debug

INCLUDEPATH += \
    $$PWD \
    $$PWD/../core \
    $$PWD/../runtime\
    $$PWD/../interpreter

LIBS += \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/core/debug -lcore \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/runtime/debug -lruntime\
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/interpreter/debug -linterpreter

SOURCES += \
    test_main.cpp

HEADERS +=\
    test_expression.h \
    test_interpreter.h \
    test_parser.h \
    test_statement.h \
    test_program.h \
    test_tokenizer.h

