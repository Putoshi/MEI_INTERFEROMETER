#include "Spectrum.h"


Spectrum::Spectrum() {
 
  
}

vector<vector<ofColor>> vecColor(600);
void Spectrum::setup() {
  spectrumHeight = 200;
  spectrumWidth = 600;
  
  color.setHsb(0, 255, 255);

  colorMap.allocate(5, spectrumHeight, OF_IMAGE_COLOR);
  colorMap.set(0);

  ofColor col;
  for (int i = 0; i < spectrumHeight; i++) {
    col = getColorMap((float)i / spectrumHeight);
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
    vecColor[i].resize(spectrumWidth);
    for (int j = 0; j < spectrumWidth; j++) {
      //vecColor[i][j] = ofColor::black;
      spectrogramPix.setColor(i, j, ofColor::red);
    }
  }

  spectrogramTex.allocate(spectrumHeight, spectrumWidth, GL_RGB);
  spectrogramTex.loadData(spectrogramPix);
  //drawSpecPixel();
}

void Spectrum::update() {
}

void Spectrum::draw(vector<float> _vec) {
  colorMapTex.draw(0, 0);
  drawSpectrogram(_vec);

  //std::cerr << "draw" << std::endl;
}

void Spectrum::drawSpectrogram(vector<float> _vec) {

  //ofPixels newPix;
  //newPix.allocate(spectrumWidth, spectrumHeight, OF_IMAGE_COLOR);
  //newPix.set(0);

  
  unsigned char * pixels = spectrogramPix.getPixels();
  unsigned char pixs2[600 * 200 * 3];
  ofImage *img;
  img = new ofImage;
  img->clear();
  int len = 0;
  for (int i = 0; i < spectrumWidth * spectrumHeight - len; i++)
  {
    pixs2[i * 3] = pixels[i * 3 + len];
    pixs2[i * 3 + 1] = pixels[i * 3 + 1 + len];
    pixs2[i * 3 + 2] = pixels[i * 3 + 2 + len];

    //char c = pixs2[i * 3];
    //std::cerr << c << std::endl;
  }

  //std::cerr << pixs2[0] << std::endl;
  //std::cerr << pixs2[1] << std::endl;
  //std::cerr << pixs2[2] << std::endl;

  for (int i = spectrumWidth * spectrumHeight - len; i <  spectrumWidth * spectrumHeight; i++)
  {
    int idx = i - (spectrumWidth * spectrumHeight - len);
    //std::cerr << _vec[idx] << std::endl;

    //if (_vec[idx] > 0.3f) {
    //  std::cerr << _vec[idx] << std::endl;
    //}

    //ofColor col = getColorMap(0.688269);
    //pixs2[i * 3] = col.r * 255;
    //pixs2[i * 3 + 1] = col.g * 255;
    //pixs2[i * 3 + 2] = col.b * 255;

    //ofColor re = ofColor::red;
    //pixs2[i * 3] = re.r;
    //pixs2[i * 3 + 1] = re.b;
    //pixs2[i * 3 + 2] = re.g;

    //ofColor re = ofColor::red;
    //pixs2[i * 3] = re..r;
    //pixs2[i * 3 + 1] = re.g;
    //pixs2[i * 3 + 2] = re.b;

    //pixs2[i * 3] = ofRandom(0, 255);
    //pixs2[i * 3 + 1] = ofRandom(0, 255);
    //pixs2[i * 3 + 2] = ofRandom(0, 255);

    //pixs2[i * 3] = pixels[i * 3];
    //pixs2[i * 3 + 1] = pixels[i * 3 + 1];
    //pixs2[i * 3 + 2] = pixels[i * 3 + 2];
  }

  //std::cerr << pixels[0].getData() << std::endl;

  img->setFromPixels(pixs2, spectrumHeight, spectrumWidth, OF_IMAGE_COLOR); 
  //img->setFromPixels(pixels, spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  img->update();

 
  //spectrogramPix = pixs2;

  //for (int i = 0; i < spectrumWidth; i++) {
  //  for (int j = 0; j < spectrumHeight; j++) {
  //    spectrogramPix.setColor(i, j, ofColor::red);
  //  }
  //}

  //std::cerr << spectrogramPix.getWidth() << std::endl;

  

  ofRotate(-90);
  spectrogramPix = img->getPixels();
  spectrogramTex.loadData(spectrogramPix);
  spectrogramTex.draw( - 10 - spectrumHeight, 10);
  img->clear();
  

  ofRotate(90);
  // 枠
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::white);
  ofDrawRectangle(10, 10, spectrumWidth, spectrumHeight);


  //for (int i = 0; i < spectrumWidth; i++) {
  //  vecColor[i].resize(spectrumHeight);
  //  for (int j = 0; j < spectrumHeight; j++) {
  //    //vecColor[i][j] = ofColor::black;
  //  }
  //}
  //drawSpecPixel();


  //std::cerr << spectrogramPix.getLine(0) << std::endl;
  //ofPixels plane = spectrogramPix.getPlane(1);
  //
  //std::cerr << plane.getWidth() << std::endl;
  //for (int i = 0; i <  line.end(); i++) {
  //  vecColor[i].resize(spectrumHeight);
  //  for (int j = 0; j < spectrumHeight; j++) {
  //    //vecColor[i][j] = ofColor::black;
  //  }
  //}

  
  //std::cerr << "draw" << std::endl;
}

void Spectrum::drawSpecPixel() {
  /*for (int i = 0; i < spectrumWidth; i++) {
    for (int j = 0; j < spectrumHeight; j++) {
      spectrogramPix.setColor(vecColor[i][j]);
    }
  }*/
  //std::cerr << vecColor[0][0] << std::endl;
}

//void Spectrum::draw(float* chn, int size) {
//  ofPushStyle();
//  ofPushMatrix();
//
//  ofTranslate(position);
//
//  ofSetColor(225);
//  sprintf(title, "Channel: %i", channel);
//
//  ofDrawBitmapString(title, 4, 18);
//
//  ofSetLineWidth(1);
//  ofNoFill();
//  ofRect(0, 0, 256 * 1 / 6, 100);
//
//  ofSetColor(color);
//  ofSetLineWidth(1);
//
//  ofBeginShape();
//
//  for (int i = 0; i < size; i++) {
//    ofVertex((i / (size / 256 * 1 / 6)) + 1, 50 - chn[i] * 90.0f / 3.14f);
//  }
//  ofEndShape(false);
//
//  ofPopMatrix();
//  ofPopStyle();
//}

ofColor Spectrum::getColorMap(float _level) {
  float a = _level / 0.166666f;
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
