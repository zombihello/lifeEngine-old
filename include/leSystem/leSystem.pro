#-------------------------------------------------
#
# Project created by QtCreator 2016-03-01T14:57:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = leSystem

CONFIG(release, debug|release): TARGET = $$qtLibraryTarget(leSystem)
CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget(leSystem-)
CONFIG += build_all
CONFIG += c++11

TEMPLATE = lib

DEFINES += LESYSTEM_LIBRARY

DESTDIR = ../lib

SOURCES += \
    ../XML/tinystr.cpp \
    ../XML/tinyxml.cpp \
    ../XML/tinyxmlerror.cpp \
    ../XML/tinyxmlparser.cpp \
    src/MouseCursor.cpp \
    src/Text.cpp \
    src/TextManager.cpp \
    src/Button.cpp \
    src/ButtonManager.cpp \
    src/System.cpp

HEADERS += \
    ../DllGlobal.h \
    ../XML/tinystr.h \
    ../XML/tinyxml.h \
    BasicStagesGame.h \
    MouseCursor.h \
    Text.h \
    TextManager.h \
    Button.h \
    ButtonManager.h \
    System.h \
    leSystem.h

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
