#ifndef MIDIPARSER_H
#define MIDIPARSER_H

#include "RtMidi.h"
#include <QObject>

#define CC 0b1011
#define NOTE_ON 0b1001
#define NOTE_OFF 0b1000

using namespace std;

class MidiParser : public QObject {
  Q_OBJECT

public:
  MidiParser(QObject *parent = 0, int = 0, int = 0);
  virtual ~MidiParser();
  int getPort();
  vector<string> getPortNames();
  void callback(int channel_filter, int eventType, int num, int velocity);
  void setChannel(int = 0);

signals:
  void noteOn(int num, int velocity);
  void noteOff(int num, int velocity);

private:
  RtMidiIn *midiin;
  vector<string> portnames;
  int port, channel = 0;
};

#endif
