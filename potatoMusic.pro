QT += core widgets gui multimedia

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets multimedia

CONFIG += c++17 taglib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    discord-files/achievement_manager.cpp \
#    discord-files/activity_manager.cpp \
#    discord-files/application_manager.cpp \
#    discord-files/core.cpp \
#    discord-files/image_manager.cpp \
#    discord-files/lobby_manager.cpp \
#    discord-files/network_manager.cpp \
#    discord-files/overlay_manager.cpp \
#    discord-files/relationship_manager.cpp \
#    discord-files/storage_manager.cpp \
#    discord-files/store_manager.cpp \
#    discord-files/types.cpp \
#    discord-files/user_manager.cpp \
#    discord-files/voice_manager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
#    discord-files/achievement_manager.h \
#    discord-files/activity_manager.h \
#    discord-files/application_manager.h \
#    discord-files/core.h \
#    discord-files/discord.h \
#    discord-files/event.h \
#    discord-files/ffi.h \
#    discord-files/image_manager.h \
#    discord-files/lobby_manager.h \
#    discord-files/network_manager.h \
#    discord-files/overlay_manager.h \
#    discord-files/relationship_manager.h \
#    discord-files/storage_manager.h \
#    discord-files/store_manager.h \
#    discord-files/types.h \
#    discord-files/user_manager.h \
#    discord-files/voice_manager.h \
    mainwindow.h \
    ui_mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$system(pkg-config --cflags taglib)
LIBS += $$system(pkg-config --libs taglib)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
