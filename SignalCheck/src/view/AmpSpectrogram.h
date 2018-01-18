#pragma once

#include "ofMain.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>

class AmpSpectrogram
{
public:
  AmpSpectrogram();
  ~AmpSpectrogram();

  void plot(vector<float>& buffer, float scale);
  void init(int _plotHeight);

private:
  int plotHeight;
};