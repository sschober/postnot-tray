#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T11:15:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = postnot-tray
TEMPLATE = app


SOURCES += main.cpp\
        pndialog.cpp \
    poststatus.cpp \
    task.cpp

HEADERS  += pndialog.h \
    poststatus.h \
    task.h

FORMS    += pndialog.ui

RESOURCES += \
    postnot.qrc
