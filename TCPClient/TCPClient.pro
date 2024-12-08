QT -= gui

CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    communication.cpp \
    shared.cpp \
    tcpcomm.cpp \
    tcpdata.cpp \
    tcpclient.cpp

HEADERS += \
    communication.h \
    shared.h \
    tcpdata.h \
    tcpcomm.h \
    tcpclient.h
