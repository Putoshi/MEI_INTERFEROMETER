#include "ofApp.h"

using namespace std;

//const int AD_SAMPLING_SPEED = 44100; // 35398230 44100

// CONFIG
const int FPS = 120;								// FPS
const int CHANNELS = 7;

// COLOR
const int COLOR_SIGNAL = 0x68e4a3;
const int COLOR_PHASE = 0x737cbd;
const int COLOR_PHASEDIFF1 = 0xe568a2;
const int COLOR_PHASEDIFF2 = 0xe5a068;//0x68e5d9

// FFT SETTING
const float AD_1S_N = 44643;						// ADボードの1秒ごとの標本数
const int N = 1024;									// FFT 標本数
const float FFT_SPAN = 25;								// FFTする間隔 ms
const float SPECTROGRAM_FFT_SPAN = 500;								// FFTする間隔 ms

const float lowFreq = 2000;							// FFTで切り出す周波数下限 Hz
const float highFreq = 4000;						// FFTで切り出す周波数上限 Hz

const float sampleRate = N * (1000 / FFT_SPAN);

// PhaseDiff Ch
int phaseDiffChAlpha[2];
int phaseDiffChBeta[2];

int deltaTime, connectTime;

int spectrogramTimeCnt = 0;  // スペクトログラムだけ別ループなのでその時間計測の変数
float signalForSpectrogram[20480];
ofxFft * fftForSpectrogram;

vector<float *> signal(CHANNELS);				// 都度読み込んで更新される信号vector
vector<SimpleGraphViewer> phaseViewer(CHANNELS);
vector<SimpleGraphViewer> signalViewer(CHANNELS);

PhaseDiffGraphViewer phaseDiffViewer;


vector<ofxFft*> fft(CHANNELS);						// FFT Class vector

vector<vector<float>> drawBins(CHANNELS), middleBins(CHANNELS), audioBins(CHANNELS), phase(CHANNELS), signalafterfft(CHANNELS);

// ループ回数
int frameCnt;

//--------------------------------------------------------------
void ofApp::setup() {
  ofEnableSmoothing();
  ofSetVerticalSync(false);
  ofSetFrameRate(FPS);
  ofBackground(1, 3, 5);

  phaseDiffChAlpha[0] = Const::getInstance().CENTER_ANT;
  phaseDiffChAlpha[1] = Const::getInstance().EAST_ANT;
  phaseDiffChBeta[0] = Const::getInstance().CENTER_ANT;
  phaseDiffChBeta[1] = Const::getInstance().SOUTH_ANT;

  gui.setup();
  gui.setPosition(ofPoint(1920 - 220, 0));
  gui.add(binaryOffset.setup("Signal Offset", 0, 0, 100));

  isLabelVisible = true;
  font.loadFont("ufonts.com_grotesquemt.ttf", 9);

  load();
}

//--------------------------------------------------------------
void ofApp::load() {

  //file_.addListener(this, &ofApp::fileEvent);
  //file_.addListener(this, &ofApp::fileEvent2);
  //file_.setTargetPath(Const::getInstance().SRC_PATH);

  EventDispatcher* eventDispatcher = EventManager::getInstance().getEventDispatcher();

  // 流星エコー検知イベント
  eventDispatcher->addEventListener(Event::ECHO_DETECT, this, detectFunc);

  signalUtil.CHANNELS = CHANNELS;
  signalUtil.AD_1S_N = AD_1S_N;
  signalUtil.N = N;
  signalUtil.FFT_SPAN = FFT_SPAN;
  signalUtil.init();

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

    free(sig);  // メモリ開放
  }

  plotHeight = 100;
  marginTop = 45;

  // 信号処理の初期化
  signal = signalUtil.parseBinary(frameCnt);
  for (int i = 0; i < CHANNELS; ++i) {
    fft[i]->setSignal(signal[i]);
    fft[i]->getImaginary();

    phaseViewer[i].setup(200);
    phaseViewer[i].setRange(-1.0, 1.0);
    phaseViewer[i].setSize(400, plotHeight);
    phaseViewer[i].setColor(COLOR_PHASE);

    signalViewer[i].setup(200);
    signalViewer[i].setRange(-1.0, 1.0);
    signalViewer[i].setSize(200, plotHeight);
    signalViewer[i].setColor(COLOR_SIGNAL);
  }

  // スペクトログラムの処理
  int len = fft[0]->getBinSize() * ((int)SPECTROGRAM_FFT_SPAN / FFT_SPAN);
  int _N = (N * (int)SPECTROGRAM_FFT_SPAN / FFT_SPAN);

  fftForSpectrogram = ofxFft::create(N * ((int)SPECTROGRAM_FFT_SPAN / FFT_SPAN), OF_FFT_WINDOW_RECTANGULAR);
  spectrums = Spectrum(ofVec2f(20, 20), 0);
  spectrums.setup(870, 255, highFreq, 3000);

  // 位相差 0-1
  phaseDiffViewer.setup(400);
  phaseDiffViewer.setRange(-180.0, 180.0);
  phaseDiffViewer.setSize(800, 180);
  phaseDiffViewer.setColor(COLOR_PHASEDIFF1, COLOR_PHASEDIFF2);

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

    // ------- スペクトログラム ループ処理 ------- //
    spectrogramTimeCnt++;
    if (spectrogramTimeCnt >= (int)SPECTROGRAM_FFT_SPAN / FFT_SPAN) {
      spectrogramTimeCnt = 0;
      //std::cerr << "スペクトログラム" << std::endl;
      spectrogramFftUpdate();
    }

  }

  for (int i = 0; i < CHANNELS; i++) {
    //std::cerr << signalafterfft[i].size() << std::endl;

    if (signalafterfft[i].size() >= 2049 / 2 / 2) {
      int size = signalafterfft[i].size();

      for (int j = 0; j < 2049 / 2 / 2 - 1; j++) {
        signalViewer[i].pushData(*signalafterfft[i].erase(signalafterfft[i].begin()) / M_PI);
      }

    }
  }
}

