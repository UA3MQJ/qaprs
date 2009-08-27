TEMPLATE = app
QT = gui \
    core \
    sql \
    core \
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
    ui/portoption.ui \
    ui/packets.ui \
    ui/messages.ui \
    ui/stations.ui
HEADERS = include/mainwindowimpl.h \
    include/packetswindow.h \
    include/messageswindow.h \
    include/stationswindow.h \
    include/portoption.h \
    include/qaprsports.h \
    include/qaprscore.h \
    include/qaprspacket.h \
    include/customsqlmodels.h\
    include/aprs.h
SOURCES = src/mainwindowimpl.cpp \
    src/packetswindow.cpp \
    src/messageswindow.cpp \
    src/stationswindow.cpp \
    src/main.cpp \
    src/portoption.cpp \
    src/qaprsports.cpp \
    src/qaprscore.cpp \
    src/qaprspacket.cpp \
    src/customsqlmodels.cpp \
    src/aprs.cpp
RESOURCES = qaprs-core.qrc
TRANSLATIONS = qaprs-core_ru.ts \
               qaprs-core_en.ts
