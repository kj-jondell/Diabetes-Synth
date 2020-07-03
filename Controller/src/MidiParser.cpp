#include "MidiParser.h"

void _callback(double delta, vector<unsigned char> *message, void *userData) {
  MidiParser *parser = static_cast<MidiParser *>(userData);

  int channel = (int)message->at(0) & 0xF;
  int eventType = ((int)message->at(0) & 0xF0) >> 4;
  int num = (int)message->at(1);
  int velocity = (int)message->at(2);

  parser->callback(channel, eventType, num, velocity);
}

MidiParser::MidiParser(QObject *parent, int out_port, int out_channel)
    : QObject(parent), port(out_port), channel(out_channel) {
  midiin = new RtMidiIn();

  unsigned int nPorts = midiin->getPortCount();
  for (unsigned int i = 0; i < nPorts; i++)
    portnames.push_back(midiin->getPortName(i));

  midiin->setCallback(&_callback, (void *)this);
  midiin->openPort(0); // TODO variable
  midiin->ignoreTypes(true, true, true);
}

MidiParser::~MidiParser(void) {}

int MidiParser::getPort() { return port; }
void MidiParser::setChannel(int out_channel) { channel = out_channel; }

vector<string> MidiParser::getPortNames() { return portnames; }
void MidiParser::callback(int channel_filter, int eventType, int num,
                          int velocity) {
  // filter by chosen channel
  if (channel_filter == channel)
    switch (eventType) {
    case CC:
      emit cc(num, velocity);
      break;
    case NOTE_ON:
      emit noteOn(num, velocity);
      break;
    case NOTE_OFF:
      emit noteOff(num, velocity);
      break;
    }
}
