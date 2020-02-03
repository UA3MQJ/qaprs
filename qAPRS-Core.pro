#TEMPLATE = app
QT = gui \
    widgets \
    core \
    sql \
    network
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
DEPENDPATH += . \
    include \
    src \
    ui
INCLUDEPATH += . \
    include
FORMS = ui/mainwindow.ui \
    ui/portoption.ui
HEADERS = \
    include/mainwindow.h \
    include/portoption.h \
    include/qaprsports.h \
    include/qaprscore.h \
    include/qaprspacket.h
SOURCES = \
    src/main.cpp \
    src/mainwindow.cpp \
    src/portoption.cpp \
    src/qaprsports.cpp \
    src/qaprscore.cpp \
    src/qaprspacket.cpp
RESOURCES = qaprs-core.qrc
