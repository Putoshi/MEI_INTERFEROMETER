#ifndef audioInputOct_spectrum_h
#define audioInputOct_spectrum_h

#pragma once

#include "ofMain.h"
#include <math.h>
#include <string>
#include <string.h>
#include <iterator>

class Spectrum
{

public:
  Spectrum();
  Spectrum(ofVec2f pos, int c) {
    position = pos;
    channel = c;
  }

  void setup(float _x, float _y);
  void update();
  //void draw(float* audioInput, int size);
  void draw();
  void setSpectrum(vector<float> _vec);

  char title[255];
  int channel;

  ofVec2f position;
  ofColor color;

private:
  ofColor getColorMap(float _level = 0.0f);
  ofPixels colorMap;
  ofTexture colorMapTex;

  ofPixels spectrogramPix;
  ofTexture spectrogramTex;

  int spectrumWidth;
  int spectrumHeight;
  ofPoint pos;
  void drawSpectrogram();
  void drawFrame();
  vector<float> vec;

};

#endif
