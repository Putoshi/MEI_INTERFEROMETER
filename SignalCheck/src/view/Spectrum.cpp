#include "Spectrum.h"


Spectrum::Spectrum() {
}

void Spectrum::setup(float _x, float _y, float _highFreq, float _lowFreq) {
  pos.x = _x;
  pos.y = _y;
  highFreq = _highFreq;
  lowFreq = _lowFreq;
  spectrumHeight = (int)((highFreq - lowFreq) * 0.5f);
  spectrumWidth = 600;
  FreqRange = 100.0f; // Hz
  peekFreq = 3000.0f;
  marginY = 25;
  maxValue = 0;
  avgValue = 0;
  
  color.setHsb(0, 255, 255);

  colorMap.allocate(5, spectrumHeight, OF_IMAGE_COLOR);
  colorMap.set(0);

  ofColor col;
  for (int i = 0; i < spectrumHeight; i++) {
    col = getColorMap(1 - (float)i / spectrumHeight);
    //std::cerr << col.getHex() << std::endl;
    for (int j = 0; j < 5; j++) {
      colorMap.setColor(j, i, col);
    }
  }
  
  colorMapTex.allocate(5, spectrumHeight, GL_RGB);
  colorMapTex.loadData(colorMap);


  // スペクトログラム
  spectrogramPix.allocate(spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  spectrogramPix.set(0);

  for (int i = 0; i < spectrumHeight; i++) {
    for (int j = 0; j < spectrumWidth; j++) {
      spectrogramPix.setColor(i, j, ofColor::black);
    }
  }
  spectrogramTex.allocate(spectrumHeight, spectrumWidth, GL_RGB);
  spectrogramTex.loadData(spectrogramPix);


  // スペクトログラム[抽出]
  pickupH = (int)((FreqRange / (_highFreq - _lowFreq))  * spectrumHeight);
  specPickupPix.allocate(pickupH, spectrumWidth, OF_IMAGE_COLOR);
  specPickupPix.set(0);
  for (int i = 0; i < pickupH; i++) {
    for (int j = 0; j < spectrumWidth; j++) {
      specPickupPix.setColor(i, j, ofColor::black);
    }
  }
  specPickupTex.allocate(pickupH, spectrumWidth, GL_RGB);
  specPickupTex.loadData(specPickupPix);


  // openCvで解析する領域確保
  colorImg.allocate(pickupH, spectrumWidth);

  // グレースケール
  grayImg.allocate(pickupH, spectrumWidth);
}

void Spectrum::update() {
}

void Spectrum::draw(float _peekFreq) {

  if (peekFreq != _peekFreq) {
    peekFreq += (_peekFreq - peekFreq) / 2;
  }
  else {
    peekFreq = _peekFreq;
  }

  if (0 > peekFreq || peekFreq > 5000) {
    peekFreq = 3000.0f;
  }
  
  colorMapTex.draw(pos.x + spectrumWidth + 10, pos.y);
  drawSpectrogram();
  drawFrame();

  //std::cerr << peekFreq << std::endl;

}

void Spectrum::setSpectrum(vector<float> _vec) {
  vec = _vec;

  // MaxとAvgを取得
  float _avgValue = 0;
  float _maxValue = 0.45;
  for (int i = 0; i < vec.size(); i++)
  {
    _avgValue += vec[i];
    if (vec[i] > _maxValue) {
      _maxValue = vec[i];
    }
  }
  avgValue = _avgValue / vec.size();
  maxValue += (_maxValue - maxValue) / 4;
  //std::cerr << avgValue + " : " + maxValue << std::endl;
  //std::cerr << maxValue << std::endl;

  unsigned char * pixels = spectrogramPix.getPixels();

  unsigned char pixs[600 * 500 * 3];
  ofImage *img;
  img = new ofImage;
  img->clear();

  unsigned char pixsPickup[600 * 50 * 3];
  ofImage *imgPickup;
  imgPickup = new ofImage;
  imgPickup->clear();
  int pickupIdxY = (peekFreq - lowFreq) / (highFreq - lowFreq) * spectrumHeight;

  int len = 500;
  int pickupIdx = 0;
  for (int i = 0; i < spectrumWidth * spectrumHeight - len; i++)
  {
    pixs[i * 3] = pixels[i * 3 + len * 3];
    pixs[i * 3 + 1] = pixels[i * 3 + 1 + len * 3];
    pixs[i * 3 + 2] = pixels[i * 3 + 2 + len * 3];

    // 抽出
    int headY = i % spectrumHeight;
    if (pickupIdxY - pickupH / 2 <= headY && headY < pickupIdxY + pickupH / 2) {
      pixsPickup[pickupIdx * 3] = pixs[i * 3];
      pixsPickup[pickupIdx * 3 + 1] = pixs[i * 3 + 1];
      pixsPickup[pickupIdx * 3 + 2] = pixs[i * 3 + 2];
      pickupIdx++;
    }
  }

  for (int i = spectrumWidth * spectrumHeight - len; i < spectrumWidth * spectrumHeight; i++)
  {
    int idx = i - (spectrumWidth * spectrumHeight - len);
    ofColor col;

    col = getColorMap(vec[idx]);
    pixs[i * 3] = col.r;
    pixs[i * 3 + 1] = col.g;
    pixs[i * 3 + 2] = col.b;

    // 抽出
    int headY = i % spectrumHeight;
    if (pickupIdxY - pickupH / 2 <= headY && headY < pickupIdxY + pickupH / 2) {
      pixsPickup[pickupIdx * 3] = pixs[i * 3];
      pixsPickup[pickupIdx * 3 + 1] = pixs[i * 3 + 1];
      pixsPickup[pickupIdx * 3 + 2] = pixs[i * 3 + 2];
      pickupIdx++;
    }
  }

  //std::cerr << pickupIdx << std::endl;

  img->setFromPixels(pixs, spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  img->update();
  spectrogramPix = img->getPixels();
  spectrogramTex.loadData(spectrogramPix);
  img->clear();

  imgPickup->setFromPixels(pixsPickup, pickupH, spectrumWidth, OF_IMAGE_COLOR);
  imgPickup->update();
  specPickupPix = imgPickup->getPixels();
  specPickupTex.loadData(specPickupPix);
  imgPickup->clear();


  // OpenCVで解析するカラー画像領域に取得した映像を格納
  colorImg.setFromPixels(specPickupPix.getData(), pickupH, spectrumWidth);
  // 画像をグレースケールに変換
  grayImg = colorImg;
  grayImg.threshold(255 * maxValue * 0.5);

  // 輪郭を描く
  // 第1引数 輪郭検出対象
  // 第2引数 検出する最小の大きさ(20)
  // 第3引数 検出する最大の大きさ
  // 第4引数 検出する数
  // 第5引数 穴が空いたものを検出するかどうか trueで　検出する

  contourFinder.findContours(grayImg, 1, 20, 1000, false);
  std::cerr << contourFinder.nBlobs << std::endl;

  //try
  //{
  //  contourFinder.findContours(grayImg, 1, 20, 1, false);
  //  std::cerr << contourFinder.nBlobs << std::endl;

  //  // 動的配列をクリアする
  //  edgeLines.clear();

  //  //-----------------------------------------
  //  // 輪郭線の座標を結んで線にする
  //  //-----------------------------------------

  //  // １周目for文で複数の輪郭にアクセス
  //  for (int i = 0; i< contourFinder.nBlobs; i++) {
  //    ofPolyline line;
  //    // ２周目for文でそれぞれの輪郭の点にアクセスし、点を結んで線にする。
  //    for (int j = 0; j<contourFinder.blobs[i].pts.size(); j++) {
  //      // 点を線にする。
  //      line.addVertex(contourFinder.blobs[i].pts[j]);
  //    }
  //    // 作成した線を格納
  //    edgeLines.push_back(line);
  //  }
  //}
  //catch (cv::Exception& e)
  //{
  //  const char* err_msg = e.what();
  //  std::cout << "exception caught: " << err_msg << std::endl;
  //}
 
}

void Spectrum::drawSpectrogram() {
  ofRotate(-90);
  spectrogramTex.draw(-spectrumHeight - pos.y, pos.x);
  specPickupTex.draw(-spectrumHeight - pos.y - pickupH - marginY, pos.x);

  // 取り込んだ画像を表示
  colorImg.draw(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  // グレースケール画像
  grayImg.draw(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  // 境界線のサイズと色指定
  ofSetLineWidth(1);
  ofSetColor(255, 255, 0);
  // 輪郭線の描画
  for (int cnt = 0; cnt< edgeLines.size(); cnt++) {
    edgeLines[cnt].draw();
  }
  

  ofRotate(90);
}

void Spectrum::drawFrame() {
  
  // 枠
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::white);
  ofDrawRectangle(pos.x, pos.y, spectrumWidth, spectrumHeight);

  // 枠[抽出]
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::white);
  ofDrawRectangle(pos.x, pos.y + spectrumHeight + marginY, spectrumWidth, pickupH);

  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::white);
  ofDrawRectangle(pos.x, pos.y + spectrumHeight + marginY * 2 + pickupH, spectrumWidth, pickupH);

}

ofColor Spectrum::getColorMap(float _level) {
  float a = (1.0f - min(1.0f, _level)) / 0.166666f;
  int X = floor(a);
  int Y = floor(255 * (a - X));

  //std::cerr << X << std::endl;

  int r = 0;
  int g = 0;
  int b = 0;
  switch (X) {
  case 0:
    r = 255; g = Y; b = 0; break;
  case 1:
    r = 255 - Y; g = 255; b = 0; break;
  case 2:
    r = 0; g = 255; b = Y; break;
  case 3:
    r = 0; g = 255 - Y; b = 255; break;
  case 4:
    r = Y; g = 0; b = 255; break;
  case 5:
    //r = 255; g = 0; b = 255; break;
    r = 255 - Y; g = 0; b = 255 - Y; break;
  case 6:
    r = 0; g = 0; b = 0; break;
    /*r = 255 - Y; g = 0; b = 255 - Y; break;*/
  }

  return  ofColor(r, g, b);
}
