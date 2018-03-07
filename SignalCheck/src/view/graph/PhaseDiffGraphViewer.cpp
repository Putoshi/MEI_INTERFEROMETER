//
//  ofxGraphViewer.cpp
//
//  Created by motoi ishibashi on 12/04/05.
//

#include <iostream>

#include "PhaseDiffGraphViewer.h"
vector<float> last4PlotsAlpha;
vector<float> last4PlotsBeta;
PhaseDiffGraphViewer::PhaseDiffGraphViewer()
{
  dataAlpha = NULL;
  dataBeta = NULL;
  bufferLength = 0;
  min = -1.0;
  max = 1.0;
  width = 200.0;
  height = 100.0;
  lineColor1 = 0xffffff;
  lineColor2 = 0xffffff;
  idx = 0;
  offset = 0;

  last4PlotsAlpha.resize(4);
  last4PlotsBeta.resize(4);
  swingWidthAlpha = 0;
  swingWidthBeta = 0;

  prevValueAlpha = 0;
  prevValueBeta = 0;

  for (int i = 0; i < 4; i++) {
    last4PlotsAlpha.push_back(0.1);
    last4PlotsBeta.push_back(0.1);
  }
}

PhaseDiffGraphViewer::~PhaseDiffGraphViewer()
{
  if (dataAlpha != NULL) {
    delete dataAlpha;
  }

  if (dataBeta != NULL) {
    delete dataBeta;
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
  if (dataAlpha != NULL) {
    delete dataAlpha;
  }

  if (dataBeta != NULL) {
    delete dataBeta;
  }

  bufferLength = num;

  dataAlpha = new float[bufferLength];
  memset(dataAlpha, 0, sizeof(float)*bufferLength);

  dataBeta = new float[bufferLength];
  memset(dataBeta, 0, sizeof(float)*bufferLength);

}

void PhaseDiffGraphViewer::pushData(float _alpha, float _beta, float _peekFreq)
{
  if (peekFreq != _peekFreq) {
    peekFreq += (_peekFreq - peekFreq) / 2;
  }
  else {
    peekFreq = _peekFreq;
  }

  if (0 > peekFreq || peekFreq > 5000) {
    peekFreq = 3000.0f;
  }

  float diffSampling = 1 / (44643.0f * 8.0f) * peekFreq * 360.0f / 2;


  /*
  1-5ch 12
  */
  _alpha += diffSampling * (Const::getInstance().EAST_ANT - Const::getInstance().CENTER_ANT); //20  1-5ch 12
  _beta += diffSampling * (Const::getInstance().SOUTH_ANT - Const::getInstance().CENTER_ANT);

  // 位相反転の時に
  float widAlpha = 0;
  for (int i = 0; i < last4PlotsAlpha.size(); i++) {
    widAlpha += last4PlotsAlpha[i];
  }
  swingWidthAlpha = widAlpha / 4;

  float widBeta = 0;
  for (int i = 0; i < last4PlotsBeta.size(); i++) {
    widBeta += last4PlotsBeta[i];
  }
  swingWidthBeta = widBeta / 4;

  //std::cerr << swingWidth << std::endl;

  if (abs(swingWidthAlpha) < 10) {
    if (abs(abs(prevValueAlpha) - abs(_alpha)) > 90) {
      if (_alpha > 300) {
        _alpha -= 360;
      }
      else if (_alpha < 300) {
        _alpha += 360;
      }
    }
  }

  if (abs(swingWidthBeta) < 10) {
    if (abs(abs(prevValueBeta) - abs(_beta)) > 90) {
      if (_beta > 300) {
        _beta -= 360;
      }
      else if (_beta < 300) {
        _beta += 360;
      }
    }
  }

  last4PlotsAlpha.push_back(prevValueAlpha - _alpha);
  if (last4PlotsAlpha.size() > 4) last4PlotsAlpha.erase(last4PlotsAlpha.begin());

  last4PlotsBeta.push_back(prevValueBeta - _beta);
  if (last4PlotsBeta.size() > 4) last4PlotsBeta.erase(last4PlotsBeta.begin());

  for (int i = bufferLength - 1; i > 0; i--) {
    dataAlpha[i] = dataAlpha[i - 1];
    dataBeta[i] = dataBeta[i - 1];
  }
  idx++;
  prevValueAlpha = _alpha;
  prevValueBeta = _beta;

  dataAlpha[0] = _alpha;
  dataBeta[0] = _beta;
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
  if (dataAlpha == NULL) return;

  if (idx < offset) return;

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

  ofSetHexColor(lineColor1);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < bufferLength; i++) {
    //glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataAlpha[i + offset], min, max, 0.0, h), 0.0);
    if (bufferLength < i + offset) {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(0.0, min, max, 0.0, h), 0.0);
    }
    else {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataAlpha[i + offset], min, max, 0.0, h), 0.0);
    }
  }
  glEnd();

  ofSetHexColor(lineColor2);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < bufferLength; i++) {
    //glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataAlpha[i + offset], min, max, 0.0, h), 0.0);
    if (bufferLength < i + offset) {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(0.0, min, max, 0.0, h), 0.0);
    }
    else {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataBeta[i + offset], min, max, 0.0, h), 0.0);
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

