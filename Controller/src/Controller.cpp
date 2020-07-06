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

  connect(port, &QComboBox::currentTextChanged, this,
          &SynthController::newPort);

  connect(midiParser, &MidiParser::noteOn, this,
          &SynthController::sendNoteOn); // get incoming note_on
  connect(midiParser, &MidiParser::noteOff, this,
          &SynthController::sendNoteOff); // get incoming note_off
  connect(midiParser, &MidiParser::cc, this,
          &SynthController::parseCC); // get incoming cc

  connect(start_synth, &QPushButton::clicked, this,
          &SynthController::startScSynth); // respond to start_synth pressed!

  this->initAudioSelection();
  this->initParameters();
}

/**
 * Initilize audio selection ports
 */
void SynthController::initAudioSelection() {
  RtAudio audio;
  RtAudio::DeviceInfo info;

  for (unsigned int i = 0; i < audio.getDeviceCount(); i++) {
    info = audio.getDeviceInfo(i);
    if (info.probed && info.outputChannels > 0) {
      QString name = QString::fromStdString(info.name).split(": ")[1];
      devices->addItem(name);

      outputDevices[name] = info.outputChannels;
    }
  }

  this->formatPorts();
}

/**
 * Format ports according to chosen device
 */
void SynthController::formatPorts() {
  port->clear();
  int availablePorts = outputDevices[devices->currentText()];
  for (int i = 1; i <= availablePorts;
       i += 2) // TODO change incrementor to variable
    port->addItem(QString("%1-%2").arg(i).arg(
        i + 1)); // TODO change incrementor to variable
}

/**
 * Initilize parameters (maybe from csv or preset file)
 */
void SynthController::initParameters() {
  QMap<QString, float> defaultValues{
      // TODO add more presets!
      {ATTACK, 64},     {RELEASE, 20}, {DECAY, 48},         {SUSTAIN, 100},
      {BUFFER_NO, 127}, {FLUTTER, 24}, {DETUNE_FACTOR, 127}};

  for (auto name : defaultValues.keys())
    findChildren<QDial *>(name)[0]->setValue(defaultValues[name]);
}

/**
 * Starts sc synth process. PID is stored as variable
 */
void SynthController::startScSynth() {
  oscParser->sendQuit(); // ensure server is not already running!
  start_synth->setText("Restart server");

  outChannels = outputDevices[devices->currentText()];

  QString program =
      "/Applications/SuperCollider/SuperCollider.app/Contents/"
      "Resources/scsynth"; // TODO make into only scsynth (requires $PATH
                           // setting when not running from terminal!) TODO make
                           // into macro/constant (or variable?)
  QStringList args;
  args << "-i" << QString::number(inChannels) << "-o"
       << QString::number(outChannels) << "-u"
       << QString::number(OSC_SEND_ADDRESS) << "-R"
       << "0"
       << "-m" // memsize
       << QString::number(memorySize) << "-H"
       << devices->currentText(); // TODO make variable!

  scsynth = new QProcess(this);
  scsynth->setProgram(program);
  scsynth->setArguments(args);

  if (!scsynth->startDetached()) {
    qDebug() << "failed to start";
    exit(-1); // kill or try again?
  }

  if (!port->isEnabled())
    port->setEnabled(true);

  this->formatPorts();

  // TODO change device output formatting

  if (oscParser->sync()) // wait for boot...
  {
    int bufnum = 0;
    for (auto index : order) // load buffers
      oscParser->readBufferFromFile(bufnum++,
                                    QString(filename).arg(index).toStdString());
    // TODO count done messages and check against order.size()
  }
}

/**
 * Kill scsynth if program is closed
 */
void SynthController::cleanupOnQuit() { oscParser->sendQuit(); }

/**
 * Midi-osc gate (when "cc" received)
 * TODO make user definable midi mapping
 */
void SynthController::parseCC(int num, int velocity) {
  if (ccDefinitions[num] != NULL)
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
    dialValues[FREQ] = num; // TODO change! (tuning etc..)
    // fix tuning...
    dialValues[VELOCITY] = velocity;
    dialValues[OUT_BUS] = this->getPort();
    oscParser->createNewSynth(keys[num], SYNTH_NAME,
                              dialValues); // TODO change sine...
    mtx.unlock();                          // mutex needed?
  }
}

/**
 * Respond to user input (ports)
 */
void SynthController::updateKeys(QString parameter, float value) {
  for (int i = 0; i < MIDI_KEYS; i++)
    if (keys[i] != -1)
      oscParser->setParameterFloat(keys[i], parameter.toStdString(), value);
}

/**
 *  Return port num!
 */
int SynthController::getPort() {
  return port->currentText().split("-")[0].toInt() - 1;
}

/**
 * Respond to user input (ports)
 */
void SynthController::newPort(QString label) {
  this->updateKeys(OUT_BUS, (float)this->getPort());
}

/**
 * Respond to user input (dials)
 */
void SynthController::valueChanged(int idx) {
  QObject *sender = QObject::sender();
  float newValue = linearConversion(idx, rangeMap[sender->objectName()]);

  dialValues[remapNames[sender->objectName()]] = newValue; // TODO exeption...
  this->updateKeys(remapNames[sender->objectName()], newValue);
}

/**
 * Keep track of node ids
 */
int SynthController::nextNodeID() { return ++nodeCounter; }
