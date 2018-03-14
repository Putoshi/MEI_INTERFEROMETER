
#include "ofMain.h"
#include "./core/util/ScreenCapture.h"
#include "./config/Const.h"
#include "./util/DateUtil.h"
#include "./util/LogUtil.h"

#define M_PI 3.14159265358979323846

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
  void pushData(float _alpha, float _beta, float _peekFreq);

  void draw();
  void draw(float posx, float posy);
  void draw(float posx, float posy, float w, float h, float min, float max);

  void setSize(float _width, float _height);
  void setRange(float _min, float _max);
  void setColor(int _lineColor1, int _lineColor2);

  void culcDiff(int _lifetime);

  float min, max, width, height;


private:
  int bufferLength, offset, idx;
  float* dataAlpha;
  float* dataBeta;
  int lineColor1;
  int lineColor2;
  float swingWidthAlpha;
  float swingWidthBeta;
  float prevValueAlpha;
  float prevValueBeta;
  float peekFreq;

};