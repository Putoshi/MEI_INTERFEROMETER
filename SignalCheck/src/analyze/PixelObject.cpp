#include "PixelObject.h"

using namespace std;

PixelObject::PixelObject(int _y)
{
  dots.reserve(600);
  lifetime = 0;
  y = _y;
  dopplerShift = 0;
  check.resize(0);
  checkHeadIdx(y);
  detect = 0; // 0: ”»’f’†,  1: OK , 2: NG
}


PixelObject::~PixelObject()
{
}

void PixelObject::updateHead()
{
  //std::cerr << "dots : " << dots.size() << std::endl;
  dots.push_back(check);
  if (dots.size() >= 600) {
    dots.erase(dots.begin());
  }

  check.clear();
  lifetime++;

}

void PixelObject::deactivate()
{
  //vector<vector<int>> dots;
  vector<vector<int>>().swap(dots);
}


vector<int> PixelObject::checkHeadIdx(int _y)
{
  int v1 = _y + 1;
  int v2 = _y;
  int v3 = _y - 1;
  vector<int> _check;
  _check.reserve(50);
  if (_y == 0) {
    _check.push_back(v1);
    _check.push_back(v2);
  }
  else if (_y == (50 - 1)) {
    _check.push_back(v2);
    _check.push_back(v3);
  }
  else {
    _check.push_back(v1);
    _check.push_back(v2);
    _check.push_back(v3);
  }

  vector<int> _newcheck;
  _newcheck.reserve(50);

  std::sort(_check.begin(), _check.end());
  std::sort(check.begin(), check.end());
  _newcheck.resize(check.size() + _check.size());
  set_union(check.begin(), check.end(), _check.begin(), _check.end(), _newcheck.begin());

  check.clear();
  check.reserve(_newcheck.size());
  check = _newcheck;
  std::sort(check.begin(), check.end());

  vector<int>().swap(_check);
  vector<int>().swap(_newcheck);

  if (_y > y) {
    dopplerShift++;
  }
  y = _y;



  return check;
}

