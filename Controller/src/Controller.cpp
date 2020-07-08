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
  tuner = new Tuning(JUST_TUNING);

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

  //// MENU
  connect(open_converter, &QAction::triggered, this,
          &SynthController::openConverter);
  connect(open_project, &QAction::triggered, this, // qfiledialog...
          &SynthController::openProject);

  //// Tuning Signals / Slots
  connect(tuning, &QComboBox::currentTextChanged, this,
          &SynthController::changeTuning);
  connect(equal_temperament, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int idx) { tuner->setTuning(EQUAL_TUNING, idx); });
  connect(root_freq, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int idx) { rootFreqValue = root_freq->value(); });
  connect(degree, QOverload<int>::of(&QSpinBox::valueChanged),
          [=](int idx) { degreeValue = degree->value(); });

  rootFreqValue = root_freq->value();
  degreeValue = degree->value();

  initAudioSelection();
  initParameters();

  fileOpen("/Users/kj/Documents/Diabetes Synth "
           "Projects/200707/200707.dia"); // TODO
                                          // remove
}

/**
 * Open file event
 */
void SynthController::fileOpen(QString csvFilename) {
  // TODO check if project already open (dialog)

  reader = new CSVReader(csvFilename);
  settingsDictionary = reader->getParsed();

  setProjectName(settingsDictionary[PROJECTNAME]);
  filename = QString(settingsDictionary[FILENAME]);

  killSynth("Start server");

  order.clear();
  for (auto value : settingsDictionary[ORDER].split(' '))
    order.push_back(value.toInt());

  rangeMap[BUFFER_NO][3] =
      (float)(order.size() - 2); // set buffer no mapping to fit ordersize
}

/**
 * Kill synth before starting / loading new project
 */
void SynthController::killSynth(QString newText) {
  if (scsynth != nullptr) // free all buffers before starting new project
  {
    for (auto index : order)
      oscParser->freeBuffer(index);

    oscParser->resetCounter();
    oscParser->sendQuit(); // ensure server is not already running!
    port->setEnabled(false);
  }
  start_synth->setText(newText);
}

/**
 * Update project name
 */
void SynthController::setProjectName(QString name) {
  projectName = name;

  statusBar()->showMessage(QString("Chosen project: %1").arg(projectName));
}

/**
 * Tuning change
 */
void SynthController::changeTuning(QString text) {
  bool isEqualTemp = text == "Equal Temperament";
  tuner->setTuning(TUNING_MAP[text],
                   isEqualTemp ? equal_temperament->value() : 0.f);
  equal_temperament->setEnabled(isEqualTemp);
}

/**
 * Open project
 */
void SynthController::openProject() {
  QString chosenFilename = QFileDialog::getOpenFileName(
      this, tr("Open File"), QString(), tr("Dia-projects (*.dia)"));
  if (chosenFilename != NULL)
    fileOpen(chosenFilename);
}

/**
 * Open converter (if exists!)
 */
void SynthController::openConverter() {
  QDir binPath(QApplication::applicationDirPath());
  binPath.cd("../../Extra");

  converter = new QProcess(this);
  converter->start(binPath.path() + "/Converter", QStringList());

  connect(converter,
          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
          [=](int exitCode, QProcess::ExitStatus exitStatus) {
            open_converter->setEnabled(true);
          });

  open_converter->setEnabled(false);
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

  formatPorts();
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
 * Initilize parameters (maybe from csv or preset file) and rangemap
 */
void SynthController::initParameters() {
  QMap<QString, float> defaultValues{
      // TODO add more presets!
      {ATTACK, 64},     {RELEASE, 20}, {DECAY, 48},         {SUSTAIN, 100},
      {BUFFER_NO, 127}, {FLUTTER, 24}, {DETUNE_FACTOR, 127}};

  for (auto name : defaultRangeMap.keys())
    rangeMap[name] = defaultRangeMap[name];

  for (auto name : defaultValues.keys())
    findChildren<QDial *>(name)[0]->setValue(defaultValues[name]);
}

/**
 * Starts sc synth process. PID is stored as variable
 */
void SynthController::startScSynth() {
  killSynth("Restart server");

  outChannels = outputDevices[devices->currentText()];

  QString outputDevice = devices->currentText();
  if (outputDevice == "Built-in Output")
    outputDevice = "Built-in"; // hack needed by scsynth

  QString program =
      "/Applications/SuperCollider/SuperCollider.app/Contents/"
      "Resources/scsynth"; // TODO make into only scsynth (requires $PATH
                           // setting when not running from terminal!)
                           // TODO make into macro/constant (or variable?)
  QStringList args;
  args << "-i" << QString::number(inChannels) << "-o"
       << QString::number(outChannels) << "-u"
       << QString::number(OSC_SEND_ADDRESS) << "-R"
       << "0"
       << "-m" // memsize
       << QString::number(memorySize) << "-H"
       << outputDevice; // TODO make variable!

  scsynth = new QProcess(this);
  scsynth->setProgram(program);
  scsynth->setArguments(args);

  if (!scsynth->startDetached()) {
    qDebug() << "failed to start";
    exit(-1); // kill or try again?
  }

  if (!port->isEnabled())
    port->setEnabled(true);

  formatPorts();

  // TODO change device output formatting

  if (oscParser->sync()) // wait for boot...
  {
    int bufnum = 0;

    for (auto index : order) // load buffers
      oscParser->readBufferFromFile(bufnum++,
                                    filename.arg(index).toStdString());
    // TODO count done messages and check against order.size()
    qDebug() << oscParser->getCounter();
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
    keys[num] = nextNodeID();
    dialValues[FREQ] = tuner->getFreqFromMIDI(
        num, degreeValue, rootFreqValue); // TODO change! (tuning etc..)
    // fix tuning...
    dialValues[VELOCITY] = velocity;
    dialValues[OUT_BUS] = getPort();
    dialValues[ORDER_SIZE] = order.size() - 2; // TODO Change!
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
  updateKeys(OUT_BUS, (float)getPort());
}

/**
 * Respond to user input (dials)
 */
void SynthController::valueChanged(int idx) {
  QObject *sender = QObject::sender();
  float newValue = linearConversion(idx, rangeMap[sender->objectName()]);

  dialValues[remapNames[sender->objectName()]] = newValue; // TODO exception...
  updateKeys(remapNames[sender->objectName()], newValue);
}

/**
 * Keep track of node ids
 */
int SynthController::nextNodeID() { return ++nodeCounter; }
