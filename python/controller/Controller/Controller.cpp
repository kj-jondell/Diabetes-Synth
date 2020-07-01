#include "Controller.h"

Controller::Controller(QWidget *parent) : QMainWindow(parent) { setupUi(this); }

Controller::~Controller() {}

SynthController::SynthController(QWidget *parent) : Controller(parent) {

  for (auto dial : findChildren<QDial *>()) {
    connect(dial, &QDial::valueChanged, this, &SynthController::valueChanged);
  }

  socket = new QUdpSocket(this);
  socket->bind(QHostAddress::LocalHost, 1222);

  connect(socket, &QUdpSocket::readyRead, this, &SynthController::oscReady);

  oscpkt::Message msg;

  msg.init("/b_allocRead")
      .pushInt32(0)
      .pushStr("/Users/kj/Documents/diabetes/200627/samples/sample_59.wav");

  this->sendMessage(msg);

  msg.init("/b_allocRead")
      .pushInt32(1)
      .pushStr("/Users/kj/Documents/diabetes/200627/samples/sample_60.wav");

  this->sendMessage(msg);
}

void SynthController::valueChanged(int idx) {
  QObject *sender = QObject::sender();

  oscpkt::Message msg;
  msg.init(std::string("/n_set"))
      .pushInt32(1005)
      .pushStr(std::string(sender->objectName().toLocal8Bit().data()))
      .pushFloat(idx / 127);

  this->sendMessage(msg);
}

void SynthController::sendMessage(oscpkt::Message msg) {
  packet_writer.init().addMessage(msg);
  socket->writeDatagram(packet_writer.packetData(), packet_writer.packetSize(),
                        QHostAddress::LocalHost, 1234);
}

void SynthController::oscReady() {
  while (socket->hasPendingDatagrams()) {
    QNetworkDatagram datagram = socket->receiveDatagram();
    if (datagram.data().contains("/done")) {
      printf("done received %d\n", ++doneCounter);
    }
  }
  if (doneCounter == 2) {
    doneCounter = 0;
    oscpkt::Message msg;
    msg.init("/s_new")
        .pushStr("DiabetesPanEnvelop")
        .pushInt32(1005)
        .pushInt32(1)
        .pushInt32(0);

    this->sendMessage(msg);
  }
}
