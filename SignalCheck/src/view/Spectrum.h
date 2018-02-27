#ifndef audioInputOct_spectrum_h
#define audioInputOct_spectrum_h

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
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

  void setup(float _x, float _y, float _highFreq, float _lowFreq);
  void update();
  //void draw(float* audioInput, int size);
  void draw(float _peekFreq);
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

  ofPixels specPickupPix;
  ofTexture specPickupTex;

  

  int spectrumWidth;
  int spectrumHeight;
  ofPoint pos;
  void drawSpectrogram();
  void drawFrame();
  vector<float> vec;
  float highFreq;
  float lowFreq;
  float peekFreq;
  float FreqRange;
  int pickupH;
  int marginY;

  // opencv
  ofxCvColorImage colorImg; // openCvで扱うイメージ
  ofxCvGrayscaleImage grayImg; // openCvで扱うグレースケールイメージ
  ofxCvContourFinder contourFinder; // 輪郭検出用
  vector <ofPolyline> edgeLines; // 輪郭線を格納する動的配列
  
  
  float maxValue;
  float avgValue;

};

#endif
