//
//  ofxGraphViewer.cpp
//
//  Created by motoi ishibashi on 12/04/05.
//

#include <iostream>

#include "PhaseDiffGraphViewer.h"
vector<float> last4Plots;
PhaseDiffGraphViewer::PhaseDiffGraphViewer()
{
  data = NULL;
  bufferLength = 0;
  min = -1.0;
  max = 1.0;
  width = 200.0;
  height = 100.0;
  lineColor = 0xffffff;
  idx = 0;
  offset = 0;

  last4Plots.resize(4);
  swingWidth = 0;

  prevValue = 0;

  for (int i = 0; i < 4; i++) {
    last4Plots.push_back(0.1);
  }
}

PhaseDiffGraphViewer::~PhaseDiffGraphViewer()
{
  if (data != NULL) {
    delete data;
  }

}

void PhaseDiffGraphViewer::setup(int _bufferLength, int _offset)
{
  bufferLength = _bufferLength;
  offset = _offset;

  this->allocate(bufferLength);

}

void PhaseDiffGraphViewer::allocate(int num)
{
  if (data != NULL) {
    delete data;
  }

  bufferLength = num;

  data = new float[bufferLength];
  memset(data, 0, sizeof(float)*bufferLength);

}

void PhaseDiffGraphViewer::pushData(float _d, float _peekFreq)
{
 if(peekFreq != _peekFreq){
    peekFreq += (_peekFreq - peekFreq) / 2;
  }
  else {
    peekFreq = _peekFreq;
  }

  if (0 > peekFreq || peekFreq > 5000) {
    peekFreq = 3000.0f;
  }
  //std::cerr << peekFreq << std::endl;

  //peekFreq = 3736.0f;

  float diffSampling = 1 / (44643.0f * 8.0f) * peekFreq * 360.0f / 2;
  //std::cerr << diffSampling << std::endl;


  /*
  1-5ch 12
  */
  _d += diffSampling * 4; //20  1-5ch 12

  // ˆÊ‘Š”½“]‚ÌŽž‚É
  float wid = 0;
  for (int i = 0; i < last4Plots.size(); i++) {
    wid += last4Plots[i];
  }
  swingWidth = wid / 4;
  //std::cerr << swingWidth << std::endl;

  if (abs(swingWidth) < 10) {
    if (abs(abs(prevValue) - abs(_d)) > 90) {
      if (_d > 300) {
        _d -= 360;
      }
      else if (_d < 300) {
        _d += 360;
      }
    }
  }
  
  last4Plots.push_back(prevValue - _d);
  if (last4Plots.size() > 4) last4Plots.erase(last4Plots.begin());

  
  //std::cerr << last4Plots.size() << std::endl;


  for (int i = bufferLength - 1; i > 0; i--) {
    data[i] = data[i - 1];
  }
  idx++;

  prevValue = _d;

  data[0] = _d;

  //std::cerr << _d << std::endl;
}

void PhaseDiffGraphViewer::draw()
{
  draw(0.0, 0.0);
}

void PhaseDiffGraphViewer::draw(float posx, float posy)
{
  draw(posx, posy, width, height, min, max);
}


void PhaseDiffGraphViewer::draw(float posx, float posy, float w, float h, float min, float max)
{
  if (data == NULL) return;

  if(idx < offset) return;

  ofPushStyle();
  ofNoFill();
  ofSetLineWidth(0.5);
  ofSetHexColor(0x555555);

  ofSetColor(30, 30, 30);
  for (int i = 0; i < 6; i++) {
    ofLine(posx, posy + h * (i + 1) / 6, posx + w, posy + h * (i + 1) / 6);
  }

  ofSetColor(120, 120, 120);
  ofRect(posx, posy, w, h);
  ofSetColor(120, 120, 120);
  ofLine(posx, posy + h / 2.0, posx + w, posy + h / 2.0);

  ofSetHexColor(lineColor);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < bufferLength; i++) {
    //glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(data[i + offset], min, max, 0.0, h), 0.0);
    if (bufferLength < i + offset) {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(0.0, min, max, 0.0, h), 0.0);
    }
    else {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(data[i + offset], min, max, 0.0, h), 0.0);
    }
  }
  glEnd();

  ofPopStyle();
}

void PhaseDiffGraphViewer::setSize(float _width, float _height)
{
  width = _width;
  height = _height;
}

void PhaseDiffGraphViewer::setRange(float _min, float _max)
{
  min = _min;
  max = _max;
}

void PhaseDiffGraphViewer::setColor(int _lineColor)
{
  lineColor = _lineColor;
}
