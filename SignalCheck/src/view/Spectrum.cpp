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


  // �X�y�N�g���O����
  spectrogramPix.allocate(spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  spectrogramPix.set(0);

  for (int i = 0; i < spectrumHeight; i++) {
    for (int j = 0; j < spectrumWidth; j++) {
      spectrogramPix.setColor(i, j, ofColor::black);
    }
  }
  spectrogramTex.allocate(spectrumHeight, spectrumWidth, GL_RGB);
  spectrogramTex.loadData(spectrogramPix);


  // �X�y�N�g���O����[���o]
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


  // openCv�ŉ�͂���̈�m��
  colorImg.allocate(pickupH, spectrumWidth);

  // �O���[�X�P�[��
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

  // Max��Avg���擾
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

    // ���o
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

    // ���o
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


  // OpenCV�ŉ�͂���J���[�摜�̈�Ɏ擾�����f�����i�[
  colorImg.setFromPixels(specPickupPix.getData(), pickupH, spectrumWidth);
  // �摜���O���[�X�P�[���ɕϊ�
  grayImg = colorImg;
  grayImg.threshold(255 * maxValue * 0.5);

  // �֊s��`��
  // ��1���� �֊s���o�Ώ�
  // ��2���� ���o����ŏ��̑傫��(20)
  // ��3���� ���o����ő�̑傫��
  // ��4���� ���o���鐔
  // ��5���� �����󂢂����̂����o���邩�ǂ��� true�Ł@���o����

  contourFinder.findContours(grayImg, 1, 20, 1000, false);
  std::cerr << contourFinder.nBlobs << std::endl;

  //try
  //{
  //  contourFinder.findContours(grayImg, 1, 20, 1, false);
  //  std::cerr << contourFinder.nBlobs << std::endl;

  //  // ���I�z����N���A����
  //  edgeLines.clear();

  //  //-----------------------------------------
  //  // �֊s���̍��W������Ő��ɂ���
  //  //-----------------------------------------

  //  // �P����for���ŕ����̗֊s�ɃA�N�Z�X
  //  for (int i = 0; i< contourFinder.nBlobs; i++) {
  //    ofPolyline line;
  //    // �Q����for���ł��ꂼ��̗֊s�̓_�ɃA�N�Z�X���A�_������Ő��ɂ���B
  //    for (int j = 0; j<contourFinder.blobs[i].pts.size(); j++) {
  //      // �_����ɂ���B
  //      line.addVertex(contourFinder.blobs[i].pts[j]);
  //    }
  //    // �쐬���������i�[
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

  // ��荞�񂾉摜��\��
  colorImg.draw(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  // �O���[�X�P�[���摜
  grayImg.draw(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  // ���E���̃T�C�Y�ƐF�w��
  ofSetLineWidth(1);
  ofSetColor(255, 255, 0);
  // �֊s���̕`��
  for (int cnt = 0; cnt< edgeLines.size(); cnt++) {
    edgeLines[cnt].draw();
  }
  

  ofRotate(90);
}

void Spectrum::drawFrame() {
  
  // �g
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::white);
  ofDrawRectangle(pos.x, pos.y, spectrumWidth, spectrumHeight);

  // �g[���o]
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
