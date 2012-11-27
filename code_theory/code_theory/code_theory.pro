TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    processor.cpp \
    utils.cpp \
    alphabet.cpp \
    config_parser.cpp

HEADERS += \
    probability_counters.h \
    alphabet.h \
    data_providers.h \
    enthropy_stat.h \
    processor.h \
    utils.h \
    config_parser.h

LIBS += -lboost_system -lboost_filesystem

OTHER_FILES += \
    process.cfg \
    result.txt

