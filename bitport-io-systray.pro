#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T19:19:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bitport-io-systray
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    Systray.cpp \
    Settings.cpp \
    Login.cpp \
    Config.cpp \
    Services.cpp \
    Token.cpp \
    FileItem.cpp \
    FileProgress.cpp

HEADERS += \
    Systray.h \
    Settings.h \
    Login.h \
    Config.h \
    Services.h \
    Token.h \
    IFileProgreess.h \
    FileItem.h \
    FileProgress.h

FORMS += \
    Login.ui \
    Settings.ui \
    Systray.ui \
    FileItem.ui

win32 {
    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
#        HEADERS += $$PWD/bitport/win32/bitport.h
        LIBS += $$PWD/bitport/win32/bitport.a
        LIBS += -lwinmm -lntdll -lWs2_32
    } else {
        ## Windows x64 (64bit) specific build here
    }
}

linux {
    HEADERS += $$PWD/bitport/linux/bitport.h
    LIBS += $$PWD/bitport/linux/bitport.a
}


RESOURCES += \
    bitport.qrc


include(vendor/QtAwesome/QtAwesome.pri)

DISTFILES += \
    bitport.css
