#ifndef CONTROLLER_H
#define CONTROLLER_H

// #include <QAudioDeviceInfo>
#include <QDebug>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QNetworkDatagram>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

#include <mutex>
#include <oscpkt/oscpkt.hh>

#include <iostream>
#include <stdio.h>

#include "MidiParser.h"
#include "OscParser.h"
#include "ui_synthcontroller.h"

#include <csignal>
#include <cstdlib>
#include <unistd.h>

#include <boost/format.hpp>

using namespace std;

#define MIDI_KEYS 128
#define MIDI_CHANNELS 16
#define OSC_ADDRESS 1222      // TODO make into variable..
#define OSC_SEND_ADDRESS 1234 // TODO make into variable..

#define SYNTH_NAME "DiabetesPanEnvelop" // TODO change to diabetes

// must correspond to objectnames as defined in ui file!
#define ATTACK "attack"
#define RELEASE "release"
#define DECAY "decay"
#define SUSTAIN "sustain"
#define DETUNE_FACTOR "detune_factor"
#define FREQ "freq"
#define BUFFER_NO "buffer_no"
#define FLUTTER "flutter"
#define VELOCITY "velocity"

class Controller : public QMainWindow, public Ui::Controller {
public:
  Controller(QWidget *parent = 0);
  virtual ~Controller();
};

class SynthController : public Controller {
  Q_OBJECT

public:
  explicit SynthController(QWidget *parent = nullptr);
  QString filename = "/Users/kj/Documents/diabetes/200630/samples/"
                     "sample_%1.wav"; // TODO temporary (variable!)
  vector<int> order = {8, 4,  3, 12, 2,  7,  13, 6,
                       5, 14, 1, 9,  11, 10, 15}; // TODO temporary...
  int nodeCounter = 1000; // start from 1000 as in sclang!
  int keys[MIDI_KEYS];

  void cleanupOnQuit();

public slots:
  void sendNoteOn(int, int);
  void sendNoteOff(int, int);
  void parseCC(int, int);

private:
  const QMap<QString, QString> remapNames{{ATTACK, "attackTime"},
                                          {DECAY, "decayTime"},
                                          {SUSTAIN, "sustainLevel"},
                                          {RELEASE, "releaseTime"},
                                          {DETUNE_FACTOR, "detuneFactor"},
                                          {BUFFER_NO, "bufferNum"},
                                          {FLUTTER, "flutter"}};

  const QMap<QString, vector<float>> rangeMap{
      {ATTACK, {0.f, 127.f, 0.f, 2.f}},
      {RELEASE, {0.f, 127.f, 0.1, 2.f}},
      {DECAY, {0.f, 127.f, 0.1, 2.f}},
      {SUSTAIN, {0.f, 127.f, 0.1, 1.f}},
      {BUFFER_NO, {0.f, 255.f, 0.f, 10.f}}, // TODO total num of bufs..
      {FLUTTER, {0.f, 127.f, 0.0001, 0.01}},
      {DETUNE_FACTOR, {0.f, 127.f, 1, 1.5}}};

  const QMap<int, QString> ccDefinitions{
      {16, ATTACK},
      {17, RELEASE},
      {18, DECAY},
      {19, SUSTAIN},
      {21, DETUNE_FACTOR}}; // TODO use bimap from boost
                            // instead (bi-directional,
                            // one-to-one...). Make variable that copies from
                            // this default value map!
  QMap<QString, float>
      dialValues; // store all dial values here! TODO as midi
                  // value and rangemap when sending to scsynth?

  MidiParser *midiParser;
  OscParser *oscParser;
  QProcess *scsynth;
  mutex mtx; // TODO necessary?
  int inChannels = 2, outChannels = 2;

  void startScSynth();
  int nextNodeID();
  void initParameters();

private slots:
  void valueChanged(int idx); // dials...
};

#endif
