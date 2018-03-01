#include "PixelObject.h"

using namespace std;

PixelObject::PixelObject(int _y)
{
  lifetime = 0;
  y = _y;
  check.resize(0);
  checkHeadIdx(y);
  detect = 0; // 0: ”»’f’†,  1: OK , 2: NG
}


PixelObject::~PixelObject()
{
}

void PixelObject::updateHead()
{
  check.clear();
  lifetime++;
}


vector<int> PixelObject::checkHeadIdx(int _y)
{
  int v1 = _y + 1;
  int v2 = _y;
  int v3 = _y - 1;
  if (_y == 0) {
    check.push_back(v1);
    check.push_back(v2);
  }
  else if(_y == (50 - 1)) {
    check.push_back(v2);
    check.push_back(v3);
  }
  else {
    check.push_back(v1);
    check.push_back(v2);
    check.push_back(v3);
  }


  //vector<vector<int>> newVec;
  //for (int i = 0; i < check.size(); i++) {
  //  check[i].
  //  for (int i = 0; i < check.size(); i++) {
  //  }
  //}

  //std::sort(check.begin(), check.end());
  //check.erase(std::unique(check.begin(), check.end()), check.end());
  return check;
}

