TEMPLATE = app
TARGET = Controller

QT += multimedia 

HEADERS = Controller.h \
    oscpkt/oscpkt.hh

SOURCES = Controller.cpp \
    main.cpp 

FORMS += synthcontroller.ui 

RESOURCES = resources.qrc

QT += widgets network
