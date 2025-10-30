TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        redmanager.cpp \
        router.cpp

DISTFILES += \
    PRUEBAS.txt

HEADERS += \
    redmanager.h \
    router.h
