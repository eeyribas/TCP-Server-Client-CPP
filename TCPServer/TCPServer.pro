QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    shared.cpp \
    communication.cpp \
    tcpcomm.cpp \
    tcpclient.cpp

HEADERS += \
    mainwindow.h \
    shared.h \
    communication.h \
    tcpcomm.h \
    tcpclient.h

FORMS += \
    mainwindow.ui
