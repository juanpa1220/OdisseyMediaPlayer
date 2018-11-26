#-------------------------------------------------
#
# Project created by QtCreator 2018-11-15T21:18:11
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OdisseyMediaPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG-=app_bundle

SOURCES += \
        main.cpp \
        widget.cpp \
    cliente.cpp \
    dialogmetadata.cpp

HEADERS += \
        widget.h \
    cliente.h \
    definitions.h \
    JSON/json.hpp \
    dialogmetadata.h

FORMS += \
        widget.ui \
    dialogmetadata.ui

QMAKE_CXXFLAGS += -std=gnu++11

#Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Desktop/MacOS/lib/release/ -lvlc.5
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Desktop/MacOS/lib/debug/ -lvlc.5
#else:unix: LIBS += -L$$PWD/../../Desktop/MacOS/lib/ -lvlc.5

#INCLUDEPATH += $$PWD/../../Desktop/MacOS/include
#DEPENDPATH += $$PWD/../../Desktop/MacOS/include

#macx: LIBS += -L$$PWD/../Contents/MacOS/lib/ -lvlc.5

#INCLUDEPATH += $$PWD/../Contents/MacOS/include
#DEPENDPATH += $$PWD/../Contents/MacOS/include




macx: LIBS += -L$$PWD/../MacOS/lib/ -lvlc.5

INCLUDEPATH += $$PWD/../MacOS/include
DEPENDPATH += $$PWD/../MacOS/include

DISTFILES += \
    videos/prueba1.mp4
