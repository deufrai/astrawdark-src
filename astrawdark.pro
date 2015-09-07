#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T10:13:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = astrawdark
TEMPLATE = app


SOURCES += src/main.cpp\
        src/gui/mainwindow.cpp \
    src/globals.cpp \
    src/data/imageinfo.cpp \
    src/data/datastore.cpp \
    src/commands/commandfactory.cpp \
    src/commands/abstractcommand.cpp \
    src/commands/scandarksourcecommand.cpp \
    src/data/access/exifreader.cpp

HEADERS  += src/gui/mainwindow.h \
    src/globals.h \
    src/data/imageinfo.h \
    src/data/datastore.h \
    src/commands/commandfactory.h \
    src/commands/abstractcommand.h \
    src/commands/scandarksourcecommand.h \
    src/data/access/exifreader.h

FORMS    += src/gui/forms/mainwindow.ui

LIBS += -lexiv2
