#ifndef audioInputOct_spectrum_h
#define audioInputOct_spectrum_h

#pragma once

#include "ofMain.h"
#include "./analyze/NoiseDetect.h"
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
  int covertGrayScale(float _level);
  
  // 横のカラーマップ
  ofPixels colorMap;
  ofTexture colorMapTex;

  // スペクトログラム
  ofPixels spectrogramPix;
  ofTexture spectrogramTex;

  // 抽出スペクトログラム
  ofPixels specPickupPix;
  ofTexture specPickupTex; 

  // 直線ノイズなど除去
  ofPixels cleanPix;
  ofTexture cleanTex;
  NoiseDetect noiseDetect;


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
  vector <ofPolyline> edgeLines; // 輪郭線を格納する動的配列
  
  
  float maxValue;
  float avgValue;
  int pickupIdxY;

};

#endif
