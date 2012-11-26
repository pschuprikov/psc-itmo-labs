TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    data_providers.cpp \
    running_params.cpp \
    processor.cpp

HEADERS += \
    probability_counters.h \
    stat_common.h \
    alphabet.h \
    data_providers.h \
    enthropy_stat.h \
    running_params.h \
    processor.h

