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
//vector<vector<float>> phase(CHANNELS);				// 都度読み込んで更新される位相vector
vector<ofxGraphViewer> phaseViewer(CHANNELS);
vector<ofxGraphViewer> signalViewer(CHANNELS);

vector<ofxGraphViewer> phaseDiffViewer(CHANNELS);


vector<ofxFft*> fft(CHANNELS);						// FFT Class vector

vector<vector<float>> drawBins(CHANNELS), middleBins(CHANNELS), audioBins(CHANNELS), phase(CHANNELS), signalafterfft(CHANNELS);

// ループ回数
int frameCnt;

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableSmoothing();
  ofSetVerticalSync(false);
  ofSetFrameRate(FPS);
  ofBackground(3, 3, 6);

  gui.setup();
  gui.setPosition(ofPoint(1920 - 220,0));
  gui.add(binaryOffset.setup("Signal Offset", 0, 0, 100));

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
    signalafterfft[i].resize(fft[i]->getBinSize());

    spectrums.push_back(Spectrum(ofVec2f(20, (i * 100 + 20)), i));
    spectrums[0].setup();

    free(sig);  // メモリ開放
  }

  plotHeight = 128;

  // 信号処理の初期化
  signal = signalUtil.parseBinary(frameCnt, binValues);
  for (int i = 0; i < CHANNELS; ++i) {
    fft[i]->setSignal(signal[i]);
    fft[i]->getImaginary();

    //phaseViewer[i].setup(fft[0]->getBinSize()/20);
    if (i == 1) {
      phaseViewer[i].setup(200,6);
    }
    else {
      phaseViewer[i].setup(200);
    }
    
    phaseViewer[i].setRange(-1.0, 1.0);
    phaseViewer[i].setSize(400, plotHeight);
    phaseViewer[i].setColor(ofColor::magenta);

    
    signalViewer[i].setup(200);
    signalViewer[i].setRange(-1.0, 1.0);
    signalViewer[i].setSize(200, plotHeight);
    signalViewer[i].setColor(ofColor::green);
  }

  // 位相差 0-1
  phaseDiffViewer[0].setup(400);
  phaseDiffViewer[0].setRange(-180.0 * 2, 180.0 * 2);
  phaseDiffViewer[0].setSize(800, plotHeight);
  phaseDiffViewer[0].setColor(ofColor::blueViolet);

  signalMemRelease();  // メモリ開放

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

    // ------- ループ処理 ------- //
    fftUpdate();
  }

  for (int i = 0; i < CHANNELS; i++) {
    //std::cerr << signalafterfft[i].size() << std::endl;

    if (signalafterfft[i].size() >= 2049) {
      int size = signalafterfft[i].size();
      
      for (int j = 0; j < 2049 - 1; j++) {

        signalViewer[i].pushData(*signalafterfft[i].erase(signalafterfft[i].begin()) / M_PI);

        //if (i == 0) {
        //  signalViewer[i].pushData(*signalafterfft[i].erase(signalafterfft[i].begin() + binaryOffset) / M_PI);
        //}
        //else {
        //  signalViewer[i].pushData(*signalafterfft[i].erase(signalafterfft[i].begin()) / M_PI);
        //}
      }
      
    } 
  }
  //std::cerr << binaryOffset << std::endl;
  //float d1 = ofRandom(-1.0, 1.0);

  
}

void ofApp::fftUpdate() {

  //std::cerr << "fftUpdate" << std::endl;

  // Binary Analyze
  signal = signalUtil.parseBinary(frameCnt, binValues);

  vector<float> maxValue(CHANNELS);
  for (int i = 0; i < CHANNELS; i++) {

    fft[i]->setSignal(signal[i]);

    memcpy(&audioBins[i][0], fft[i]->getAmplitude(), sizeof(float) * fft[i]->getBinSize());
    maxValue[i] = 0;

    const int n = fft[i]->getBinSize();
    //std::cerr << n << std::endl;

    phase[i].assign(fft[i]->getPhase(), fft[i]->getPhase() + n);
    signalafterfft[i].assign(fft[i]->getSignal(), fft[i]->getSignal() + n);
    //phase[i].assign(fft[i]->getSignal(), fft[i]->getSignal() + n);
    //phase[i].assign(fft[i]->getReal(), fft[i]->getReal() + n);

    for (int j = 0; j < fft[i]->getBinSize(); j++) {
      if (abs(audioBins[i][j]) > maxValue[i]) {
        maxValue[i] = abs(audioBins[i][j]);
        //std::cerr << j << std::endl; //300
      }
    }

    // ノーマライズ
    //for (int k = 0; k < fft[i]->getBinSize(); k++) {
    //  audioBins[i][k] /= maxValue[i];
    //}

  }


  signalMemRelease();
  std::vector<float>().swap(maxValue); // メモリ開放

  soundMutex.lock();
  std::vector<vector<float>>().swap(middleBins); // メモリ開放
  middleBins = audioBins;
  soundMutex.unlock();

  //float* p;
  //p = new float[fft[1]->getBinSize()];
  //memcpy(p, fft[1]->getPhase(), sizeof(float) * fft[1]->getBinSize());
  ///*for (int k = 0; k < fft[1]->getBinSize(); k++) {
  //  std::cerr << p[k] << std::endl;
  //}*/
  //delete p;
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

    phaseViewer[i].pushData(phase[i][300] / M_PI);
  }

  phaseDiffViewer[0].pushData((phase[1][300] - phase[5][300]) * 180 / M_PI);


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
      //ofTranslate(16 + 220, 16);
      ofTranslate(16, 16 + (plotHeight + 30) * 3);
    }
    else if (j == 4) {
      //ofTranslate(16 + 220, 16 + plotHeight + 30);
      ofTranslate(16, 16 + (plotHeight + 30) * 4);
    }
    else if (j == 5) {
      //ofTranslate(16 + 220, 16 + (plotHeight + 30) * 2);
      ofTranslate(16, 16 + (plotHeight + 30) * 5);
    }

    ofDrawBitmapString("Channel " + ofToString(j), 0, 0);
    ampSpectrogram.plot(vec[j], -plotHeight);
    ofPopMatrix();

    phaseViewer[j].draw(450, 20 + (plotHeight + 30)*j);

    signalViewer[j].draw(230, 20 + (plotHeight + 30)*j);

  }

  phaseDiffViewer[0].draw(870, 20 + (plotHeight + 30) * 0);



  //spectrums[1].draw(fft[1]->getPhase(), fft[1]->getBinSize());

  ofSetColor(255, 255, 255);
  //ofDrawBitmapString("graph 1 <random number>", 600, 316);
  //ofDrawBitmapString("graph 2 <frame number % 1000>", 600, 520);

  std::vector<vector<float>>().swap(drawBins); // メモリ開放


  spectrums[0].draw();

  // GUIを表示
  gui.draw();
}

void ofApp::signalMemRelease() {
  for (int i = 0; i < CHANNELS; i++) {
    free(signal[i]);
  }
  std::vector<float *>().swap(signal); // メモリ開放
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