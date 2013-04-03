### eqmake4 was here ###
CONFIG += debug_and_release debug

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS += -lpthread -lboost_system -lboost_thread

SOURCES += main.cpp \
    discover_listener.cpp \
    discover_sender.cpp

HEADERS += \
    discover_listener.h \
    lowlevel.hpp \
    discover_sender.h

