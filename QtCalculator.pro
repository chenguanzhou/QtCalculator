#-------------------------------------------------
#
# Project created by QtCreator 2014-06-27T16:47:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCalculator
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp

CONFIG += mobility
MOBILITY = 

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h

unix:QMAKE_CXXFLAGS += -std=c++0x