void PhaseDiffGraphViewer::setColor(int _lineColor1, int _lineColor2)
{
  lineColor1 = _lineColor1;
  lineColor2 = _lineColor2;
}

void PhaseDiffGraphViewer::culcDiff(int _lifetime)
{
  float dispersion = 5.0f;
  float minVAlpha = dataAlpha[0];
  float maxVAlpha = dataAlpha[0];
  float minVBeta = dataBeta[0];
  float maxVBeta = dataBeta[0];

  int len = std::min(_lifetime * 20, bufferLength);
  float avgAlpha = 0;
  float avgBeta = 0;
  for (int i = len; i > 0; i--) {
    avgAlpha += dataAlpha[i - 1];
    minVAlpha = std::min(minVAlpha, dataAlpha[i - 1]);
    maxVAlpha = std::max(maxVAlpha, dataAlpha[i - 1]);

    avgBeta += dataBeta[i - 1];
    minVBeta = std::min(minVBeta, dataBeta[i - 1]);
    maxVBeta = std::max(maxVBeta, dataBeta[i - 1]);

  }
  avgAlpha /= len;
  avgBeta /= len;

  //std::cerr << std::endl;
  //秒の取得
  int s = ofGetSeconds();
  //分の取得
  int m = ofGetMinutes();
  //時の取得
  int h = ofGetHours();
  string time = ofToString(h, 0) + ":" + ofToString(m, 0) + ":" + ofToString(s, 0);
  string time2 = ofToString(h, 0) + "_" + ofToString(m, 0) + "_" + ofToString(s, 0);

  std::cerr << time << "  ";

  /*if (minVAlpha > avgAlpha - dispersion && maxVAlpha < avgAlpha + dispersion  && minVBeta > avgBeta - dispersion && maxVBeta < avgBeta + dispersion) {
    std::cerr << "◆流星検知    duration: " << (float)len * 0.025f << "s: " << "  len: " << len << "  avgAlpha:" << avgAlpha << std::endl;
    ScreenCapture::getInstance().saveScreenCapture("./tmp/", time2);
  }*/
  if (minVAlpha > avgAlpha - dispersion && maxVAlpha < avgAlpha + dispersion ) {
    std::cerr << "◆流星検知    duration: " << (float)len * 0.025f << "s: " << "  len: " << len << "  avgAlpha:" << avgAlpha << std::endl;

    if(Const::getInstance().enableCapture) ScreenCapture::getInstance().saveScreenCapture("./tmp/", time2);
  }
  else {
    std::cerr << "ばらつき多い" << "  avgAlpha:" << avgAlpha << "  min:" << minVAlpha << "  max:" << maxVAlpha << std::endl;
  }
  //std::cerr << std::endl;

  
}
