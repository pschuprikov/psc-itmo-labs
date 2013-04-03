### eqmake4 was here ###
CONFIG += debug_and_release debug

QMAKE_CXXFLAGS += -std=c++0x

TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS += -lpthread -lboost_system -lboost_thread

SOURCES += main.cpp \
    discover_listener.cpp \
    discover_sender.cpp \
    session_manager.cpp \
    #session_validator.cpp \
    #messenger.cpp \
    global_params.cpp

HEADERS += \
    discover_listener.h \
    lowlevel.hpp \
    discover_sender.h \
    session.h \
    session_manager.h \
    session_validator.h \
    messenger.h \
    global_params.h \
    packets.hpp

