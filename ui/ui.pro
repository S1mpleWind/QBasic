QT += widgets
TEMPLATE = app
CONFIG += c++17

INCLUDEPATH += $$PWD \
               $$PWD/../core \
               $$PWD/../runtime

LIBS += \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/core/debug -lcore \
    -L$$PWD/../build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/runtime/debug -lruntime

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui
