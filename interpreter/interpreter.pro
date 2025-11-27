TEMPLATE = lib
CONFIG += staticlib

TARGET = interpreter

INCLUDEPATH += ../core
INCLUDEPATH += ../runtime
INCLUDEPATH += ../parser

LIBS += \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/core/debug -lcore \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/runtime/debug -lruntime\

SOURCES += \
    interpreter.cpp

HEADERS += \
    interpreter.h

