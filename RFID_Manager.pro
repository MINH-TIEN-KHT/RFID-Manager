#-------------------------------------------------
#
# Project created by QtCreator 2014-04-20T21:12:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = RFID_Manager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    console.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    rfidmanager.qrc
