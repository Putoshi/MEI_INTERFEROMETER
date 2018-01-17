#include "ofApp.h"

using namespace std;

const char SRC_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";
const char DST_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/_DaqLog.bak";

const int IDX_BODY = 8 * 4 + 4;
//const int AD_SAMPLING_SPEED = 44100; // 35398230 44100

// CONFIG
const int CHANNELS = 6;

// FFT SETTING
const float AD_1S_N = 44643;						// ADボードの1秒ごとの標本数
const int N = 4096;
const int FPS = 60;
float FFT_SPAN = 100;								//FFTする間隔 ms

const float lowFreq = 2000;
const float highFreq = 4000;

int deltaTime, connectTime;

std::vector<int16_t> binValues;						// バイナリ

//std::vector<float *> signal(CHANNELS);

vector<ofxFft*> fft(CHANNELS);
vector<vector<float>> drawBins(CHANNELS);
vector<vector<float>> middleBins(CHANNELS);
vector<vector<float>> audioBins(CHANNELS);



// ループ回数
int frameCnt;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(FPS);
	ofBackground(3, 3, 6);

	load();

}

//--------------------------------------------------------------
void ofApp::load() {
	
	file_.addListener(this, &ofApp::fileEvent);
	file_.addListener(this, &ofApp::fileEvent2);
	file_.setTargetPath(SRC_FILE);

	// .adiファイルを開く
	bool isReaded = readSigned16bitIntBinary(SRC_FILE, &binValues);
	// _DaqLog.bakファイルに保存
	bool isWrote = writeSigned16bitIntBinary(DST_FILE, binValues);

	if (isReaded && isWrote) {
		remove(DST_FILE); // ファイル削除
	}

	init();
	parseBinary(binValues);
}

//--------------------------------------------------------------
void ofApp::init() {
	
	frameCnt = 0;

	for (int i = 0; i < CHANNELS; i++) {

		

		//audioBins[i] = new(vector<float>);
		fft[i] = ofxFft::create(N, OF_FFT_WINDOW_RECTANGULAR);
		drawBins[i].resize(fft[i]->getBinSize());
		middleBins[i].resize(fft[i]->getBinSize());
		audioBins[i].resize(fft[i]->getBinSize());
	}

	plotHeight = 128;
}


//--------------------------------------------------------------
void ofApp::update(){

	deltaTime = ofGetElapsedTimeMillis() - connectTime;
	if (deltaTime >= FFT_SPAN) {
		connectTime = ofGetElapsedTimeMillis();
		//std::cerr << (AD_1S_N * (FFT_SPAN / 1000)) << std::endl;

		if (frameCnt >= 1000 / FFT_SPAN) frameCnt = 0;
		frameCnt++;

		// ------- ループ処理 ------- //
		fftUpdate();
	}
}

