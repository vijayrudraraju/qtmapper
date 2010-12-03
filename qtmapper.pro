TEMPLATE = subdirs
SUBDIRS = gui data

QMAKE_CLEAN += *.dylib *.a

# build must be last:
CONFIG += ordered
SUBDIRS += build
