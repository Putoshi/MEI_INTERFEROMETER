#include "ofApp.h"

using namespace std;

const char SRC_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.adi";
const char DST_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/_DaqLog.bak";

const int IDX_BODY = 8 * 4 + 4;
const int AD_SAMPLING_SPEED = 353980;
const int FPS = 60;
int deltaTime, connectTime;


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(FPS);

	/*unsigned short ushortValue0 = 0xffff;
	ofLogNotice() << "value: " << ushortValue0;*/

	//adi_path = ofFilePath::getAbsolutePath("frag.frag", true);

	file_.addListener(this, &ofApp::fileEvent);
	file_.addListener(this, &ofApp::fileEvent2);
	file_.setTargetPath(SRC_FILE);


	std::vector<int16_t> binValues;

	// .adiファイルを開く
	bool isReaded = readSigned16bitIntBinary(SRC_FILE, &binValues);
	// _DaqLog.bakファイルに保存
	bool isWrote = writeSigned16bitIntBinary(DST_FILE, binValues);

	if (isReaded && isWrote) {
		remove(DST_FILE); // ファイル削除
	}

	parseBinary(binValues);


	plotHeight = 128;
	bufferSize = 2048;
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);


	/*

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

	*/

	ofBackground(0, 0, 0);
}




//--------------------------------------------------------------
void ofApp::update(){
	deltaTime = ofGetElapsedTimeMillis() - connectTime;


	if (deltaTime >= 100) {
		connectTime = ofGetElapsedTimeMillis();
		//std::cerr << connectTime << std::endl;
	}
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


void ofApp::addSignalSeg(const std::vector<int16_t>& targetVector) {

}



void ofApp::parseBinary(const std::vector<int16_t>& targetVector) {
	const size_t fileSize = targetVector.size() * 2; // int16_t (16 bit) is 2 byte.

	const int loopCnt = (fileSize - fileSize % 16) / 16;

	//std::cerr << loopCnt << " ";
	std::cerr << AD_SAMPLING_SPEED * 0.1 << " ";

	

	for (int j = 0, size = AD_SAMPLING_SPEED * 0.1; j < size; ++j) {

		std::vector<unsigned short> data(8);

		for (int k = 0, size = 8; k < size; ++k) {


			char buf[20];

			snprintf(buf, 20, "%#x", targetVector[j * 16 + k * 2 + IDX_BODY]);

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

			//data.push_back(number);
			data[k] = number;

		}
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 0]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 2]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 4]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 6]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 8]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 10]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 12]);
		//printf("%02x ", targetVector[j * 16 + IDX_BODY + 14]);
		
		//cout << endl;

		signal.add(data);
	}

	//cout << endl;
	//std::cerr << targetVector[4].data() << std::endl;
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





