#include "ofApp.h"

using namespace std;

// PATH
const char SRC_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";		// adi�t�@�C���̃p�X
const char DST_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/_DaqLog.bak";	// �ꎞ�ۑ��p�X


//const int AD_SAMPLING_SPEED = 44100; // 35398230 44100

// CONFIG
const int FPS = 120;								// FPS
const int CHANNELS = 6;

// FFT SETTING
const float AD_1S_N = 44643;						// AD�{�[�h��1�b���Ƃ̕W�{��
const int N = 4096;									// FFT �W�{��
float FFT_SPAN = 100;								// FFT����Ԋu ms

const float lowFreq = 2000;							// FFT�Ő؂�o�����g������ Hz
const float highFreq = 4000;						// FFT�Ő؂�o�����g����� Hz

int deltaTime, connectTime;

std::vector<int16_t> binValues;						// �o�C�i��

std::vector<float *> signal(CHANNELS);				// �s�x�ǂݍ���ōX�V�����M��vector
//vector<vector<float>> phase(CHANNELS);				// �s�x�ǂݍ���ōX�V�����ʑ�vector
vector<ofxGraphViewer> phaseViewer(CHANNELS);


vector<ofxFft*> fft(CHANNELS);						// FFT Class vector

vector<vector<float>> drawBins(CHANNELS), middleBins(CHANNELS), audioBins(CHANNELS), phase(CHANNELS);

// ���[�v��
int frameCnt;

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableSmoothing();
  ofSetVerticalSync(false);
  ofSetFrameRate(FPS);
  ofBackground(3, 3, 6);

  load();
}

//--------------------------------------------------------------
void ofApp::load() {

  file_.addListener(this, &ofApp::fileEvent);
  file_.addListener(this, &ofApp::fileEvent2);
  file_.setTargetPath(SRC_FILE);


  signalUtil.CHANNELS = CHANNELS;
  signalUtil.AD_1S_N = AD_1S_N;
  signalUtil.N = N;
  signalUtil.FFT_SPAN = FFT_SPAN;

  // .adi�t�@�C�����J��
  bool isReaded = signalUtil.readSigned16bitIntBinary(SRC_FILE, &binValues);
  // _DaqLog.bak�t�@�C���ɕۑ�
  bool isWrote = signalUtil.writeSigned16bitIntBinary(DST_FILE, binValues);

  if (isReaded && isWrote) {
    remove(DST_FILE); // �t�@�C���폜
  }

  init();
}

//--------------------------------------------------------------
void ofApp::init() {
  frameCnt = 0;

  for (int i = 0; i < CHANNELS; i++) {

    float * sig = (float *)malloc(sizeof(float) * N);
    signal[i] = sig;

    fft[i] = ofxFft::create(N, OF_FFT_WINDOW_RECTANGULAR);
    drawBins[i].resize(fft[i]->getBinSize());
    middleBins[i].resize(fft[i]->getBinSize());
    audioBins[i].resize(fft[i]->getBinSize());

    phase[i].resize(fft[i]->getBinSize());

    free(sig);  // �������J��
  }

  plotHeight = 128;

  // �M�������̏�����
  signal = signalUtil.parseBinary(frameCnt, binValues);
  for (int i = 0; i < CHANNELS; ++i) {
    fft[i]->setSignal(signal[i]);
    fft[i]->getImaginary();

    //phaseViewer[i].setup(fft[0]->getBinSize()/20);
    phaseViewer[i].setup(80);
    phaseViewer[i].setRange(-1.0, 1.0);
    phaseViewer[i].setSize(800, 100);
  }

  signalMemRelease();  // �������J��

  ampSpectrogram.init(plotHeight);
}


//--------------------------------------------------------------
void ofApp::update() {

  deltaTime = ofGetElapsedTimeMillis() - connectTime;
  if (deltaTime >= FFT_SPAN) {
    connectTime = ofGetElapsedTimeMillis();
    //std::cerr << (AD_1S_N * (FFT_SPAN / 1000)) << std::endl;

    if (frameCnt >= 1000 / FFT_SPAN) frameCnt = 0;
    frameCnt++;

    // ------- ���[�v���� ------- //
    fftUpdate();
  }

  for (int i = 0; i < CHANNELS; i++) {
    phaseViewer[i].pushData(*phase[i].erase(phase[i].begin()) / M_PI);
  }
  //float d1 = ofRandom(-1.0, 1.0);
}

void ofApp::fftUpdate() {

  //std::cerr << "fftUpdate" << std::endl;

  // Binary Analyze
  signal = signalUtil.parseBinary(frameCnt, binValues);

  vector<float> maxValue(CHANNELS);
  for (int i = 0; i < CHANNELS; i++) {

    fft[i]->setSignal(signal[i]);
    fft[i]->getImaginary();

    memcpy(&audioBins[i][0], fft[i]->getAmplitude(), sizeof(float) * fft[i]->getBinSize());
    maxValue[i] = 0;

    const int n = fft[i]->getBinSize();
    //std::cerr << n << std::endl;
    phase[i].assign(fft[i]->getPhase(), fft[i]->getPhase() + n);


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

  signalMemRelease();
  std::vector<float>().swap(maxValue); // �������J��

  soundMutex.lock();
  std::vector<vector<float>>().swap(middleBins); // �������J��
  middleBins = audioBins;
  soundMutex.unlock();

  float* p;
  p = new float[fft[1]->getBinSize()];
  memcpy(p, fft[1]->getPhase(), sizeof(float) * fft[1]->getBinSize());
  /*for (int k = 0; k < fft[1]->getBinSize(); k++) {
    std::cerr << p[k] << std::endl;
  }*/
  delete p;
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofSetColor(252);
  ofPushMatrix();
  ofTranslate(16, 16);

  soundMutex.lock();
  drawBins = middleBins;
  soundMutex.unlock();

  //std::cerr << drawBins[0].size() << std::endl;

  // �w�肳�ꂽ���g����vector��؂蔲�����Ⴄ
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

    // �\���ʒu�̏�����
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
    ampSpectrogram.plot(vec[j], -plotHeight);
    ofPopMatrix();

    phaseViewer[j].draw(500, (100 + 16)*j + 30);
  }

  ofSetColor(255, 255, 255);
  //ofDrawBitmapString("graph 1 <random number>", 600, 316);
  //ofDrawBitmapString("graph 2 <frame number % 1000>", 600, 520);

  std::vector<vector<float>>().swap(drawBins); // �������J��
}

void ofApp::signalMemRelease() {
  for (int i = 0; i < CHANNELS; i++) {
    free(signal[i]);
  }
  std::vector<float *>().swap(signal); // �������J��
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


void ofApp::fileEvent(ofFile &file)
{
  std::cout << "loaded event function called" << std::endl;
}
void ofApp::fileEvent2(const void *sender, ofFile &file)
{
  std::cout << "loaded event function (with sender) called" << std::endl;
}