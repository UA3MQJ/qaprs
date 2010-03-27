#-------------------------------------------------
#
# Project created by QtCreator 2010-01-14T15:35:29
#
#-------------------------------------------------

QT       += network sql xml

QT       -= gui

TARGET = console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../core/include/qaprscore.h \
           ../core/include/qaprsports.h \
           ../core/include/qaprspacket.h \
           ../core/include/qaprsbeacon.h

SOURCES += src/main.cpp ../core/src/qaprscore.cpp \
           ../core/src/qaprsports.cpp \
           ../core/src/qaprspacket.cpp \
           ../core/src/qaprsbeacon.cpp

