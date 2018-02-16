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
    vecColor[i].resize(spectrumWidth);
    for (int j = 0; j < spectrumWidth; j++) {
      //vecColor[i][j] = ofColor::black;
      spectrogramPix.setColor(i, j, ofColor::black);
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

  unsigned char * pixels = spectrogramPix.getPixels();
  unsigned char pixs2[600 * 200 * 3];
  ofImage *img;
  img = new ofImage;
  img->clear();
  int len = 200;
  for (int i = 0; i < spectrumWidth * spectrumHeight - len; i++)
  {
    pixs2[i * 3] = pixels[i * 3 + len * 3];
    pixs2[i * 3 + 1] = pixels[i * 3 + 1 + len * 3];
    pixs2[i * 3 + 2] = pixels[i * 3 + 2 + len * 3];
  }

  for (int i = spectrumWidth * spectrumHeight - len; i <  spectrumWidth * spectrumHeight; i++)
  {
    int idx = i - (spectrumWidth * spectrumHeight - len);
    ofColor col;
    //if (_vec[idx]>=0.3) {
    //  col = getColorMap(_vec[idx]+0.3);
    //}
    //else {
    //  col = getColorMap(_vec[idx]);
    //}
    col = getColorMap(_vec[idx]);
    
    pixs2[i * 3] = col.r;
    pixs2[i * 3 + 1] = col.g;
    pixs2[i * 3 + 2] = col.b;
  }
  //std::cerr << spectrogramPix.getWidth() << std::endl;

  img->setFromPixels(pixs2, spectrumHeight, spectrumWidth, OF_IMAGE_COLOR);
  img->update();
  spectrogramPix = img->getPixels();

  

  ofRotate(-90);
  spectrogramTex.loadData(spectrogramPix);
  spectrogramTex.draw( - 10 - spectrumHeight, 10);
  img->clear();
  

  ofRotate(90);
  // 枠
  ofPushStyle();
  ofNoFill();
  ofSetColor(ofColor::white);
  ofDrawRectangle(10, 10, spectrumWidth, spectrumHeight);

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
