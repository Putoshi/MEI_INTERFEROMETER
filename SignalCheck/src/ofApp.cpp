#include "ofApp.h"
#include "../TriggerBuffer.h"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
#include <iterator>

using namespace std;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	/*unsigned short ushortValue0 = 0xffff;
	ofLogNotice() << "value: " << ushortValue0;

	unsigned short ushortValue1 = 0x478d;
	ofLogNotice() << "value: " << ushortValue1;

	unsigned short ushortValue2 = 0x6a8d;
	ofLogNotice() << "value: " << ushortValue2;*/


	


	//adi_path = ofFilePath::getAbsolutePath("frag.frag", true);
	adi_path = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";

	file_.addListener(this, &ofApp::fileEvent);
	file_.addListener(this, &ofApp::fileEvent2);
	file_.setTargetPath(adi_path);



	/*ofLogNotice() << "value: " << 1;
	string fileName = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";
	string text = string(ofBufferFromFile(fileName));
	ofLogNotice() << "value: " << text;*/

	std::ifstream ifs("C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak", std::ios_base::in | std::ios_base::binary);
	if (ifs.fail()) {
		std::cerr << "ファイルオープンに失敗" << std::endl;
		std::exit(1);
	}else{
		std::cerr << "ファイルオープンに成功" << std::endl;

		//int ch;
		///*while (std::ifstream::traits_type::not_eof(ch = ifs.get()))
		//	std::cout << std::hex << std::uppercase << ch << ' ';*/
		//ch = ifs.getline();
		//std::cout << std::hex << std::uppercase << ch << ' ';

		std::vector<int> bufvec;
		int length;
		unique_ptr<unsigned char[]> buf;


		// ファイルの終端に移動
		ifs.seekg(0, ifs.end);
		// 終端の位置からファイルサイズを取得
		length = static_cast<int>(ifs.tellg());
		// バッファとなる配列のメモリを確保
		buf.reset(new unsigned char[length]);
		// ファイルの先頭に移動
		ifs.seekg(0, ifs.beg);
		// ファイルを読み込む
		ifs.read(reinterpret_cast<char *>(buf.get()), length);

		// 出力
		for (int i = 0; i < length; ++i) {
			printf("%x", static_cast<unsigned>(buf[i]));
		}


		//cout << "0x";
		//for (int i = 0; i < length; ++i) {
		//	//printf("%x", static_cast<unsigned>(buf[i]));
		//	bufvec.push_back(static_cast<unsigned>(buf[i]));
		//}
		//cout << endl;

		

		for (int j = 0; j < 30; ++j) {
			//printf("%x", static_cast<unsigned>(buf[i]));
			std::cout << std::hex << std::showbase << bufvec[j] << std::endl;
		}
			


	}


	

	/*int len = (int)strlen(destination.c_str());
	unsigned char data[len / 2];*/

	//ofstream wf;
	//wf.open("output", ios::trunc);

	//for (int i = 0; i < len; i += 2) {
	//	unsigned int x;
	//	sscanf((char *)(str + i), "%02x", &x);
	//	data[i / 2] = x;
	//	wf << data[i / 2];
	//}


	// FFT
	plotHeight = 128;
	bufferSize = 2048;

	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);

	drawBins.resize(fft->getBinSize());
	middleBins.resize(fft->getBinSize());
	audioBins.resize(fft->getBinSize());


	ofSoundStreamSettings settings;
	settings.setApi(ofSoundDevice::Api::MS_WASAPI);

	// or by name
	auto devices = soundStream.getMatchingDevices("default");
	if (!devices.empty()) {
		settings.setInDevice(devices[0]);
	}

	settings.setInListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 0;
	settings.numInputChannels = 2;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

	// 0 output channels,
	// 1 input channel
	// 44100 samples per second
	// [bins] samples per buffer
	// 4 num buffers (latency)

	//ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);

	ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(16, 16);

	soundMutex.lock();
	drawBins = middleBins;
	soundMutex.unlock();

	ofDrawBitmapString("Frequency Domain", 0, 0);
	plot(drawBins, -plotHeight, plotHeight / 2);
	ofPopMatrix();
	string msg = ofToString((int)ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::fileEvent(ofFile &file)
{
	std::cout << "loaded event function called" << std::endl;
}
void ofApp::fileEvent2(const void *sender, ofFile &file)
{
	std::cout << "loaded event function (with sender) called" << std::endl;
}

void ofApp::plot(vector<float>& buffer, float scale, float offset) {
	ofNoFill();
	int n = buffer.size();
	ofDrawRectangle(0, 0, n, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset, 0);
	ofBeginShape();
	for (int i = 0; i < n; i++) {
		ofVertex(i, sqrt(buffer[i]) * scale);
	}
	ofEndShape();
	glPopMatrix();
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
	float maxValue = 0;
	for (int i = 0; i < bufferSize; i++) {
		if (abs(input[i]) > maxValue) {
			maxValue = abs(input[i]);
		}
	}
	for (int i = 0; i < bufferSize; i++) {
		input[i] /= maxValue;
	}

	fft->setSignal(input);

	float* curFft = fft->getAmplitude();
	memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());

	maxValue = 0;
	for (int i = 0; i < fft->getBinSize(); i++) {
		if (abs(audioBins[i]) > maxValue) {
			maxValue = abs(audioBins[i]);
		}
	}
	for (int i = 0; i < fft->getBinSize(); i++) {
		audioBins[i] /= maxValue;
	}

	soundMutex.lock();
	middleBins = audioBins;
	soundMutex.unlock();
}