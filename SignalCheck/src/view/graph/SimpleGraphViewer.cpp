#include <iostream>

#include "SimpleGraphViewer.h"

SimpleGraphViewer::SimpleGraphViewer()
{
  data = NULL;
  bufferLength = 0;
  min = -1.0;
  max = 1.0;
  width = 200.0;
  height = 50.0;
  lineColor = 0xd83f87;
  idx = 0;
  offset = 0;
}

SimpleGraphViewer::~SimpleGraphViewer()
{
  if (data != NULL) {
    delete data;
  }
}

void SimpleGraphViewer::setup(int _bufferLength, int _offset)
{
  bufferLength = _bufferLength;
  offset = _offset;

  this->allocate(bufferLength);

}

void SimpleGraphViewer::allocate(int num)
{
  if (data != NULL) {
    delete data;
  }

  bufferLength = num;

  data = new float[bufferLength];
  memset(data, 0, sizeof(float)*bufferLength);

}

void SimpleGraphViewer::pushData(float _d)
{
  for (int i = bufferLength - 1; i > 0; i--) {
    data[i] = data[i - 1];
  }
  idx++;

  data[0] = _d;
}

void SimpleGraphViewer::draw()
{
  draw(0.0, 0.0);
}

void SimpleGraphViewer::draw(float posx, float posy)
{
  draw(posx, posy, width, height, min, max);
}


void SimpleGraphViewer::draw(float posx, float posy, float w, float h, float min, float max)
{
  if (data == NULL) return;

  if(idx < offset) return;

  ofPushStyle();
  ofNoFill();
  ofSetLineWidth(0.5);
  ofSetHexColor(0x555555);
  //ofSetColor(255, 255, 255);
  ofRect(posx, posy, w, h);
  ofSetColor(120, 120, 120);
  ofLine(posx, posy + h / 2.0, posx + w, posy + h / 2.0);

  //ofSetColor(lineColor);
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

void SimpleGraphViewer::setSize(float _width, float _height)
{
  width = _width;
  height = _height;
}

void SimpleGraphViewer::setRange(float _min, float _max)
{
  min = _min;
  max = _max;
}

void SimpleGraphViewer::setColor(int _lineColor)
{
  lineColor = _lineColor;
}

float SimpleGraphViewer::getAvg()
{
  float total = 0;
  int cnt = 10;
  for (int i = bufferLength - 1; i > bufferLength - cnt - 1; i--) {
    total += data[i];
    //std::cerr << i << std::endl;
  }
  total /= cnt;
  return total;
}

