#include "ofApp.h"

using namespace std;

// PATH
char * SRC_FILE = "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog.bak";		// adiファイルのパス
char * DST_FILE = "C:/Users/Putoshi/Documents/MEI/DaqLog/_DaqLog.bak";	// 一時保存パス


//const int AD_SAMPLING_SPEED = 44100; // 35398230 44100

// CONFIG
const int FPS = 120;								// FPS
const int CHANNELS = 7;

// COLOR
const int COLOR_SIGNAL = 0x00dd44;
const int COLOR_PHASE = 0x00ddc7;
const int COLOR_PHASEDIFF = 0xdd00b6;

// FFT SETTING
const float AD_1S_N = 44643;						// ADボードの1秒ごとの標本数
const int N = 4096 / 2 / 2;									// FFT 標本数 1024
const float FFT_SPAN = 100 / 2 / 2;								// FFTする間隔 ms
const float SPECTROGRAM_FFT_SPAN = 500;								// FFTする間隔 ms

const float lowFreq = 2000;							// FFTで切り出す周波数下限 Hz
const float highFreq = 4000;						// FFTで切り出す周波数上限 Hz

// PhaseDiff Ch
const int phaseDiffCh[2] = {0, 4};

int deltaTime, connectTime;

int spectrogramTimeCnt = 0;  // スペクトログラムだけ別ループなのでその時間計測の変数
//float * signalForSpectrogram[10260];
float signalForSpectrogram[20480];
ofxFft * fftForSpectrogram;

std::vector<float *> signal(CHANNELS);				// 都度読み込んで更新される信号vector
//vector<vector<float>> phase(CHANNELS);				// 都度読み込んで更新される位相vector
vector<ofxGraphViewer> phaseViewer(CHANNELS);
vector<ofxGraphViewer> signalViewer(CHANNELS);

vector<PhaseDiffGraphViewer> phaseDiffViewer(CHANNELS);


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

  isLabelVisible = false;
  font.loadFont("ufonts.com_grotesquemtt.ttf", 8);

  load();
}

//--------------------------------------------------------------
void ofApp::load() {

  //file_.addListener(this, &ofApp::fileEvent);
  //file_.addListener(this, &ofApp::fileEvent2);
  //file_.setTargetPath(SRC_FILE);

  signalUtil.CHANNELS = CHANNELS;
  signalUtil.AD_1S_N = AD_1S_N;
  signalUtil.N = N;
  signalUtil.FFT_SPAN = FFT_SPAN;
  signalUtil.SRC_FILE = SRC_FILE;
  signalUtil.DST_FILE = DST_FILE;

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
  spectrums.setup(870, 175);

  // 位相差 0-1
  phaseDiffViewer[0].setup(400);
  phaseDiffViewer[0].setRange(-180.0, 180.0);
  phaseDiffViewer[0].setSize(800, plotHeight);
  phaseDiffViewer[0].setColor(COLOR_PHASEDIFF);

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
  float sampleRate = N * (1000 / FFT_SPAN);
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
  phaseDiffViewer[0].pushData((phase[phaseDiffCh[0]][maxIdxForPhase] - phase[phaseDiffCh[1]][maxIdxForPhase]) * 180 / M_PI, peekFreq);


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
  
  phaseDiffViewer[0].draw(870, marginTop + (plotHeight + 30) * 0);

  //std::cerr << (endIdx - startIdx) << std::endl;
  ofSetColor(255, 255, 255);

  if(isLabelVisible) drawLabel();

  spectrums.draw();

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

void ofApp::spectrogramFftUpdate() {

  fftForSpectrogram->setSignal(signalForSpectrogram);

  // 指定された周波数でvectorを切り抜いちゃう
  float sampleRate = N * (1000 / FFT_SPAN);
  int startIdxForSpectrogram = roundf((3000 / sampleRate * 2) * fftForSpectrogram->getBinSize());
  int endIdxForSpectrogram = roundf((highFreq / sampleRate * 2) * fftForSpectrogram->getBinSize());
  vector<float> vecForSpectrogram(endIdxForSpectrogram - startIdxForSpectrogram, 0);


  float max = 0;
  int maxIdx = 0;
  for (int k = 0; k < vecForSpectrogram.size(); k++)
  {
    //fftForSpectrogram->getAmplitude()
    vecForSpectrogram[k] = fftForSpectrogram->getAmplitude()[k + startIdxForSpectrogram];

    if (max < vecForSpectrogram[k]) {
      max = vecForSpectrogram[k];
      maxIdx = k;
    }
  }

  peekFreq = (float)maxIdx / ((float)endIdxForSpectrogram - (float)startIdxForSpectrogram) * (highFreq - 3000) + 3000;

  //std::cerr << peekFreq << std::endl;
  spectrums.setSpectrum(vecForSpectrogram);

}

void ofApp::drawLabel() {
  ofPushMatrix();
  //ofSetHexColor(0xbbbbbb);
  font.drawString("Signal 1.25V", 157, marginTop - 3);
  font.drawString("Freq 2-4kHz", 250 - 8, marginTop - 3);
  font.drawString("Phase +-180", 325, marginTop - 3);
  font.drawString("PhaseDifference +-180", 870, marginTop - 3);
  font.drawString("Spectrogram 3-4kHz", 870, 175 - 3);

  
  //ofDrawBitmapString("Signal 1.25V ", 117, marginTop - 3);
  //ofDrawBitmapString("Freq 2-4kHz", 250, marginTop - 3);
  //ofDrawBitmapString("Phase +-180", 330, marginTop - 3);
  //ofDrawBitmapString("PhaseDifference  +-180", 870, marginTop - 3);
  //ofSetHexColor(0xffffff);

  ofPopMatrix();
}

