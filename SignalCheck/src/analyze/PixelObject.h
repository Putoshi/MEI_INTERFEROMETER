#pragma once

#include "ofMain.h"
#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iterator>
#include <algorithm>  // std::unique
class PixelObject
{
public:
  PixelObject(int _y);
  ~PixelObject();
  void updateHead();
  void deactivate();

  vector<int> checkHeadIdx(int _y);
  vector<int> check;
  vector<vector<int>> dots;
  int lifetime;

private:
  
  int y;
  int detect;
  
};