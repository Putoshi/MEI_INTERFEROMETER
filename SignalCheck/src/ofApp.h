#pragma once

#include "ofMain.h"
#include "ofxWatchFile.h"
#include "ofxFft.h"
#include "./TriggerBuffer.h"
#include "./Signal.h"


#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <string.h>
#include <iterator>

class ofApp : public ofBaseApp{


	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		int plotHeight, bufferSize;

		ofxFft* fft;
		void plot(vector<float>& buffer, float scale, float offset);
		//void audioReceived(float* input, int bufferSize, int nChannels);
		ofSoundStream soundStream;

		ofMutex soundMutex;
		vector<float> drawBins, middleBins, audioBins;


	private:
		ofxWatchFile file_;
		void fileEvent(ofFile &file);
		void fileEvent2(const void *sender, ofFile &file);

		bool checkIsLittleEndian();
		const size_t getFileByteSize(std::ifstream& file);
		void convertSigned16bitIntEndian(std::vector<int16_t>* target_vector);

		bool readSigned16bitIntBinary(const std::string& file_full_path, std::vector<int16_t>* targetVector);
		bool writeSigned16bitIntBinary(const std::string& file_full_path, const std::vector<int16_t>& targetVector);
		
		void parseBinary(const std::vector<int16_t>& targetVector);

		Signal signal;

		void fftUpdate();
			
};