void ofApp::fftUpdate() {

	//std::cerr << "fftUpdate" << std::endl;

	// Binary Analyze
	parseBinary(binValues);

	vector<float> maxValue(CHANNELS);
	for (int i = 0; i < CHANNELS; i++) {

		memcpy(&audioBins[i][0], fft[i]->getAmplitude(), sizeof(float) * fft[i]->getBinSize());
		maxValue[i] = 0;

		for (int j = 0; j < fft[i]->getBinSize(); j++) {			
			if (abs(audioBins[i][j]) > maxValue[i]) {
				maxValue[i] = abs(audioBins[i][j]);
				//std::cerr << i << std::endl; //300
			}
		}

		for (int k = 0; k < fft[i]->getBinSize(); k++) {
			audioBins[i][k] /= maxValue[i];
		}
	}

	soundMutex.lock();
	middleBins = audioBins;
	soundMutex.unlock();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(252);
	ofPushMatrix();
	ofTranslate(16, 16);
	
	soundMutex.lock();
	drawBins = middleBins;
	soundMutex.unlock();

	//std::cerr << drawBins[0].size() << std::endl;

	// 指定された周波数でvectorを切り抜いちゃう
	float sampleRate = N * (1000 / FFT_SPAN);
	int startIdx = roundf((lowFreq / sampleRate * 2) * drawBins[0].size());
	int endIdx = roundf((highFreq / sampleRate * 2) * drawBins[0].size());

	vector<vector<float>> vec(CHANNELS);
	
	for (int i = 0; i < CHANNELS; i++) {
		vector<float> _vec(endIdx - startIdx, 0);
		vec[i] = _vec;
	}

	
	string msg = ofToString((int)ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
	
	for (int j = 0; j < CHANNELS; j++)
	{
		for (int k = 0; k < vec[j].size(); k++)
		{
			vec[j][k] = drawBins[j][k + startIdx];
		}

		// 表示位置の初期化
		ofPushMatrix();
		if (j == 1) {
			ofTranslate(16, 16 + plotHeight + 30);
		}
		else if (j == 2) {
			ofTranslate(16, 16 + (plotHeight + 30) * 2);
		}
		else if (j == 3) {
			ofTranslate(16 + 220, 16);
		}
		else if (j == 4) {
			ofTranslate(16 + 220, 16 + plotHeight + 30);
		}
		else if (j == 5) {
			ofTranslate(16 + 220, 16 + (plotHeight + 30) * 2);
		}

		ofDrawBitmapString("Frequency Domain " + ofToString(j), 0, 0);
		plot(vec[j], -plotHeight);
		ofPopMatrix();
	}
}

void ofApp::plot(vector<float>& buffer, float scale) {

	float marginY = 3;
	float offset = plotHeight / 2;
	ofNoFill();
	int n = buffer.size();
	ofSetLineWidth(0.5);
	ofDrawRectangle(0, marginY, n, plotHeight);
	glPushMatrix();
	glTranslatef(0, plotHeight / 2 + offset + marginY, 0);
	ofBeginShape();

	for (int i = 0; i < n; i++) {
		ofVertex(i, sqrt(buffer[i]) * scale);
	}
	//std::cerr << buffer[n - 1] << std::endl;
	ofEndShape();
	glPopMatrix();
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


void ofApp::parseBinary(const std::vector<int16_t>& targetVector) {
	const size_t fileSize = targetVector.size() * 2; // int16_t (16 bit) is 2 byte.

	/*float * sig = (float *)malloc(sizeof(float) * N);
	signal[i] = sig;*/

	for (int j = 0, size = N; j < size; ++j) {

		float * sig = (float *)malloc(sizeof(float) * N);

		// インデックスADボードの時間単位の標本数と
		// FFTに掛ける時間単位の標本数(2のべき乗)が合わないので、
		// ADボードの標本数に合わせたインデックスを与える
		int idx = frameCnt *  roundf(AD_1S_N * (FFT_SPAN / 1000)) + j;
		//std::cerr << idx << " ";

		std::vector<unsigned short> data(8);

		for (int k = 0, size = 8; k < size; ++k) {

			char buf[20];
			snprintf(buf, 20, "%#x", targetVector[idx * 16 + k * 2 + IDX_BODY]);

			char t[5];
			strncpy(t, &buf[strlen(buf) - 4], 4);
			t[4] = '\0';            //取り出した文字数分の最後に'\0'を入れる

			char hexStr[7];
			hexStr[0] = '0';
			hexStr[1] = 'x';
			hexStr[2] = t[0];
			hexStr[3] = t[1];
			hexStr[4] = t[2];
			hexStr[5] = t[3];
			hexStr[6] = '\0';
			//puts(hexStr);

			unsigned short number = (unsigned short)strtoul(hexStr, NULL, 0);
			//std::cerr << number << " ";

			//long lon = (long)number;
			data[k] = number;

		}
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 0]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 2]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 4]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 6]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 8]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 10]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 12]);
		//printf("%02x ", targetVector[idx * 16 + IDX_BODY + 14]);

		//for (int i = 0; i < CHANNELS; ++i) {
		//	long lon = (long)data[i];
		//	float flo = (float)lon;
		//	signal[i][j] = (flo - 65535 / 2) / (65535 / 2);
		//}

		long lon = (long)data[0];
		float flo = (float)lon;
		signal[j] = (flo - 65535 / 2) / (65535 / 2);
		//std::cerr << sig[j] << std::endl;
	}

	/*for (int l = 0; l < CHANNELS; ++l) {
		fft[l]->setSignal(signal[l]);
		fft[l]->getImaginary();
	}*/
	fft[0]->setSignal(signal[0]);
	fft[0]->getImaginary();

	//cout << endl;
}



bool ofApp::checkIsLittleEndian() {
	const int16_t value = 1;
	return (*(char*)&value) ? true : false;
}

const size_t ofApp::getFileByteSize(std::ifstream& file) {
	file.seekg(0, std::ios::end);
	const size_t fileSize = (size_t)file.tellg();
	file.seekg(0, std::ios::beg);

	return fileSize;
}

void ofApp::convertSigned16bitIntEndian(std::vector<int16_t>* targetVector) {
	for (auto& value : *targetVector) {
		value = (value << 8) | ((value >> 8) & 0xFF); // The most left value does not change even if bit-shifted, so the mask by 0xFF is necessary.
	}
}

bool ofApp::readSigned16bitIntBinary(const std::string& file_full_path, std::vector<int16_t>* targetVector) {
	std::ifstream file(file_full_path, std::ios::in | std::ios::binary);
	if (!file) {
		std::cout << "Error: The file path is incorrect. There is no file." << std::endl;
		return false;
	}
	const size_t fileSize = getFileByteSize(file);
	targetVector->clear();
	targetVector->resize(fileSize / 2); // 16 bit is 2 bytes.

	file.read((char*)targetVector->data(), fileSize);
	/*if (checkIsLlittleEndian()) {
	std::cout << "is Little endian" << std::endl;
	convertSigned16bitIntEndian(targetVector);
	}
	else {
	std::cout << "is Big endian" << std::endl;
	}*/

	//convertSigned16bitIntEndian(targetVector);
	file.close();
	return true;
}

bool ofApp::writeSigned16bitIntBinary(const std::string& file_full_path, const std::vector<int16_t>& targetVector) {
	std::ofstream file(file_full_path, std::ios::out | std::ios::binary);
	if (!file) {
		std::cout << "Error: The file to write could not be opend." << std::endl;
		return false;
	}
	const size_t fileSize = targetVector.size() * 2; // int16_t (16 bit) is 2 byte.
	if (checkIsLittleEndian()) {
		std::vector<int16_t> temp_vector = targetVector; // deep copy.
		convertSigned16bitIntEndian(&temp_vector);
		file.write((char*)temp_vector.data(), fileSize);
	}
	else {
		file.write((char*)targetVector.data(), fileSize);
	}

	file.close();
	return true;
}





