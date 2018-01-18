#pragma once

#include "ofMain.h"
#include "ofxWatchFile.h"
#include "ofxFft.h"
#include "./TriggerBuffer.h"
#include "./SignalUtil.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>

#define _USE_MATH_DEFINES
#include <math.h>


#define MIC 0
#define NOISE 1
#define SINE 2

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

	ofMutex soundMutex;


  void audioReceived(float* input, int bufferSize, int nChannels);
  ofSoundStream soundStream;

private:
	ofxWatchFile file_;
	SignalUtil signalUtil;
	void init();
	void load();
	void fileEvent(ofFile &file);
	void fileEvent2(const void *sender, ofFile &file);

	void fftUpdate();
	void plot(vector<float>& buffer, float scale);



  int plotHeight2, bufferSize2;

  ofxFft* fft2;

  float* audioInput;
  float* fftOutput;
  float* eqFunction;
  float* eqOutput;
  float* ifftOutput;

  float appWidth;
  float appHeight;

  int mode;

  void setup2();
  void plot2(float* array, int length, float scale, float offset);
  
  void draw2();
};
