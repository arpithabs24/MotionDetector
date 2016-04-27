#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T18:03:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionDetector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS+= -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lopencv_highgui -lopencv_video
