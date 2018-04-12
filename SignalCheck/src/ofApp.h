#pragma once

#include "ofMain.h"
#include "ofxWatchFile.h"
#include "ofxFft.h"
#include "ofxGui.h"

#include "./config/Const.h"
#include "./TriggerBuffer.h"
#include "./SignalUtil.h"
#include "./util/DateUtil.h"
#include "./view/AmpSpectrogram.h"
#include "./view/Spectrum.h"
#include "./view/graph/PhaseDiffGraphViewer.h"
#include "./view/graph/SimpleGraphViewer.h"
#include "./event/EventManager.h"
#include "./event/Event.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>


#define _USE_MATH_DEFINES
#include <math.h>

class ofApp : public ofBaseApp {


public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  int plotHeight, bufferSize;
  float peekFreq;
  bool isLabelVisible;
  ofTrueTypeFont font;

  ofMutex soundMutex; 

private:
  ofxWatchFile file_;
  SignalUtil signalUtil;
  AmpSpectrogram ampSpectrogram;

  void init();
  void load();
  void fileEvent(ofFile &file);
  void fileEvent2(const void *sender, ofFile &file);
  static void detectFunc(Event &event);

  void fftUpdate();
  void spectrogramFftUpdate();
  void ofApp::signalMemRelease();

  Spectrum spectrums;

  

  int maxIdxForPhase;

  int marginTop;

  void drawLabel();
  void drawVGA();

  
  ofxIntSlider frameRate;
  ofxToggle enableDebug;
  ofxIntSlider binaryOffset;
  ofxToggle enableLogTxt;
  ofxToggle enableCapImg;
  ofxToggle enableSaveAdi;

  ofxIntSlider thresholdDispersion;
  ofxIntSlider thresholdBipolar;
  ofxIntSlider antPhaseDiff0;
  ofxIntSlider antPhaseDiff1;
  ofxIntSlider antPhaseDiff2;
  ofxIntSlider antPhaseDiff3;
  ofxIntSlider antPhaseDiff4;
  ofxGuiGroup gui;
  ofxGuiGroup saveGroupGui;
  ofxGuiGroup thresholdGroupGui;

  void setupGui();
  void onGuiChangeFrameRate(int & n);
  void onGuiChangeDebug(bool & pressed);
  void onGuiChangeLogTxt(bool & pressed);
  void onGuiChangeCapImg(bool & pressed);
  void onGuiChangeSaveAdi(bool & pressed);
  void onGuiChangeDispersion(int & n);
  void onGuiChangeBipolar(int & n);
  void onGuiChangeAntPhaseDiff0(int & n);
  void onGuiChangeAntPhaseDiff1(int & n);
  void onGuiChangeAntPhaseDiff2(int & n);
  void onGuiChangeAntPhaseDiff3(int & n);
  void onGuiChangeAntPhaseDiff4(int & n);

};
