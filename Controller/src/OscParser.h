#ifndef OSCPARSER_H
#define OSCPARSER_H
#include <QCoreApplication>
#include <QMap>
#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>
#include <oscpkt/oscpkt.hh>

#include <iostream>
#include <stdio.h>
#include <unistd.h>

/// SERVER COMMANDS
#define QUIT_MESSAGE "/quit"
#define NODE_SET "/n_set"
#define SYNTHDEF_LOAD "/d_load"
#define NODE_FREE "/n_free"
#define BUFFER_ALLOC_READ "/b_allocRead"
#define SYNTH_NEW "/s_new"
#define SYNC "/sync"
#define SYNCED "/synced"
#define DONE "/done"

// SYNTH DEF COMMANDS
#define GATE "gate"

using namespace std;
using namespace oscpkt;

class OscParser : public QObject {
  Q_OBJECT
public:
  OscParser(QObject *parent = 0, int oscAddressIn = -1, int oscAddressOut = -1);
  virtual ~OscParser();

  void sendQuit();
  void releaseNode(int nodeId);
  void freeNode(int nodeId);
  void createNewSynth(int nodeId, string synthName,
                      QMap<QString, float> values);
  void readBufferFromFile(int bufNum, string filename);
  void setParameterFloat(int nodeId, string parameterName, float value);
  void loadSynthDef(string synthDefName);
  bool sync(float pollWaitTime = 0.5, int maxPolls = 20);

private:
  int oscAddress, oscSendAddress;
  bool synced;
  QUdpSocket *socket;
  PacketWriter packetWriter;
  PacketReader packetReader;

  void sendMessage(Message message);

private slots:
  void receivedMessage();
};
#endif
