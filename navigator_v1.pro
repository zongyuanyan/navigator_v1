
QT  += core gui widgets quick quickwidgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = stove_navigator
SOURCES += \
    main.cpp \
    stove_navigator.cpp

HEADERS += \
    params.h \
    stove_navigator.h

FORMS += \
    stove_navigator.ui

TRANSLATIONS += \
    navigator_v1_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

RC_ICONS = avvbu-fiqxm-001.ico

DISTFILES += \
    resources/images/stove_icon6.svg

unix|win32: {
    CONFIG(release, debug|release): {
        LIBS += -L$$PWD/libs/ -lyaml
    }
    CONFIG(debug, debug|release): {
        LIBS += -L$$PWD/libs/ -lyaml
    }
    INCLUDEPATH += $$PWD/src/yaml/include
    DEPENDPATH += $$PWD/src/yaml/include
}

