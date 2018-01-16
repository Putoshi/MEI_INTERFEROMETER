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

const char SRC_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";
const char DST_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog_.bak";
const int BUF_SIZE = 16; // バイト
const int HEADER_BUF_SIZE = 4;


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

	file_.addListener(this, &ofApp::fileEvent);
	file_.addListener(this, &ofApp::fileEvent2);
	file_.setTargetPath(SRC_FILE);



	/*ofLogNotice() << "value: " << 1;
	string fileName = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";
	string text = string(ofBufferFromFile(fileName));
	ofLogNotice() << "value: " << text;*/

	fstream src, dst;

	// ファイルを開く
	src.open(SRC_FILE, ios::in | ios::binary);

	dst.open(DST_FILE, ios::out | ios::binary);
	if (!dst.is_open()) {
		src.close();
	}


	bool error = false;
	char buf[BUF_SIZE];
	char mix_buf[BUF_SIZE];

	int read_cnt = 0;
	do {

		//char buf[BUF_SIZE];
		// 読み込み
		src.read(buf, sizeof buf); // BUF_SIZE バイトずつ
		if (src.fail() && !src.eof()) { // ! src && ! src.eof としても同じです
			error = true;
			break;
		}

		// 内容を16進ダンプ
		//std::cerr << src.gcount() << std::endl;
		for (int i = 0, size = src.gcount(); i < size; ++i) { // get count
															  //printf("%02X ", (unsigned char)buf[i]); // 2桁の16進数大文字表示 (小文字表示は %x)
															  // ↑char型のままでは負の値として扱われる可能性がある
		}
		//cout << endl;

		// データを操作
		mix_buf[0] = (unsigned char)buf[3];
		mix_buf[1] = (unsigned char)buf[2];
		mix_buf[2] = (unsigned char)buf[1];
		mix_buf[3] = (unsigned char)buf[0];
		mix_buf[4] = (unsigned char)buf[7];
		mix_buf[5] = (unsigned char)buf[6];
		mix_buf[6] = (unsigned char)buf[5];
		mix_buf[7] = (unsigned char)buf[4];
		mix_buf[8] = (unsigned char)buf[11];
		mix_buf[9] = (unsigned char)buf[10];
		mix_buf[10] = (unsigned char)buf[9];
		mix_buf[11] = (unsigned char)buf[8];
		mix_buf[12] = (unsigned char)buf[15];
		mix_buf[13] = (unsigned char)buf[14];
		mix_buf[14] = (unsigned char)buf[13];
		mix_buf[15] = (unsigned char)buf[12];


		// LOG_FRONTEND を抜いて書き込み
		if (read_cnt >= HEADER_BUF_SIZE) {
			dst.write(mix_buf, src.gcount());
		}

		read_cnt++;

		if (dst.fail()) { // ! dst としても同じです
			error = true;
			break;
		}

	} while (!src.eof()); // eof: ファイル終端にファイルポインタがあってもfalseです。
						  // 終端を越えようとしていればtrueです。そのため、input.bin が空であっても最初はfalseです
						  // よって、do-while であっても while であっても表示結果は同じで最低1ループは実行されます。
						  // ここでは現実をそのまま直感的に表現したdo-whileを採用しました。


	src.close();
	dst.close();

	if (error) {
		remove(DST_FILE); // ファイル削除 (cstdioより)
	}


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