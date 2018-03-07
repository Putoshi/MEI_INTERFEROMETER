
#include "ofMain.h"
#include "./core/util/ScreenCapture.h"

#include <algorithm>    // std::max
#include <cmath>
#include <math.h>
#include <string>
#include <string.h>
#include <iterator>

class PhaseDiffGraphViewer
{
public:
  PhaseDiffGraphViewer();
  ~PhaseDiffGraphViewer();

  void setup(int _bufferLength = 256, int _offset = 0);

  void allocate(int num);
  void pushData(float _d, float _peekFreq);

  void draw();
  void draw(float posx, float posy);
  void draw(float posx, float posy, float w, float h, float min, float max);

  void setSize(float _width, float _height);
  void setRange(float _min, float _max);
  void setColor(int _lineColor);

  void culcDiff(int _lifetime);

  float min, max, width, height;


private:
  int bufferLength, offset, idx;
  float* data;
  int lineColor;
  float swingWidth;
  float prevValue;
  float peekFreq;

};