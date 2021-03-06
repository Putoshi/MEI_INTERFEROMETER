#pragma once

#include "ofMain.h"
#include "./analyze/PixelObject.h"
#include "./event/EventManager.h"
#include "./event/Event.h"
#include "./util/DateUtil.h"
#include "./config/Const.h"
#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iterator>
#include <algorithm>  // std::unique
class NoiseDetect
{
public:
  NoiseDetect();
  ~NoiseDetect();

  ofPixels convert(ofPixels _dat);
  //Event event;

private:
  vector<PixelObject> pixelObjectVec;
  std::vector<int> checkIdxVec;
  vector<vector<int>> blurPixVec;
  int pastTime;

};