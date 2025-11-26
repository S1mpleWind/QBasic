TEMPLATE = lib
CONFIG += staticlib c++17

INCLUDEPATH += $$PWD

SOURCES += program.cpp \
           statement.cpp \
           exp.cpp

HEADERS += program.h \
           statement.h \
           exp.h
