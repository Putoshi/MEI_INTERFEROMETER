#pragma once

#include "ofMain.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>

class ScreenCapture {
private:
  ScreenCapture() {
  };
  ofImage img;

public:
  static ScreenCapture& ScreenCapture::getInstance() {
    static ScreenCapture sc;
    return sc;
  }


  void saveScreenCapture(string _path, string _name) {
    img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    img.save(_path + _name + ".png");
    //std::cerr << "ScreenCapture: " + _path + _name + ".png" << std::endl;
  }


};

