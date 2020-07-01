#ifndef CONTROLLER_H
#define CONTROLLER_H

// #include <QAudioDeviceInfo>
#include <QMainWindow>
#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>

#include <oscpkt/oscpkt.hh>

#include <iostream>
#include <stdio.h>

#include "ui_synthcontroller.h"

class Controller : public QMainWindow, public Ui::Controller {
public:
  Controller(QWidget *parent = 0);
  virtual ~Controller();
};

class SynthController : public Controller {
  Q_OBJECT

public:
  explicit SynthController(QWidget *parent = nullptr);

private:
  QUdpSocket *socket;
  oscpkt::PacketWriter packet_writer;
  oscpkt::PacketReader packet_reader;
  int doneCounter = 0;

  void sendMessage(oscpkt::Message message);

private slots:
  void valueChanged(int idx);
  void oscReady();
};

#endif
