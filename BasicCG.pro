#-------------------------------------------------
#
# Project created by QtCreator 2017-10-03T16:47:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BasicCG
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    basic_cg.cpp \
    basic_cg_premitive.cpp \
    pixmapscene.cpp \
    basic_cg_curve.cpp \
    basic_cg_fractal.cpp

HEADERS += \
        mainwindow.h \
    basic_cg.h \
    pixmapscene.h \
    rscene.h \
    rview.h \
    rdepthbuffer.h

FORMS += \
        mainwindow.ui
