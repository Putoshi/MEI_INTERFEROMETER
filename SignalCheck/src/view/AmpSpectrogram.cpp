#include "AmpSpectrogram.h"

using namespace std;

AmpSpectrogram::AmpSpectrogram()
{
}


AmpSpectrogram::~AmpSpectrogram()
{
}

void AmpSpectrogram::init(int _plotHeight) {
  plotHeight = _plotHeight;
}

void AmpSpectrogram::plot(vector<float>& buffer, float scale) {
  float marginY = 3;
  float offset = plotHeight / 2;
  ofNoFill();
  int n = buffer.size();  
  ofSetLineWidth(0.5);
  ofSetHexColor(0x555555);
  ofDrawRectangle(0, marginY, n, plotHeight);
  glPushMatrix();
  glTranslatef(0, plotHeight / 2 + offset + marginY, 0);

  ofSetHexColor(0xbed25e);
  ofBeginShape();
  for (int i = 0; i < n; i++) {
    ofVertex(i, sqrt(buffer[i]) * scale);
    //std::cerr << buffer[i] << std::endl;
  }
  
  ofEndShape();
  glPopMatrix();
  ofSetHexColor(0xffffff);
}