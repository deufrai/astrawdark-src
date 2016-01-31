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

RC_ICONS = resources/pix/appicon.ico
ICON = resources/pix/appicon.icns

RESOURCES += resources/i18n.qrc \
    resources/pixmaps.qrc

SOURCES += src/main.cpp\
    src/globals.cpp \
    src/commands/abstractCommand.cpp \
    src/commands/commandFactory.cpp \
    src/commands/scanDarkSourceCommand.cpp \
    src/data/access/exifReader.cpp \
    src/data/dataStore.cpp \
    src/data/dto/imageInfo.cpp \
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
    src/helpers/localeHelper.cpp \
    src/commands/abstractScanCommand.cpp \
    src/commands/scanLightsCommand.cpp \
    src/commands/lightsCheckCommand.cpp \
    src/data/dto/imageStack.cpp \
    src/data/helpers/imageStackHelper.cpp \
    src/commands/matchDarksCommand.cpp \
    src/processing/darkMatcher.cpp \
    src/processing/exceptions/matchingException.cpp \
    src/processing/exceptions/noDarkForShootSettingsExcpetion.cpp \
    src/processing/exceptions/noDarkForTempException.cpp \
    src/commands/computeBestMatchCommand.cpp

INCLUDEPATH += src

HEADERS  += \
    src/globals.h \
    src/commands/abstractCommand.h \
    src/commands/commandFactory.h \
    src/commands/scanDarkSourceCommand.h \
    src/data/access/exifReader.h \
    src/data/dataStore.h \
    src/data/dto/imageInfo.h \
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
    src/helpers/localeHelper.h \
    src/commands/abstractScanCommand.h \
    src/commands/scanLightsCommand.h \
    src/commands/lightsCheckCommand.h \
    src/data/dto/imageStack.h \
    src/data/helpers/imageStackHelper.h \
    src/commands/matchDarksCommand.h \
    src/processing/darkMatcher.h \
    src/processing/exceptions/matchingException.h \
    src/processing/exceptions/noDarkForShootSettingsExcpetion.h \
    src/processing/exceptions/noDarkForTempException.h \ 
    src/commands/computeBestMatchCommand.h

FORMS    += \
    src/gui/forms/mainWindow.ui \
    src/gui/forms/prefDialog.ui \
    src/gui/forms/aboutDialog.ui \
    src/gui/forms/commandReportDisplay.ui

LIBS += -lexiv2

DISTFILES += \
    doc/README \
    Doxyfile \
    README \
    COMPILING

UI_DIR = tmp
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp

# Linux install
linux-* {

    BINSRC = ./$$TARGET

    target.path = /usr/bin
    INSTALLS += target
}
