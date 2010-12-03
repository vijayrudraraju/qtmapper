! include(../common.pri) {
        error( Couldn't find the common.pri file! )
}

TEMPLATE = app

LIBS += -lgui -ldata

SOURCES += main.cpp #\
#    guithread.cpp

TARGET = ../qtmapper

#HEADERS += \
#    guithread.h
