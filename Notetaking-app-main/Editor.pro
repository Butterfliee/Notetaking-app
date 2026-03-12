QT += core gui widgets
QT += core gui sql printsupport
QT += network
TARGET = Editorek
TEMPLATE = app
SOURCES += main.cpp \
    authmanager.cpp \
    window.cpp

HEADERS += \
    authmanager.h \
    window.h

RESOURCES += \
    back.qrc \
    insert.qrc

DISTFILES += \
    ../Downloads/background.png
