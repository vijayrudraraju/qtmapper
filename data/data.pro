! include(../common.pri) {
	error(Couldn't find the common.pri file!)
}

TEMPLATE = lib

HEADERS += \
    mapperdata.h

SOURCES += \
    mapperdata.cpp
