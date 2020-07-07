#ifndef TUNING_H
#define TUNING_H

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>

#define PYTHAGOREAN_COMMA 531441.f / 524288.f
#define JUST_TUNING 0
#define EQUAL_TUNING 1

using namespace std;

class Tuning {

public:
  Tuning();
  virtual ~Tuning();
  float getFreqFromMIDI(int step, int rootDegree = 69, float rootFreq = 440.f);
  void setTuning(int tuning = EQUAL_TUNING, float degree = 12.f);

private:
  const vector<float> just{1.f,       16.f / 15.f, 9.f / 8.f,   6.f / 5.f,
                           5.f / 4.f, 4.f / 3.f,   45.f / 32.f, 3.f / 2.f,
                           8.f / 5.f, 5.f / 3.f,   9.f / 5.f,   15.f / 8.f};

  vector<float> currentTuning;
  bool justTuning = false;
  void generateEqualTemperament(float degree = 29.f);
};

#endif
