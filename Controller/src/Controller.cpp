#include "Controller.h"

/**
 * Linear - linear range mapping
 */
float linearConversion(int oldValue, vector<float> rangeList) {
  return ((float)oldValue - rangeList[0]) / (rangeList[1] - rangeList[0]) *
             (rangeList[3] - rangeList[2]) +
         rangeList[2];
}

Controller::Controller(QWidget *parent) : QMainWindow(parent) { setupUi(this); }
Controller::~Controller() {}

SynthController::SynthController(QWidget *parent) : Controller(parent) {
  midiParser = new MidiParser(this);
  oscParser = new OscParser(this, OSC_ADDRESS, OSC_SEND_ADDRESS);

  for (int i = 0; i < MIDI_KEYS; i++) // initilize keys
    keys[i] = -1;

  for (int i = 1; i <= MIDI_CHANNELS; i++) // fill midichannel selection
    midichannel->addItem(QString::number(i));

  connect(midichannel, // respond to change of midichannel
          static_cast<void (QComboBox::*)(int index)>(
              &QComboBox::currentIndexChanged),
          midiParser, &MidiParser::setChannel);

  for (auto dial : findChildren<QDial *>()) // respond to change of dial
    connect(dial, &QDial::valueChanged, this, &SynthController::valueChanged);

  connect(midiParser, &MidiParser::noteOn, this,
          &SynthController::sendNoteOn); // get incoming note_on
  connect(midiParser, &MidiParser::noteOff, this,
          &SynthController::sendNoteOff); // get incoming note_off
  connect(midiParser, &MidiParser::cc, this,
          &SynthController::parseCC); // get incoming cc

  connect(start_synth, &QPushButton::clicked, this,
          &SynthController::startScSynth); // respond to start_synth pressed!

  this->initParameters();
}

/**
 * Initilize parameters (maybe from csv or preset file)
 */
void SynthController::initParameters() {
  dialValues[ATTACK] = 0.1;
  dialValues[RELEASE] = 1;
  dialValues[DECAY] = 1;
  dialValues[SUSTAIN] = 0.8;
  dialValues[DETUNE_FACTOR] = 1;

  // TODO update dials to show values (default values given as midi value?)
}

/**
 * Starts sc synth process. PID is stored as variable
 */
void SynthController::startScSynth() {
  int in = 2, out = 2; // TODO

  oscParser->sendQuit(); // ensure server is not already running!
  start_synth->setText("Restart server");

  QString program =
      "/Applications/SuperCollider/SuperCollider.app/Contents/"
      "Resources/scsynth"; // TODO make into only scsynth (requires $PATH
                           // setting when not running from terminal!) TODO make
                           // into macro/constant (or variable?)
  QStringList args;
  args << "-i" << QString::number(in) << "-o" << QString::number(out) << "-u"
       << QString::number(OSC_SEND_ADDRESS) << "-R"
       << "0"
       << "-H"
       << "Soundflower (64ch)"; // TODO make variable!

  scsynth = new QProcess(this);
  scsynth->setProgram(program);
  scsynth->setArguments(args);

  if (!scsynth->startDetached()) {
    qDebug() << "failed to start";
    exit(-1); // kill or try again?
  }

  // TODO SYNC WITH SERVER ("WAIT FOR BOOT")
  // TODO LOAD BUFFERS FROM CHOSEN PROJECT
  // oscParser->readBufferFromFile(); TODO
}

/**
 * Kill scsynth if program is closed
 */
void SynthController::cleanupOnQuit() { oscParser->sendQuit(); }

/**
 * Midi-osc gate (when "cc" received)
 * TODO make user definable midi mapping (not with switch-case but for-loop or
 * map/dictionary)
 */
void SynthController::parseCC(int num, int velocity) {
  findChildren<QDial *>(ccDefinitions[num])[0]->setValue(
      velocity); // Defined in header-file TODO make user definable
}

/**
 * Midi-osc gate (when "note_off" received)
 */
void SynthController::sendNoteOff(int num, int velocity) {
  if (keys[num] != -1) {
    oscParser->releaseNode(keys[num]);
    keys[num] = -1;
  }
}

/**
 * Midi-osc gate (when "note_on" received)
 */
void SynthController::sendNoteOn(int num, int velocity) {
  if (keys[num] == -1) {
    mtx.lock(); // mutex needed?;
    keys[num] = this->nextNodeID();
    dialValues[FREQ] = num * 10; // TODO change!
    oscParser->createNewSynth(keys[num], SYNTH_NAME,
                              dialValues); // TODO change sine...
    mtx.unlock();                          // mutex needed?
  }
}

/**
 * Respond to user input (dials)
 */
void SynthController::valueChanged(int idx) {
  QObject *sender = QObject::sender();

  dialValues[sender->objectName()] =
      linearConversion(idx, rangeMap[sender->objectName()]);
  // oscParser->setParameterFloat(); TODO
}

/**
 * Keep track of node ids
 */
int SynthController::nextNodeID() { return ++nodeCounter; }
