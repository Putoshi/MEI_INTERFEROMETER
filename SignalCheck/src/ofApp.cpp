#include "ofApp.h"

using namespace std;

// PATH
const char SRC_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";		// adiファイルのパス
const char DST_FILE[] = "C:/Users/Putoshi/Documents/MEI/DaqLog/_DaqLog.bak";	// 一時保存パス


//const int AD_SAMPLING_SPEED = 44100; // 35398230 44100

// CONFIG
const int FPS = 120;								// FPS
const int CHANNELS = 6;

// FFT SETTING
const float AD_1S_N = 44643;						// ADボードの1秒ごとの標本数
const int N = 4096;									// FFT 標本数
float FFT_SPAN = 100;								// FFTする間隔 ms

const float lowFreq = 2000;							// FFTで切り出す周波数下限 Hz
const float highFreq = 4000;						// FFTで切り出す周波数上限 Hz

int deltaTime, connectTime;

std::vector<int16_t> binValues;						// バイナリ

std::vector<float *> signal(CHANNELS);				// 都度読み込んで更新される信号vector
std::vector<float *> phase(CHANNELS);				// 都度読み込んで更新される位相vector


vector<ofxFft*> fft(CHANNELS);						// FFT Class vector

vector<vector<float>> drawBins(CHANNELS), middleBins(CHANNELS), audioBins(CHANNELS);

// ループ回数
int frameCnt;

//--------------------------------------------------------------
void ofApp::setup() {
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

  // .adiファイルを開く
  bool isReaded = signalUtil.readSigned16bitIntBinary(SRC_FILE, &binValues);
  // _DaqLog.bakファイルに保存
  bool isWrote = signalUtil.writeSigned16bitIntBinary(DST_FILE, binValues);

  if (isReaded && isWrote) {
    remove(DST_FILE); // ファイル削除
  }

  init();
  signal = signalUtil.parseBinary(frameCnt, binValues);
  for (int i = 0; i < CHANNELS; ++i) {
    fft[i]->setSignal(signal[i]);
    fft[i]->getImaginary();
  }
}

//--------------------------------------------------------------
void ofApp::init() {

  frameCnt = 0;

  for (int i = 0; i < CHANNELS; i++) {

    float * sig = (float *)malloc(sizeof(float) * N);
    signal[i] = sig;

    float * pha = (float *)malloc(sizeof(float) * N);
    phase[i] = pha;

    //audioBins[i] = new(vector<float>);
    fft[i] = ofxFft::create(N, OF_FFT_WINDOW_RECTANGULAR);
    drawBins[i].resize(fft[i]->getBinSize());
    middleBins[i].resize(fft[i]->getBinSize());
    audioBins[i].resize(fft[i]->getBinSize());
  }

  plotHeight = 128;
}


//--------------------------------------------------------------
void ofApp::update() {

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
  signal = signalUtil.parseBinary(frameCnt, binValues);
  

  vector<float> maxValue(CHANNELS);
  for (int i = 0; i < CHANNELS; i++) {

    fft[i]->setSignal(signal[i]);
    fft[i]->getImaginary();

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
  phase[1] = fft[1]->getPhase();
  //fft[1]->setPolar(phase[1], fft[1]->getPhase());

  std::cerr << phase[1] << std::endl;
  for (int j = 0; j < fft[1]->getBinSize(); j++) {
    std::cerr << phase[1][j]/M_PI * 180 << std::endl;
  }

  soundMutex.lock();
  middleBins = audioBins;
  soundMutex.unlock();

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