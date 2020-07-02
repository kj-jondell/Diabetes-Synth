TEMPLATE = app
TARGET = Controller

LIBS += -L/usr/local/opt/rtmidi/lib -lrtmidi
INCLUDEPATH += /usr/local/opt/rtmidi/include

QT += multimedia 

HEADERS = Controller.h \
    MidiParser.h \
    oscpkt/oscpkt.hh

SOURCES = Controller.cpp \
    MidiParser.cpp \
    main.cpp 

FORMS += synthcontroller.ui 

RESOURCES = resources.qrc

QT += widgets network

LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
DEFINES += __MACOSX_CORE__

