#Includes common configuration for all subdirectory .pro files.

CONFIG -= app_bundle
CONFIG += qt gui static thread x86

INCLUDEPATH += . .. /Developer/Projects/qtmapper/gui\
 /usr/local/include/mapper-0
LIBS += -L/Developer/Projects/qtmapper/gui -L/usr/local/lib -llo

WARNINGS += -Wall

QMAKE_CC = g++
QMAKE_CLEAN += *.dylib *.a
#QMAKE_CXXFLAGS += -m32 -L/usr/local/lib -llo
#QMAKE_CFLAGS_DEBUG += -static
#QMAKE_CFLAGS_RELEASE += -m32
#QMAKE_CFLAGS_SHLIB += -m32

# The following keeps the generated files at least somewhat separate
# from the source files.
#UI_DIR = uics
#MOC_DIR = mocs
#OBJECTS_DIR = objs
