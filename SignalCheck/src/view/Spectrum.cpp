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
  pickupIdxY = 0;

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
  specPickupPix.allocate(pickupH, spectrumWidth, OF_IMAGE_GRAYSCALE);
  specPickupPix.set(0);
  for (int i = 0; i < pickupH; i++) {
    for (int j = 0; j < spectrumWidth; j++) {
      specPickupPix.setColor(i, j, ofColor::black);
    }
  }
  specPickupTex.allocate(pickupH, spectrumWidth, GL_RGB);
  specPickupTex.loadData(specPickupPix);

  hough = Hough();
  hough.init();
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

  if (1 > peekFreq || peekFreq > 5000) {
    peekFreq = 3000.0f;
  }
  
  colorMapTex.draw(pos.x + spectrumWidth + 10, pos.y);
  drawSpectrogram();
  drawFrame();

  //std::cerr << peekFreq << std::endl;

}

void Spectrum::setSpectrum(vector<float> _vec) {
  vec = _vec;

  // 直線抽出用のvector
  std::vector<int16_t> monoVec;
  monoVec.resize(600 * 50);

  // MaxとAvgを取得
  float _avgValue = 0;
  float _maxValue = 0.3;
  for (int i = 0; i < vec.size(); i++)
  {
    _avgValue += vec[i];
    if (vec[i] > _maxValue) {
      _maxValue = vec[i];
    }
  }
  avgValue = _avgValue / vec.size();
  //maxValue += (_maxValue - maxValue) / 2;
  maxValue = 0.45;


  unsigned char * pixels = spectrogramPix.getPixels();

  unsigned char pixs[600 * 500 * 3];
  ofImage *img;
  img = new ofImage;
  img->clear();

  
  ofImage *imgPickup;
  imgPickup = new ofImage;
  imgPickup->clear();

  if((peekFreq - lowFreq) / (highFreq - lowFreq) * spectrumHeight > 0) pickupIdxY = (peekFreq - lowFreq) / (highFreq - lowFreq) * spectrumHeight;

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
      specPickupPix[pickupIdx] = specPickupPix[pickupIdx + 50];

      if (specPickupPix[pickupIdx] == 255) {
        monoVec[pickupIdx] = 1;
      }
      else {
        monoVec[pickupIdx] = 0;
      }
      
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
      
      //_specPickupPix[pickupIdx * 3] = pixs[i * 3];
      //_specPickupPix[pickupIdx * 3 + 1] = pixs[i * 3 + 1];
      //_specPickupPix[pickupIdx * 3 + 2] = pixs[i * 3 + 2];
      int _colgray = covertGrayScale(vec[idx]);
      specPickupPix[pickupIdx] = _colgray;
      monoVec[pickupIdx] = _colgray;
      //ofColor c = specPickupPix.getColor(pickupIdx);
      //std::cerr << specPickupPix.getColor(pickupIdx).r << std::endl;
      pickupIdx++;
    }
  }
  
  //std::cerr << pickupIdxY << std::endl;

  

  img->setFromPixels(pixs, spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  img->update();
  spectrogramPix = img->getPixels();
  spectrogramTex.loadData(spectrogramPix);
  img->clear();

  // 抽出
  imgPickup->setFromPixels(specPickupPix.getData(), pickupH, spectrumWidth, OF_IMAGE_GRAYSCALE);
  imgPickup->update();
  specPickupTex.loadData(imgPickup->getPixels());
  imgPickup->clear();

  
  hough.calcHoughLine(monoVec);

}

void Spectrum::drawSpectrogram() {
  ofRotate(-90);
  spectrogramTex.draw(-spectrumHeight - pos.y, pos.x);

  // 抽出
  specPickupTex.draw(-spectrumHeight - pos.y - pickupH - marginY, pos.x);

  // 境界線のサイズと色指定
  ofPushMatrix();
  ofTranslate(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  ofSetLineWidth(1);
  ofSetColor(255, 0, 0);
  // 輪郭線の描画
  for (int cnt = 0; cnt< edgeLines.size(); cnt++) {
    edgeLines[cnt].draw();
  }
  //if (edgeLines.size() > 0) {
  //  //for (int i = 0; i < edgeLines[0].getVertices.size(); i++) {

  //  //}
  //  //std::cout << edgeLines[0].getVertices << std::endl;
  //  edgeLines[0].draw();
  //}
  

  ofPopMatrix();
  

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

int Spectrum::covertGrayScale(float _level) {
  
  int l = 0;
  if (maxValue * 0.7f <= _level) {
    l = 255;
  }else {
    l = 0;
  }
  return  l;
}
