TEMPLATE = app
TARGET = Controller

LIBS += -L/usr/local/opt/rtmidi/lib -lrtmidi
LIBS += -L/usr/local/lib -lrtaudio

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/rtmidi
INCLUDEPATH += /usr/local/opt/boost/include
INCLUDEPATH += ../libs/oscpkt

#LIBS += -L../libs/rtmidi/lib -lrtmidi
#LIBS += -L../libs/rtaudio/lib -lrtaudio
#
#INCLUDEPATH += ../libs/rtaudio/include
#INCLUDEPATH += ../libs/rtmidi/include

HEADERS = Controller.h \
    MidiParser.h \
    Tuning.h \
    CApplication.h \
    CSVReader.h \
    OscParser.h 

SOURCES = Controller.cpp \
    MidiParser.cpp \
    OscParser.cpp \
    Tuning.cpp \
    main.cpp 

FORMS += synthcontroller.ui 
RESOURCES = resources.qrc

QT += widgets network

# BINARY.path = Extra
# BINARY.files = ../libs/Converter
QMAKE_BUNDLE_DATA += BINARY

QMAKE_INFO_PLIST = Info.plist
