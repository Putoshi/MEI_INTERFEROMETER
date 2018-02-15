//
//  spectrum.h
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

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

  void setup();
  void update();
  //void draw(float* audioInput, int size);
  void draw();

  char title[255];
  int channel;

  ofVec2f position;
  ofColor color;

private:
  ofColor getColorMap(float _level = 0.0f);
  ofPixels colorMap;
  ofTexture colorMapTex;

};

#endif