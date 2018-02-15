//
//  spectrum.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Spectrum.h"

Spectrum::Spectrum() {
 
  
}

void Spectrum::setup() {
  color.setHsb(0, 255, 255);


  colorMap.allocate(5, 255, OF_IMAGE_COLOR);
  colorMap.set(0);

  ofColor col;
  for (int i = 0; i < 255; i++) {
    col = getColorMap((float)i / 255);
    //std::cerr << col.getHex() << std::endl;

    for (int j = 0; j < 5; j++) {
      colorMap.setColor(j, i, col);
    }
  }
  
  colorMapTex.allocate(5, 255, GL_RGB);
  colorMapTex.loadData(colorMap);
}

void Spectrum::update() {
}

void Spectrum::draw() {
  colorMapTex.draw(0, 0);
  //std::cerr << "draw" << std::endl;
}

//void Spectrum::draw(float* chn, int size) {
//  ofPushStyle();
//  ofPushMatrix();
//
//  ofTranslate(position);
//
//  ofSetColor(225);
//  sprintf(title, "Channel: %i", channel);
//
//  ofDrawBitmapString(title, 4, 18);
//
//  ofSetLineWidth(1);
//  ofNoFill();
//  ofRect(0, 0, 256 * 1 / 6, 100);
//
//  ofSetColor(color);
//  ofSetLineWidth(1);
//
//  ofBeginShape();
//
//  for (int i = 0; i < size; i++) {
//    ofVertex((i / (size / 256 * 1 / 6)) + 1, 50 - chn[i] * 90.0f / 3.14f);
//  }
//  ofEndShape(false);
//
//  ofPopMatrix();
//  ofPopStyle();
//}

ofColor Spectrum::getColorMap(float _level) {
  float a = _level / 0.166666f;
  int X = floor(a);
  int Y = floor(255 * (a - X));

  //std::cerr << X << std::endl;

  int r = 0;
  int g = 0;
  int b = 0;
  switch (X) {
  case 0:
    r = 255; g = Y; b = 0; break;
  case 1:
    r = 255 - Y; g = 255; b = 0; break;
  case 2:
    r = 0; g = 255; b = Y; break;
  case 3:
    r = 0; g = 255 - Y; b = 255; break;
  case 4:
    r = Y; g = 0; b = 255; break;
  case 5:
    //r = 255; g = 0; b = 255; break;
    r = 255 - Y; g = 0; b = 255 - Y; break;
  case 6:
    r = 0; g = 0; b = 0; break;
    /*r = 255 - Y; g = 0; b = 255 - Y; break;*/
  }

  return  ofColor(r, g, b);
}
