#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T12:22:35
#
#-------------------------------------------------

QT       -= core gui

TARGET = leEfects
CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leEfects)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leEfects-)
CONFIG += build_all
CONFIG += c++11

TEMPLATE = lib

DEFINES += LEEFECTS_LIBRARY

DESTDIR = ../lib

SOURCES += \
    src/Light.cpp \
    src/LightManager.cpp

HEADERS += leEfects.h \
    ../DllGlobal.h \
    LightManager.h \
    Light.h

# Подключение lifeEngine

LIBS += -L$$PWD/../../lib

CONFIG(release, debug|release):
 LIBS += -lleSystem -lleLevel
CONFIG(debug, debug|release):
 LIBS += -lleSystem-d -lleLevel-d

# Подключение SFML

LIBS += -L$$PWD/../SFML/lib

CONFIG(release, debug|release):
 LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release):
 LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += $$PWD/../SFML/include
DEPENDPATH += $$PWD/../SFML/include

# Подключение XML

INCLUDEPATH += $$PWD/../XML
DEPENDPATH += $$PWD/../XML

# Подключение Box2D

INCLUDEPATH += $$PWD/../Box2D
DEPENDPATH += $$PWD/../Box2D

unix {
    target.path = /usr/lib
    INSTALLS += target
}
