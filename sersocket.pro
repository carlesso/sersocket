#-------------------------------------------------
#
# Project created by QtCreator 2013-06-23T10:51:05
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = sersocket
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(qextserialport/qextserialport.pri);

SOURCES += main.cpp \
    logger.cpp \
    sersocket.cpp

HEADERS += \
    logger.h \
    sersocket.h

OTHER_FILES += sersocket.ini
