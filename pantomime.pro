

PROJECT = pantomime

TEMPLATE = app
QT += core gui declarative

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ./include

RESOURCES = pantomime.qrc

OBJECTS_DIR = tmp/objs
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc

HEADERS = \
    include/loop-gesture.h \
    include/loop-recognizer.h \
    include/panto.h

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/main.qml

SOURCES += \
    src/panto.cpp \
    src/loop-gesture.cpp \
    src/loop-recognizer.cpp \
    src/main.cpp

unix:!symbian:!maemo5 {
    target.path = /opt/pantomime/bin
    INSTALLS += target
}

unix:!symbian:!maemo5 {
    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
}

unix:!symbian:!maemo5 {
    icon.files = pantomime.png
    icon.path = /usr/share/icons/hicolor/64x64/apps
    INSTALLS += icon
}
