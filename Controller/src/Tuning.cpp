#include "Tuning.h"

Tuning::Tuning(int tuning) { this->setTuning(tuning); }
Tuning::~Tuning() {}

void Tuning::generateEqualTemperament(float degree) {
  this->currentTuning.clear();

  for (float i = 0.f; i <= degree; i++)
    this->currentTuning.push_back(pow(2.f, i / degree));
}

float Tuning::getFreqFromMIDI(int step, int rootDegree, float rootFreq) {
  if (step == rootDegree)
    return rootFreq;
  else {
    float tuningSize = this->currentTuning.size() - 1;
    int stepsAway = abs(step - rootDegree),
        octavesAway = stepsAway / ((int)tuningSize),
        degreesAway = stepsAway % ((int)tuningSize);

    if (step < rootDegree) {
      octavesAway = (octavesAway + 1) * -1;
      degreesAway = ((int)tuningSize) - degreesAway;
    }

    rootFreq *= pow(2, octavesAway);

    return rootFreq * this->currentTuning[degreesAway] *
           (justTuning ? pow(PYTHAGOREAN_COMMA, octavesAway) : 1.f);
  }
}

void Tuning::setTuning(int tuning, float degree) {
  switch (tuning) {
  case JUST_TUNING:
    this->currentTuning = just;
    break;
  case EQUAL_TUNING:
    this->generateEqualTemperament(degree);
    break;
  }

  justTuning = (tuning == JUST_TUNING);
}
