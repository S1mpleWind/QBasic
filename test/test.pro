TEMPLATE = app
TARGET = tests

CONFIG += c++17 qt testcase

INCLUDEPATH += \
    $$PWD \
    $$PWD/../core \
    $$PWD/../runtime

LIBS += \
    -L$$PWD/../core/debug -lcore \
    -L$$PWD/../runtime/debug -lruntime



SOURCES += \
    main.cpp
