#ifndef CONTROLLER_H
#define CONTROLLER_H

// #include <QAudioDeviceInfo>
#include <QDebug>
#include <QList>
#include <QMainWindow>
#include <QNetworkDatagram>
#include <QObject>
#include <QString>
#include <QUdpSocket>

#include <oscpkt/oscpkt.hh>

#include <iostream>
#include <stdio.h>

#include "MidiParser.h"
#include "ui_synthcontroller.h"

#include <mutex>

using namespace std;

#define MIDI_KEYS 128
#define MIDI_CHANNELS 16
#define OSC_ADDRESS 1222
#define OSC_SEND_ADDRESS 1234

class Controller : public QMainWindow, public Ui::Controller {
public:
  Controller(QWidget *parent = 0);
  virtual ~Controller();
};

class SynthController : public Controller {
  Q_OBJECT

public:
  explicit SynthController(QWidget *parent = nullptr);
  float attack = 0.1;
  int nodeCounter = 1000;
  int keys[MIDI_KEYS];

public slots:
  void sendNoteOn(int, int);
  void sendNoteOff(int, int);

private:
  QUdpSocket *socket;
  oscpkt::PacketWriter packet_writer;
  oscpkt::PacketReader packet_reader;
  MidiParser *parser;
  std::mutex mtx;

  int doneCounter = 0; // TODO temporary

  void sendMessage(oscpkt::Message message);
  int nextNodeID();

private slots:
  void valueChanged(int idx);
  void oscReady();
};

#endif
