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

  this->startScSynth(2, 2); // starts scsynth

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

/**
 * Starts sc synth process. PID is stored as variable
 */
void SynthController::startScSynth(int in, int out) {
  scsynthPid = fork();

  if (scsynthPid == 0) {
    ostringstream in_str, out_str, port_str;
    char *port_name = "Soundflower (64ch)"; // TODO variable...

    in_str << 2;
    out_str << 2;
    port_str << OSC_SEND_ADDRESS;

    // char *args[] = {"scsynth", "-i",   "2",  "-o",      "2",
    //                 "-u",      "1234", "-H", port_name, NULL};
    // char *args[] = {"ls", "-a", NULL};
    char *args[] = {"scsynth", "-i", "2", "-o", "2", "-u", "1234", NULL};

    qDebug() << execvp(args[0], args);
    exit(-1);
    // kill if scsynth won't start?
  }
}

/**
 * Kill scsynth if program is closed
 */
void SynthController::cleanupOnQuit() { kill(scsynthPid, SIGTERM); }

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

/**
 * Respond to user input (dials)
 */
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
