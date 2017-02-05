#-------------------------------------------------
#
# Project created by QtCreator 2016-12-18T23:53:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeaGame
TEMPLATE = app


SOURCES += main.cpp \
    seagame.cpp \
    ships.cpp \
    mainwindow.cpp \
    seafield.cpp

HEADERS  += \
    seagame.h \
    ships.h \
    points.h \
    mainwindow.h \
    seafield.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    README.md

RESOURCES += \
    resources.qrc
