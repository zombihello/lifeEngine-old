#-------------------------------------------------
#
# Project created by QtCreator 2016-03-18T14:28:37
#
#-------------------------------------------------

QT       -= core gui

TARGET = leAnimation
CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leAnimation)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leAnimation-)
CONFIG += build_all
CONFIG += c++11

TEMPLATE = lib

DESTDIR = ../lib

DEFINES += LEANIMATION_LIBRARY

SOURCES += \
    src/Animation.cpp \
    src/AnimationManager.cpp

HEADERS += leAnimation.h \
    ../DllGlobal.h \
    AnimationManager.h \
    Animation.h

# Подключение lifeEngine

LIBS += -L$$PWD/../../lib

CONFIG(release, debug|release):
 LIBS += -lleSystem
CONFIG(debug, debug|release):
 LIBS += -lleSystem-d

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

unix {
    target.path = /usr/lib
    INSTALLS += target
}
