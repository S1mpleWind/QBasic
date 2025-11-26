QT -= gui
TEMPLATE = lib
CONFIG += staticlib c++17

INCLUDEPATH += $$PWD \
               $$PWD/../core

LIBS += \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/core/debug -lcore

SOURCES += evalstate.cpp \
           parser.cpp \
           tokenizer.cpp \
           parser.cpp

HEADERS += evalstate.h \
           parser.h \
           tokenizer.h \
           parser.h
