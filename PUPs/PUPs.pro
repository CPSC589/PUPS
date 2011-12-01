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
    state.cpp \
    controlpoint.cpp

HEADERS  += mainwindow.h \
    pupbasis.h \
    renderer.h \
    pup.h \
    point.h \
    nurbs.h \
    state.h \
    controlpoint.h

FORMS    += mainwindow.ui
