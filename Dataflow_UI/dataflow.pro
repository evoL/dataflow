QT += widgets

HEADERS	    =   mainwindow.h \
		diagramscene.h \
		arrow.h \
    diagrammoduleitem.h
SOURCES	    =   mainwindow.cpp \
		main.cpp \
		arrow.cpp \
		diagramscene.cpp \
    diagrammoduleitem.cpp
RESOURCES   =	diagramscene.qrc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target
