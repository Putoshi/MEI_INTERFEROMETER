#include "ofMain.h"

class SimpleGraphViewer
{
public:
  SimpleGraphViewer();
  ~SimpleGraphViewer();

  void setup(int _bufferLength = 256, int _offset = 0);

  void allocate(int num);
  void pushData(float _d);

  void draw();
  void draw(float posx, float posy);
  void draw(float posx, float posy, float w, float h, float min, float max);

  void setSize(float _width, float _height);
  void setRange(float _min, float _max);
  void setColor(int _lineColor);

  float min, max, width, height;



private:
  int bufferLength, offset, idx;
  float* data;
  int lineColor;


};
