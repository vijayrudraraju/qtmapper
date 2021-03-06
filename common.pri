#Includes common configuration for all subdirectory .pro files.

CONFIG -= app_bundle
CONFIG += qt gui static thread x86
CONFIG += link_pkgconfig

#INCLUDEPATH += . .. /Developer/Projects/qtmapper/gui\
# /usr/local/include/mapper-0
#LIBS += -L/Developer/Projects/qtmapper/gui -L/usr/local/lib -llo

PKGCONFIG += libmapper-0 liblo

INCLUDEPATH += . .. $$PWD/gui $$PWD/data
LIBS += -L$$PWD/gui -L$$PWD/data

WARNINGS += -Wall

#QMAKE_CC = g++
QMAKE_CLEAN += *.dylib *.a
#QMAKE_CXXFLAGS += $$system(pkg-config --cflags --libs libmapper-0 liblo)

# The following keeps the generated files at least somewhat separate
# from the source files.
#UI_DIR = uics
#MOC_DIR = mocs
#OBJECTS_DIR = objs

HEADERS +=

SOURCES +=
