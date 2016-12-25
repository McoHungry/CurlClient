HEADERS += \
    LoginPanel.h \
    cJSON.h

SOURCES += \
    LoginPanel.cpp \
    main.cpp \
    cJSON.c

QT += widgets gui

CONFIG += C++11
LIBS +=-lcurl
