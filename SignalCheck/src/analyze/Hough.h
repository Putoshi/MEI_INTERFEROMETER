#pragma once

#include "ofMain.h"
#include "./analyze/Tuple2.h"
#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iterator>
#define M_PI 3.14159265358979323846

class Hough
{
public:
  Hough();
  ~Hough();
  void init();
  std::vector<int16_t> getHoughLine(std::vector<int16_t> src_image);
  std::vector<float> sin_table;
  std::vector<float> cos_table;

  std::vector<Tuple2> calcHoughLine(std::vector<int16_t> src_image);

private:
  int w, h, diagonal, d2;

};