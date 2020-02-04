#-------------------------------------------------
#
# Project created by QtCreator 2010-01-14T15:35:29
#
#-------------------------------------------------

QT       += core widgets gui sql xml network

QT       -= gui

TARGET = console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../core/include/aprs.h \
    ../core/include/qsysvars.h \
    ../core/include/qaprsports.h \
    ../core/include/qsysports.h \
    ../core/include/qaprspacket.h \
    ../core/include/qaprsbeacon.h \
    ../core/include/qatomix.h \
    ../core/include/qaprscore.h \
    ../core/include/qsysbeacons.h \
    ../core/include/qaprssymbols.h \
    ../core/include/qsyspackets.h \
    ../core/include/qsysstationlist.h \
    ../core/include/version.h \
    ../core/include/qsysmessages.h


SOURCES += src/main.cpp ../core/src/aprs.cpp \
            ../core/src/qsysvars.cpp \
            ../core/src/qaprsports.cpp \
            ../core/src/qsysports.cpp \
            ../core/src/qaprspacket.cpp \
            ../core/src/qaprsbeacon.cpp \
            ../core/src/qatomix.cpp \
            ../core/src/qaprscore.cpp \
            ../core/src/qsysbeacons.cpp \
            ../core/src/qaprssymbols.cpp \
            ../core/src/qsyspackets.cpp \
            ../core/src/qsysstationlist.cpp \
            ../core/src/qsysmessages.cpp

