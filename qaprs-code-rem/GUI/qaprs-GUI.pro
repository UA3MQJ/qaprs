# -------------------------------------------------
# Project created by QtCreator 2010-01-09T22:18:18
# -------------------------------------------------
QT += network sql xml
TARGET = GUI
TEMPLATE = app

SOURCES += ./src/main.cpp \
           ./src/mainwindow.cpp \
           ../core/src/qaprscore.cpp \
           ../core/src/qaprsports.cpp \
           ../core/src/qaprspacket.cpp \
           ../core/src/qaprsbeacon.cpp \
           ./src/custommodels.cpp \
           ./src/portoption.cpp \
           ./src/beaconoption.cpp
HEADERS += ./include/mainwindow.h \
           ../core/include/qaprscore.h \
           ../core/include/qaprsports.h \
           ../core/include/qaprspacket.h \
           ../core/include/qaprsbeacon.h \
           ./include/custommodels.h \
           ./include/portoption.h \
           ./include/beaconoption.h
FORMS += ./ui/mainwindow.ui ./ui/portoption.ui ./ui/beaconoption.ui
RESOURCES = qaprs-core.qrc
