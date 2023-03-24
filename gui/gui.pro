#-------------------------------------------------
#
# Project created by QtCreator 2010-08-26T10:20:58
#
#-------------------------------------------------

QT       += widgets core gui sql xml network

TARGET = gui
TEMPLATE = app

TRANSLATIONS    = lng_ru.ts

SOURCES  += \
    ./src/main.cpp \
    ./src/mainwindow.cpp \
    ../core/src/aprs.cpp \
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
    ../core/src/qsysmessages.cpp \
    src/storageprogress.cpp \
    src/custommodels.cpp \
    src/portoption.cpp \
    src/beaconoption.cpp \
    src/packetswindow.cpp \
    src/messageswindow.cpp \
    src/stationswindow.cpp \
    src/mapswindow.cpp \
    src/loadmap.cpp \
    src/previewmap.cpp \
    src/messengerwindow.cpp \
    src/startwindow.cpp \
    src/mygraphicsview.cpp


HEADERS  += \
    ./include/mainwindow.h \
    ../core/include/aprs.h \
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
    ../core/include/qsysmessages.h \
    include/storageprogress.h \
    include/custommodels.h \
    include/portoption.h \
    include/beaconoption.h \
    include/packetswindow.h \
    include/messageswindow.h \
    include/stationswindow.h \
    include/mapswindow.h \
    include/loadmap.h \
    include/previewmap.h \
    include/messengerwindow.h \
    include/startwindow.h \
    include/mygraphicsview.h


FORMS    += \
    ./ui/mainwindow.ui \
    ui/storageprogress.ui \
    ui/portoption.ui \
    ui/beaconoption.ui \
    ui/packets.ui \
    ui/messages.ui \
    ui/stations.ui \
    ui/maps.ui \
    ui/loadmap.ui \
    ui/previewmap.ui \
    ui/messenger.ui \
    ui/start.ui

RESOURCES = gui.qrc
