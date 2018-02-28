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

  // �O���[�X�P�[�� 2�ɉ�
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

  // Max��Avg���擾
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

    // ���o
    int headY = i % spectrumHeight;
    if (pickupIdxY - pickupH / 2 <= headY && headY < pickupIdxY + pickupH / 2) {
      //_specPickupPix[pickupIdx * 3] = pixs[i * 3];
      //_specPickupPix[pickupIdx * 3 + 1] = pixs[i * 3 + 1];
      //_specPickupPix[pickupIdx * 3 + 2] = pixs[i * 3 + 2];

      ofColor _col = covertGrayScale(ofColor(pixs[i * 3], pixs[i * 3 + 1], pixs[i * 3 + 2]));
      specPickupPix[pickupIdx * 3] = _col.r;
      specPickupPix[pickupIdx * 3 + 1] = _col.g;
      specPickupPix[pickupIdx * 3 + 2] = _col.b;
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
      
      //_specPickupPix[pickupIdx * 3] = pixs[i * 3];
      //_specPickupPix[pickupIdx * 3 + 1] = pixs[i * 3 + 1];
      //_specPickupPix[pickupIdx * 3 + 2] = pixs[i * 3 + 2];

      ofColor _col = covertGrayScale(ofColor(pixs[i * 3], pixs[i * 3 + 1], pixs[i * 3 + 2]));
      specPickupPix[pickupIdx * 3] = _col.r;
      specPickupPix[pickupIdx * 3 + 1] = _col.g;
      specPickupPix[pickupIdx * 3 + 2] = _col.b;
      pickupIdx++;
    }
  }

  //std::cerr << pickupIdxY << std::endl;

  img->setFromPixels(pixs, spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  img->update();
  spectrogramPix = img->getPixels();
  spectrogramTex.loadData(spectrogramPix);
  img->clear();

  // ���o
  imgPickup->setFromPixels(specPickupPix.getData(), pickupH, spectrumWidth, OF_IMAGE_COLOR);
  imgPickup->update();
  specPickupTex.loadData(imgPickup->getPixels());
  imgPickup->clear();

  // OpenCV�ŉ�͂���J���[�摜�̈�Ɏ擾�����f�����i�[
  colorImg.setFromPixels(specPickupPix.getData(), pickupH, spectrumWidth);
  // �摜���O���[�X�P�[���ɕϊ�
  grayImg = colorImg;
  grayImg.threshold(255 * maxValue * 0.8);

  // �֊s��`��
  // ��1���� �֊s���o�Ώ�
  // ��2���� ���o����ŏ��̑傫��(20)
  // ��3���� ���o����ő�̑傫��
  // ��4���� ���o���鐔
  // ��5���� �����󂢂����̂����o���邩�ǂ��� true�Ł@���o����

  //contourFinder.findContours(grayImg, 1, 20, 1000, false);
  //std::cerr << contourFinder.nBlobs << std::endl;

  try
  {
    contourFinder.findContours(grayImg, 0, 600 * 50, 100, false, false);
    //std::cerr << contourFinder.nBlobs << std::endl;

    // ���I�z����N���A����
    edgeLines.clear();

    //-----------------------------------------
    // �֊s���̍��W������Ő��ɂ���
    //-----------------------------------------

    // �P����for���ŕ����̗֊s�ɃA�N�Z�X
    for (int i = 0; i< contourFinder.nBlobs; i++) {
      ofPolyline line;
      //std::cout << contourFinder.blobs[i].pts.size() << std::endl;
      // �Q����for���ł��ꂼ��̗֊s�̓_�ɃA�N�Z�X���A�_������Ő��ɂ���B
      for (int j = 0; j<contourFinder.blobs[i].pts.size(); j++) {
        // �_����ɂ���B
        //std::cout << "exception caught: " << contourFinder.blobs[i].pts[j].x << std::endl;
        line.addVertex(contourFinder.blobs[i].pts[j]);
      }
      // �쐬���������i�[
      edgeLines.push_back(line);
    }
  }
  catch (cv::Exception& e)
  {
    const char* err_msg = e.what();
    std::cout << "exception caught: " << err_msg << std::endl;
  }

  //std::cout << pickupIdxY << std::endl;
  
}

void Spectrum::drawSpectrogram() {
  ofRotate(-90);
  spectrogramTex.draw(-spectrumHeight - pos.y, pos.x);

  // ���o
  specPickupTex.draw(-spectrumHeight - pos.y - pickupH - marginY, pos.x);

  // �O���[�X�P�[���摜 2�ɉ�
  grayImg.draw(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  // ���E���̃T�C�Y�ƐF�w��
  ofPushMatrix();
  ofTranslate(-spectrumHeight - pos.y - pickupH * 2 - marginY * 2, pos.x);

  ofSetLineWidth(1);
  ofSetColor(255, 0, 0);
  // �֊s���̕`��
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

ofColor Spectrum::covertGrayScale(ofColor _col) {

  int X = 0;
  int Y = 0;
  if (_col.r == 255) {
    X = 0;
    Y = _col.g;
  }
  else if (_col.r == 0) {
    if (_col.g == 255) {
      X = 2;
      Y = _col.b;
    }
    else if(_col.b == 255) {
      X = 3;
      Y = 255 - _col.g;
    }
    else {
      X = 6;
      //Y = floor(255 * (a - X));
    }
  }
  else if (_col.g == 255) {
    X = 1;
    Y = 255 - _col.r;
  }
  else if (_col.b == 255) {
    X = 4;
    Y = _col.r;
  }
  else if (_col.g == 0) {
    X = 5;
    Y = 255 - _col.b;
  }

  ofColor newCol;
  newCol.r = 255 - 255 / 6 * X;
  newCol.g = 255 - 255 / 6 * X;
  newCol.b = 255 - 255 / 6 * X;

  return  newCol;
}
