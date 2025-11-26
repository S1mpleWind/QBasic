QT += widgets
TEMPLATE = app
CONFIG += c++17

INCLUDEPATH += $$PWD \
               $$PWD/../core \
               $$PWD/../runtime

LIBS += -L$$PWD/../core/debug -lcore \
        -L$$PWD/../runtime/debug -lruntime

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui
