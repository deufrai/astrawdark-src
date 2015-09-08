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
    src/globals.cpp \
    src/commands/abstractCommand.cpp \
    src/commands/commandFactory.cpp \
    src/commands/scanDarkSourceCommand.cpp \
    src/data/access/exifReader.cpp \
    src/data/dataStore.cpp \
    src/data/imageInfo.cpp \
    src/gui/mainWindow.cpp \
    src/gui/prefDialog.cpp

HEADERS  += \
    src/globals.h \
    src/commands/abstractCommand.h \
    src/commands/commandFactory.h \
    src/commands/scanDarkSourceCommand.h \
    src/data/access/exifReader.h \
    src/data/dataStore.h \
    src/data/imageInfo.h \
    src/gui/mainWindow.h \
    src/gui/prefDialog.h

FORMS    += \
    src/gui/forms/mainWindow.ui \
    src/gui/forms/prefDialog.ui

LIBS += -lexiv2
