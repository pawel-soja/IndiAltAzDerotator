QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

LIBS += -lindiclient

INCLUDEPATH += src

SOURCES += \
    src/indiclient.cpp \
    src/indirotator.cpp \
    src/inditelescope.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/rotatordial.cpp \

HEADERS += \
    src/indiclient.h \
    src/indirotator.h \
    src/inditelescope.h \
    src/mainwindow.h \
    src/rotatordial.h

FORMS += \
    src/mainwindow.ui

unix:!android: target.path = /usr/bin/
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/data.qrc
