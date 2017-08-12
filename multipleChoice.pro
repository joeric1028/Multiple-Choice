#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T10:17:44
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = multipleChoice
TEMPLATE = app


SOURCES += main.cpp\
    logindialog.cpp \
    teacherinterface.cpp \
    studentinterface.cpp

HEADERS  += \
    logindialog.h \
    teacherinterface.h \
    studentinterface.h

FORMS    += \
    logindialog.ui \
    teacherinterface.ui \
    studentinterface.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    config/config.xml
