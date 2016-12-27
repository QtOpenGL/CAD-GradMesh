#-------------------------------------------------
#
# Project created by QtCreator 2016-11-22T14:42:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SubdivCurves
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    mainview.cpp \
    controlnet.cpp \
    mousehandler.cpp \
    mesh.cpp \
    objfile.cpp

HEADERS  += mainwindow.h \
    mainview.h \
    controlnet.h \
    mousehandler.h \
    face.h \
    vertex.h \
    halfedge.h \
    mesh.h \
    objfile.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

CONFIG += c++11

