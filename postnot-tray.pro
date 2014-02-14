#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T11:15:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = postnot-tray
TEMPLATE = app

VERSION = 0.0.1
QMAKE_TARGET_COMPANY = kiz
QMAKE_TARGET_PRODUCT = PostNot-Tray
QMAKE_TARGET_DESCRIPTION = Postnofifikationssymbol
QMAKE_TARGET_COPYRIGHT = Sven Schober

SOURCES += main.cpp\
        pndialog.cpp \
    poststatus.cpp \
    task.cpp

HEADERS  += pndialog.h \
    poststatus.h \
    task.h

FORMS    += pndialog.ui \
    about.ui

RESOURCES += \
    postnot.qrc
