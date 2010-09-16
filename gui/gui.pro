! include(../common.pri) {
	error(Couldn't find the common.pri file!)
}

TEMPLATE = lib

UI_DIR = .

FORMS = form.ui

HEADERS += form.h \
node.h \
edge.h \
    mappergraphicsview.h \
    mappergraphicsscene.h \
    link.h

SOURCES += form.cpp \
node.cpp \
edge.cpp \
    mappergraphicsview.cpp \
    mappergraphicsscene.cpp \
    link.cpp
