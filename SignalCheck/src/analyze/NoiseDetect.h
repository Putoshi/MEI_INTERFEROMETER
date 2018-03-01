#pragma once

#include "ofMain.h"
#include "./analyze/PixelObject.h"
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

private:
  vector<PixelObject> pixelObjectVec;
  vector<int> checkIdxVec;

};