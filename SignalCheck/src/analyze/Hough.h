#pragma once

#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iterator>

class Hough
{
public:
  Hough();
  ~Hough();
  void init();
  std::vector<int16_t> getHoughLine(unsigned char * src_image);

private:
  int w, h, diagonal, d2;

};

