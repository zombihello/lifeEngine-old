#-------------------------------------------------
#
# Project created by QtCreator 2016-03-10T22:55:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = leWeapons
CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leWeapons)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leWeapons-)
CONFIG += build_all
CONFIG += c++11

TEMPLATE = lib

DESTDIR = ../lib

DEFINES += LEWEAPONS_LIBRARY

SOURCES += \
    src/BasicWeapon.cpp \
    src/WeaponManager.cpp

HEADERS += leWeapons.h \
    ../DllGlobal.h \
    BasicWeapon.h \
    WeaponManager.h

# Подключение lifeEngine

LIBS += -L$$PWD/../../lib

CONFIG(release, debug|release):
 LIBS += -lleSystem -lleAnimation
CONFIG(debug, debug|release):
 LIBS += -lleSystem-d -lleAnimation-d

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

#INCLUDEPATH += $$PWD/../Box2D
#DEPENDPATH += $$PWD/../Box2D


unix {
    target.path = /usr/lib
    INSTALLS += target
}
