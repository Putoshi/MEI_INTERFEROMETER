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
class PixelObject
{
public:
  PixelObject(int _y);
  ~PixelObject();
  void updateHead();

  vector<int> checkHeadIdx(int _y);
  vector<int> check;
  int lifetime;

private:
  
  int y;
  int detect;
  
};