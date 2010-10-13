! include(../common.pri) {
	error(Couldn't find the common.pri file!)
}

TEMPLATE = lib

UI_DIR = .

FORMS = form.ui

HEADERS += form.h \
link.h \
node.h \
    mappergraphicsview.h \
    mappergraphicsscene.h \
    utility.h

SOURCES += form.cpp \
link.cpp \
node.cpp \
    mappergraphicsview.cpp \
    mappergraphicsscene.cpp \
    utility.cpp
