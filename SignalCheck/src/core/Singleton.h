#pragma once

#include "ofMain.h"
//#include "./core/Singleton.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>

class Singleton {
private:
  Singleton() {
    std::cerr << "Singleton" << std::endl;
  };

public:
  static Singleton& Singleton::getInstance() {
    static Singleton sin;
    return sin;
  }


  int test;
  int setDetectEvent(int _num) {
    test = _num;
    return test;
  }


};

