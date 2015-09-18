#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T10:13:56
#
#-------------------------------------------------

QT       += core gui concurrent printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

macx:INCLUDEPATH += /usr/local/include

TARGET = astrawdark
TEMPLATE = app

TRANSLATIONS = resources/i18n/astrawdark_fr.ts

RESOURCES += resources/i18n.qrc

SOURCES += src/main.cpp\
    src/globals.cpp \
    src/commands/abstractCommand.cpp \
    src/commands/commandFactory.cpp \
    src/commands/scanDarkSourceCommand.cpp \
    src/data/access/exifReader.cpp \
    src/data/dataStore.cpp \
    src/data/imageInfo.cpp \
    src/gui/mainWindow.cpp \
    src/gui/prefDialog.cpp \
    src/commands/commandManager.cpp \
    src/commands/commandQueue.cpp \
    src/gui/aboutDialog.cpp \
    src/commands/commandExecutor.cpp \
    src/commands/signalDispatcher.cpp \
    src/commands/reports/commandReport.cpp \
    src/gui/commandReportDisplay.cpp \
    src/contrib/qCustomPlot/qcustomplot.cpp \
    src/gui/plotManager.cpp \
    src/helpers/localeHelper.cpp

HEADERS  += \
    src/globals.h \
    src/commands/abstractCommand.h \
    src/commands/commandFactory.h \
    src/commands/scanDarkSourceCommand.h \
    src/data/access/exifReader.h \
    src/data/dataStore.h \
    src/data/imageInfo.h \
    src/gui/mainWindow.h \
    src/gui/prefDialog.h \
    src/commands/commandManager.h \
    src/commands/commandQueue.h \
    src/gui/aboutDialog.h \
    src/commands/commandExecutor.h \
    src/commands/signalDispatcher.h \
    src/commands/reports/commandReport.h \
    src/gui/commandReportDisplay.h \
    src/contrib/qCustomPlot/qcustomplot.h \
    src/gui/plotManager.h \
    src/helpers/localeHelper.h

FORMS    += \
    src/gui/forms/mainWindow.ui \
    src/gui/forms/prefDialog.ui \
    src/gui/forms/aboutDialog.ui \
    src/gui/forms/commandReportDisplay.ui

LIBS += -lexiv2

DISTFILES += \
    doc/README \
    Doxyfile \
    README

UI_DIR = tmp
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp
