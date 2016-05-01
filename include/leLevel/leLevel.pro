#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T22:38:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = leLevel
CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leLevel)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leLevel-)
CONFIG += build_all
CONFIG += c++11

TEMPLATE = lib

DEFINES += LELEVEL_LIBRARY

DESTDIR = ../lib

SOURCES += \
    src/Level.cpp \
    src/Object.cpp

HEADERS += leLevel.h \
    ../DllGlobal.h \
    Level.h \
    Object.h \
    Layer.h

# Подключение lifeEngine

LIBS += -L$$PWD/../../lib

CONFIG(release, debug|release):
 LIBS += -lleSystem -llePhysic
CONFIG(debug, debug|release):
 LIBS += -lleSystem-d -llePhysic-d

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
