#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T18:17:14
#
#-------------------------------------------------

QT       -= core gui

TARGET = leEntity
CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leEntity)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leEntity-)
CONFIG += build_all
CONFIG += c++11

TEMPLATE = lib

DEFINES += LEENTITY_LIBRARY

DESTDIR = ../lib

SOURCES += \
    src/BasicEntity.cpp \
    src/EntityManager.cpp \
    src/BasicItems.cpp \
    src/BasicPersonages.cpp \
    src/BasicBullet.cpp \
    src/AI.cpp \
    src/ViewAI.cpp

HEADERS +=\
    ../DllGlobal.h \
    leEntity.h \
    EntityManager.h \
    BasicItems.h \
    BasicPersonages.h \
    BasicEntity.h \
    BasicBullet.h \
    AI.h \
    ViewAI.h

# Подключение lifeEngine

LIBS += -L$$PWD/../../lib

CONFIG(release, debug|release):
 LIBS += -lleSystem -llePhysic -lleWeapons -lleAnimation
CONFIG(debug, debug|release):
 LIBS += -lleSystem-d -llePhysic-d -lleWeapons-d -lleAnimation-d

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
