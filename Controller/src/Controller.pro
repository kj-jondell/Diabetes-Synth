TEMPLATE = app
TARGET = Controller

LIBS += -L/usr/local/opt/rtmidi/lib -lrtmidi
LIBS += -L/usr/local/opt/rt-audio/lib -lrtaudio

INCLUDEPATH += /usr/local/opt/rt-audio/include
INCLUDEPATH += /usr/local/opt/rtmidi/include
INCLUDEPATH += /usr/local/opt/boost/include
INCLUDEPATH += /usr/local/Cellar//python@3.8/3.8.3/Frameworks/Python.framework/Versions/3.8/include/python3.8/
INCLUDEPATH += /usr/local/opt/pybind11/include

QT += multimedia 

HEADERS = Controller.h \
    MidiParser.h \
    OscParser.h \
    oscpkt/oscpkt.hh

SOURCES = Controller.cpp \
    MidiParser.cpp \
    OscParser.cpp \
    main.cpp 

FORMS += synthcontroller.ui 

RESOURCES = resources.qrc

QT += widgets network

LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
DEFINES += __MACOSX_CORE__

