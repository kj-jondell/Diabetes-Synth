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

    Message *msg;
    packetReader.init(datagram.data(), datagram.data().size());

    msg = packetReader.popMessage();

    if (msg->match(SYNCED))
      synced = true;
    else if (msg->match(DONE))
      doneCounter++;
  }
}

/**
 * Send osc message
 */
void OscParser::resetCounter() { doneCounter = 0; }

/**
 * Send osc message
 */
int OscParser::getCounter() { return doneCounter; }

/**
 * Send osc message
 */
void OscParser::sendMessage(Message msg) {
  packetWriter.init().addMessage(msg);
  socket->writeDatagram(packetWriter.packetData(), packetWriter.packetSize(),
                        QHostAddress::LocalHost, oscSendAddress);
}

/**
 * Load synth. Returns true if synced with server, else false!
 * Sleep time in seconds.
 */
bool OscParser::sync(float sleepTime, int maxPolls) {
  synced = false;

  Message msg;
  msg.init(SYNC);

  int hungCounter = 0;
  do {
    this->sendMessage(msg);
    usleep((int)(sleepTime * 1000 * 1000)); // Qtimer instead?

    QCoreApplication::processEvents(); // use qmutex / qwaitcondition instead?
                                       // (possible with polling?)
    if (++hungCounter == maxPolls)
      break;
  } while (!synced);

  return synced;
}

/**
 * Load synth
 */
void OscParser::loadSynthDef(string synthDefName) {
  Message msg;
  msg.init(SYNTHDEF_LOAD).pushStr(synthDefName);
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
  msg.init(NODE_SET).pushInt32(nodeId).pushStr(GATE).pushInt32(0);
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
 * Free all buffers
 */
void OscParser::freeBuffer(int nodeId) {
  Message msg;
  msg.init(BUFFER_FREE).pushInt32(nodeId);
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
