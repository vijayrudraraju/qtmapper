! include(../common.pri) {
        error( Couldn't find the common.pri file! )
}

TEMPLATE = app

LIBS += -lmapper-0 -lgui

SOURCES += main.cpp #\
#    guithread.cpp

TARGET = ../qtmapper

#HEADERS += \
#    guithread.h
