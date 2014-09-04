#-------------------------------------------------
#
# Project created by QtCreator 2014-09-04T09:06:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flash
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    server.cpp

HEADERS  += client.h \
    server.h

FORMS    += client.ui
