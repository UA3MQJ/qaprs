TEMPLATE = app
QT = gui \
    core \
    sql \
    network
CONFIG += qt \
    warn_on \
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
HEADERS = include/mainwindowimpl.h \
    include/portoption.h \
    include/qaprsports.h \
    include/qaprscore.h \
    include/qaprspacket.h
SOURCES = src/mainwindowimpl.cpp \
    src/main.cpp \
    src/portoption.cpp \
    src/qaprsports.cpp \
    src/qaprscore.cpp \
    src/qaprspacket.cpp
RESOURCES = qaprs-core.qrc
