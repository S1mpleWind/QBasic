QT -= gui
TEMPLATE = lib
CONFIG += staticlib c++17

INCLUDEPATH += $$PWD \
               $$PWD/../core

LIBS += -L$$PWD/../core/debug -lcore

SOURCES += evalstate.cpp \
           parser.cpp \
           tokenizer.cpp \
           parser.cpp

HEADERS += evalstate.h \
           parser.h \
           tokenizer.h \
           parser.h
