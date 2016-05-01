#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T23:09:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = leMedia
CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leMedia)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leMedia-)
CONFIG += build_all
CONFIG += c++11

DEFINES += LEMEDIA_LIBRARY

TEMPLATE = lib

DESTDIR = ../lib

SOURCES += \
    src/Music.cpp \
    src/MusicManager.cpp \
    src/Sound.cpp \
    src/SoundManager.cpp

HEADERS += leMedia.h \
    SoundManager.h \
    Sound.h \
    MusicManager.h \
    Music.h \
    ../DllGlobal.h

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
