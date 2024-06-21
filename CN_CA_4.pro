QT = core
QT += network
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        client.cpp \
        congest_client.cpp \
        congest_server.cpp \
        main.cpp \
        router.cpp \
        server.cpp \
        serverthread.cpp \
        slidingwindowclient.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += -lws2_32
HEADERS += \
    client.h \
    defs.h \
    router.h \
    server.h \
    serverthread.h \
    slidingwindowclient.h
