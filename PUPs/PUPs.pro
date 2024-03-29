#-------------------------------------------------
#
# Project created by QtCreator 2011-11-17T12:31:47
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = PUPs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderer.cpp \
    point.cpp \
    pup.cpp \
    nurbs.cpp \
    fileio.cpp \
    controlpoint.cpp

HEADERS  += mainwindow.h \
    renderer.h \
    pup.h \
    point.h \
    nurbs.h \
    fileio.h \
    controlpoint.h

FORMS    += mainwindow.ui
