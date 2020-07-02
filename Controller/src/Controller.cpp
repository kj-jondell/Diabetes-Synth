#include "Controller.h"

Controller::Controller(QWidget *parent) : QMainWindow(parent) { setupUi(this); }
Controller::~Controller() {}

SynthController::SynthController(QWidget *parent) : Controller(parent) {
  parser = new MidiParser(this);

  for (int i = 0; i < MIDI_KEYS; i++) // initilize keys
    keys[i] = -1;

  for (int i = 1; i <= MIDI_CHANNELS; i++) // fill midichannel selection
    midichannel->addItem(QString::number(i));

  connect(midichannel, // respond to change of midichannel
          static_cast<void (QComboBox::*)(int index)>(
              &QComboBox::currentIndexChanged),
          parser, &MidiParser::setChannel);

  for (auto dial : findChildren<QDial *>()) // respond to change of dial
    connect(dial, &QDial::valueChanged, this, &SynthController::valueChanged);

  connect(parser, &MidiParser::noteOn, this,
          &SynthController::sendNoteOn); // get incoming note_on
  connect(parser, &MidiParser::noteOff, this,
          &SynthController::sendNoteOff); // get incoming note_off

  socket = new QUdpSocket(this);
  socket->bind(QHostAddress::LocalHost, OSC_ADDRESS);

  connect(socket, &QUdpSocket::readyRead, this, &SynthController::oscReady);

  oscpkt::Message msg;

  // TEMPORARY
  msg.init("/b_allocRead")
      .pushInt32(0)
      .pushStr("/Users/kj/Documents/diabetes/200627/samples/sample_59.wav");

  this->sendMessage(msg);

  msg.init("/b_allocRead")
      .pushInt32(1)
      .pushStr("/Users/kj/Documents/diabetes/200627/samples/sample_60.wav");

  this->sendMessage(msg);
  // TEMPORARY
}

void SynthController::sendNoteOff(int num, int velocity) {
  if (keys[num] != -1) {
    oscpkt::Message msg;
    msg.init("/n_set")
        .pushInt32(keys[num]) // handle nodeID
        .pushStr("gate")
        .pushInt32(0); // set keys[num] to -1
    this->sendMessage(msg);
    keys[num] = -1;
  }
}

void SynthController::sendNoteOn(int num, int velocity) {
  if (keys[num] == -1) {
    oscpkt::Message msg;

    mtx.lock(); // mutex needed?;
    keys[num] = this->nextNodeID();

    msg.init("/s_new")
        .pushStr("DiabetesPanEnvelop")
        .pushInt32(
            keys[num]) // handle nodeID (if keys[num] == -1 then generate node)
        .pushInt32(1)
        .pushInt32(0)
        .pushStr("freq")
        .pushFloat(num * 10)
        .pushStr("attackTime")
        .pushFloat(attack);
    this->sendMessage(msg);
    mtx.unlock(); // mutex needed?
  }
}

void SynthController::valueChanged(int idx) {
  QObject *sender = QObject::sender();

  oscpkt::Message msg;
  //  msg.init(std::string("/n_set"))
  //        .pushInt32(1005)
  //        .pushStr(std::string(sender->objectName().toLocal8Bit().data()))
  //        .pushFloat();
  if (std::string(sender->objectName().toLocal8Bit().data()) == "attack")
    attack = 5.f * (float(idx) / 127.f);
  this->sendMessage(msg);
}

/**
 * Send osc message
 */
void SynthController::sendMessage(oscpkt::Message msg) {
  packet_writer.init().addMessage(msg);
  socket->writeDatagram(packet_writer.packetData(), packet_writer.packetSize(),
                        QHostAddress::LocalHost, OSC_SEND_ADDRESS);
}

/**
 * Handle incoming osc from sc
 */
void SynthController::oscReady() {
  while (socket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = socket->receiveDatagram();
    qDebug() << datagram.data();
  }
}

int SynthController::nextNodeID() { return ++nodeCounter; }
