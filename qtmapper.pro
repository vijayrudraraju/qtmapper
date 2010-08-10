TEMPLATE = subdirs
SUBDIRS = gui

QMAKE_CLEAN += *.dylib *.a

# build must be last:
CONFIG += ordered
SUBDIRS += build
