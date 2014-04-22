#-------------------------------------------------
#
# Project created by QtCreator 2014-04-22T16:43:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = RFID-Manager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp

HEADERS  += mainwindow.h \
    files/AX12/AX12.h \
    serial.h

FORMS    += mainwindow.ui

RESOURCES += \
    rfidmanager.qrc