void ofApp::fftUpdate() {
  //float val = 10;
  //ofNotifyEvent(event.detectEvent, val);

  //std::cerr << "fftUpdate" << std::endl;

  // Binary Analyze
  signal = signalUtil.parseBinary(frameCnt);
  vector<float> maxValue(CHANNELS);
  for (int i = 0; i < CHANNELS; i++) {

    fft[i]->setSignal(signal[i]);

    memcpy(&audioBins[i][0], fft[i]->getAmplitude(), sizeof(float) * fft[i]->getBinSize());
    maxValue[i] = 0;

    const int n = fft[i]->getBinSize();

    phase[i].assign(fft[i]->getPhase(), fft[i]->getPhase() + n);
    signalafterfft[i].assign(fft[i]->getSignal(), fft[i]->getSignal() + n);

    for (int j = 0; j < fft[i]->getBinSize(); j++) {
      if (abs(audioBins[i][j]) > maxValue[i]) {
        maxValue[i] = abs(audioBins[i][j]);
        if (i == 0) {
          maxIdxForPhase = j;
        }
      }
    }

    // ノーマライズ
    //for (int k = 0; k < fft[i]->getBinSize(); k++) {
    //  audioBins[i][k] /= maxValue[i];
    //}

  }

  int startIdx = roundf((lowFreq / sampleRate * 2) * audioBins[0].size());
  int endIdx = roundf((highFreq / sampleRate * 2) * audioBins[0].size());
  peekFreq = (float)maxIdxForPhase / ((float)endIdx - (float)startIdx) * (highFreq - lowFreq);

  int _loopCnt = (signalafterfft[1].size() - 1) * 2;
  for (int i = 0; i < _loopCnt; i++) {
    int idx = i + _loopCnt * spectrogramTimeCnt;
    signalForSpectrogram[idx] = signal[1][i];
  }
  //std::cerr << signalForSpectrogram[10] << std::endl;

  signalMemRelease();
  std::vector<float>().swap(maxValue); // メモリ開放

  soundMutex.lock();
  std::vector<vector<float>>().swap(middleBins); // メモリ開放
  middleBins = audioBins;
  soundMutex.unlock();
}

