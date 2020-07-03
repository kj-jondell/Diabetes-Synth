#include "OscParser.h"

OscParser::OscParser(QObject *parent, int oscAddressIn, int oscAddressOut)
    : QObject(parent), oscAddress(oscAddressIn), oscSendAddress(oscAddressOut) {

  if (oscAddressIn < 1 || oscAddressOut < 1)
    throw "Must define osc ports!";

  socket = new QUdpSocket(this); // initilize midiParser and bind to osc_address
  socket->bind(QHostAddress::LocalHost, oscAddress); // TODO react if busy!

  connect(socket, &QUdpSocket::readyRead, this,
          &OscParser::receivedMessage); // listen to port defined by socket
}

OscParser::~OscParser(void) {}

/**
 * Handle incoming osc from sc
 */
void OscParser::receivedMessage() {
  while (socket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = socket->receiveDatagram();
    qDebug() << datagram.data();
  }
}

/**
 * Send osc message
 */
void OscParser::sendMessage(Message msg) {
  packet_writer.init().addMessage(msg);
  socket->writeDatagram(packet_writer.packetData(), packet_writer.packetSize(),
                        QHostAddress::LocalHost, oscSendAddress);
}

/**
 * Load synth
 */
void OscParser::loadSynthDef(string synthDefName) {
  Message msg;
  msg.init("/d_load").pushStr(synthDefName);
  this->sendMessage(msg);
}

/**
 * Send quit message
 */
void OscParser::sendQuit() {
  Message msg;
  msg.init(QUIT_MESSAGE);
  this->sendMessage(msg);
}

/**
 * Free node
 */
void OscParser::freeNode(int nodeId) {
  Message msg;
  msg.init(NODE_FREE).pushInt32(nodeId);
  this->sendMessage(msg);
}

/**
 * Releases node by setting gate to 0 (triggering release of envelope)
 */
void OscParser::releaseNode(int nodeId) {
  Message msg;
  msg.init(NODE_SET).pushInt32(nodeId).pushStr("gate").pushInt32(0);
  this->sendMessage(msg);
}

/**
 * Creates a new instance of synth
 */
void OscParser::createNewSynth(int nodeId, string synthName,
                               QMap<QString, float> values) {
  Message msg;
  msg.init(SYNTH_NEW)
      .pushStr(synthName)
      .pushInt32(nodeId)
      .pushInt32(1)  // what to do with these?
      .pushInt32(0); // what to do with these?
  QMapIterator<QString, float> i(values);
  while (i.hasNext()) {
    i.next();
    string st = i.key().toStdString();
    msg.pushStr(st).pushFloat(i.value());
  }

  this->sendMessage(msg);
}

/**
 * Sets a float parameter
 */
void OscParser::setParameterFloat(int nodeId, string parameterName,
                                  float value) {
  Message msg;
  msg.init(NODE_SET).pushInt32(nodeId).pushStr(parameterName).pushFloat(value);
  this->sendMessage(msg);
}

/**
 * Reads buffer with given name
 */
void OscParser::readBufferFromFile(int bufNum, string filename) {
  Message msg;
  msg.init(BUFFER_ALLOC_READ).pushInt32(bufNum).pushStr(filename);

  this->sendMessage(msg);
}