//--------------------------------------------------------------
void ofApp::draw() {

  ofSetColor(255);

  soundMutex.lock();
  drawBins = middleBins;
  soundMutex.unlock();

  //std::cerr << drawBins[0].size() << std::endl;

  // 指定された周波数でvectorを切り抜いちゃう

  int startIdx = roundf((lowFreq / sampleRate * 2) * drawBins[0].size());
  int endIdx = roundf((highFreq / sampleRate * 2) * drawBins[0].size());
  //std::cerr << drawBins[0].size() << std::endl;

  vector<vector<float>> vec(CHANNELS);

  // index 300でPhase出してみる
  for (int i = 0; i < CHANNELS; i++) {
    vector<float> _vec(endIdx - startIdx, 0);
    vec[i] = _vec;
    phaseViewer[i].pushData(phase[i][maxIdxForPhase] / M_PI);
  }

  float _alpha = (phase[phaseDiffChAlpha[0]][maxIdxForPhase] - phase[phaseDiffChAlpha[1]][maxIdxForPhase]) * 180 / M_PI;
  float _beta = (phase[phaseDiffChBeta[0]][maxIdxForPhase] - phase[phaseDiffChBeta[1]][maxIdxForPhase]) * 180 / M_PI;
  phaseDiffViewer.pushData(_alpha, _beta, peekFreq);

  string msg = ofToString((int)ofGetFrameRate()) + " fps";
  ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);

  for (int j = 0; j < CHANNELS; j++)
  {
    for (int k = 0; k < vec[j].size(); k++)
    {
      vec[j][k] = drawBins[j][k + startIdx];
    }

    // 周波数スペクトル 表示位置の初期化
    if (isLabelVisible) {

      ofPushMatrix();
      //ofTranslate(16, 16 + (plotHeight + 30) * j);

      if (j <= 4) {
        font.drawString("Ant " + ofToString(j), 26, marginTop + (plotHeight + 30) * j - 4);
        //ofDrawBitmapString("Ant " + ofToString(j), 26, marginTop + (plotHeight + 30) * j - 4);
      }
      else if (j == 5) {
        font.drawString("GPS", 26, marginTop + (plotHeight + 30) * j - 4);
        //ofDrawBitmapString("GPS", 26, marginTop + (plotHeight + 30) * j - 4);
      }
      else if (j == 6) {
        font.drawString("AGC", 26, marginTop + (plotHeight + 30) * j - 4);
        //ofDrawBitmapString("AGC", 26, marginTop + (plotHeight + 30) * j - 4);
      }
      ofPopMatrix();
    }

    // Signal
    ofPushMatrix();
    signalViewer[j].draw(26, marginTop + (plotHeight + 30) * j);
    ofPopMatrix();

    if (j <= 4) {
      // Freq
      ofPushMatrix();
      ofTranslate(250, marginTop + (plotHeight + 30) * j - 3);
      ampSpectrogram.plot(vec[j], -plotHeight);
      ofPopMatrix();

      // Phase
      ofPushMatrix();
      phaseViewer[j].draw(325, marginTop + (plotHeight + 30) * j);
      ofPopMatrix();
    }
  }

  phaseDiffViewer.draw(870, marginTop + (plotHeight + 30) * 0);

  //std::cerr << (endIdx - startIdx) << std::endl;
  ofSetColor(255, 255, 255);

  if (isLabelVisible) drawLabel();

  spectrums.draw(peekFreq);

  std::vector<vector<float>>().swap(drawBins); // メモリ開放

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
  if (key == 'l') isLabelVisible = !isLabelVisible;
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
  //std::cout << "loaded event function called" << std::endl;
}
void ofApp::fileEvent2(const void *sender, ofFile &file)
{
  //std::cout << "loaded event function (with sender) called" << std::endl;
}

void ofApp::detectFunc(Event &event)
{
  //cout << "[event.target] : " << ofToString(event.type()).c_str() << endl;

  phaseDiffViewer.culcDiff(EventManager::getInstance().lifetime);
  EventManager::getInstance().lifetime = 0;
}

void ofApp::spectrogramFftUpdate() {

  fftForSpectrogram->setSignal(signalForSpectrogram);

  // 指定された周波数でvectorを切り抜いちゃう
  int startIdxForSpectrogram = roundf((3000 / sampleRate * 2) * fftForSpectrogram->getBinSize());
  int endIdxForSpectrogram = roundf((highFreq / sampleRate * 2) * fftForSpectrogram->getBinSize());
  vector<float> vecForSpectrogram(endIdxForSpectrogram - startIdxForSpectrogram, 0);


  float max = 0;
  int maxIdx = 0;
  for (int k = 0; k < vecForSpectrogram.size(); k++)
  {
    vecForSpectrogram[k] = fftForSpectrogram->getAmplitude()[k + startIdxForSpectrogram];

    if (max < vecForSpectrogram[k]) {
      max = vecForSpectrogram[k];
      maxIdx = k;
    }
  }

  spectrums.setSpectrum(vecForSpectrogram);
}

void ofApp::drawLabel() {
  ofPushMatrix();
  font.drawString("Signal 1.25V", 70, marginTop - 3);
  font.drawString("Freq 2-4kHz", 250 - 2, marginTop - 3);
  font.drawString("Phase +-180", 325, marginTop - 3);
  font.drawString("PhaseDifference +-180", 870, marginTop - 3);
  font.drawString("Spectrogram 3-4kHz", 870, 255 - 3);
  font.drawString("Mono Spectrogram 100Hz", 870, 255 + 500 + 25 - 3);
  font.drawString("Echo Analyze", 870, 255 + 500 + 25 * 2 + 50 - 3);

  ofPopMatrix();
}

