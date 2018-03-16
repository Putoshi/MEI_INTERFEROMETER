#include "NoiseDetect.h"

using namespace std;

NoiseDetect::NoiseDetect()
{
  //pixelObjectVec.push_back(PixelObject());
  blurPixVec.resize(3);
  blurPixVec[0].resize(50);
  blurPixVec[1].resize(50);
  blurPixVec[2].resize(50);
  //event = Event();
}


NoiseDetect::~NoiseDetect()
{
}

ofPixels NoiseDetect::convert(ofPixels _dat)
{
  //std::cerr << "convert start " << std::endl;
  ofPixels newPix;
  newPix = _dat;

  // 3px残像残す
  vector<int> pix;
  pix.resize(50);
  for (int i = 0; i < 50; i++) {
    ofColor col = newPix.getColor(i, 600 - 1);
    if (col.getBrightness() >= 255) {
      pix[i] = 1;
    }
    else {
      pix[i] = 0;
    }

    if (blurPixVec[0][i] == 1 || blurPixVec[1][i] == 1 || blurPixVec[2][i] == 1) {
      newPix.setColor(i, 600 - 1, ofColor::white);
    }
    else {
      newPix.setColor(i, 600 - 1, ofColor::black);
    }
  }
  blurPixVec.erase(blurPixVec.begin());
  blurPixVec.push_back(pix);

  checkIdxVec.clear();
  vector<PixelObject> _pixelObjectVec;

  int loop = pixelObjectVec.size();
  //std::cerr << "convert start2 " << std::endl;
  for (int i = loop - 1; i >= 0; i--) {
    //std::cerr << i  << std::endl;
    PixelObject v1 = pixelObjectVec[i];
    pixelObjectVec[i].updateHead();

    bool isActive = false;
    for (int j = 0; j < v1.check.size(); j++) {
      ofColor col = newPix.getColor(v1.check[j], 600 - 1);

      if (col.getBrightness() >= 255) {
        //std::cerr << ofToString(col.r) << std::endl;
        isActive = true;
        std::vector<int> v2 = pixelObjectVec[i].checkHeadIdx(v1.check[j]);
        std::sort(v2.begin(), v2.end());

        std::sort(checkIdxVec.begin(), checkIdxVec.end());
        std::vector<int> v3;
        v3.resize(v2.size() + checkIdxVec.size());

        set_union(v2.begin(), v2.end(), checkIdxVec.begin(), checkIdxVec.end(), v3.begin());

        checkIdxVec.clear();
        checkIdxVec.resize(v3.size());
        checkIdxVec = v3;

        vector<int>().swap(v2);
        vector<int>().swap(v3);

        newPix.setColor(v1.check[j], 600 - 1, ofColor(251, 211, 80));
      }
    }

    if (!isActive) {
      PixelObject deleteObj = v1;

      // ジャッジ
      string time = DateUtil::getInstance().getTimeString();
      std::cerr << time << "  ";
      std::cerr << "判定:  LifeTime=" << deleteObj.lifetime + 1 << "  DopplerShift=" << deleteObj.dopplerShift << std::endl;

      ofColor judgeColor;
      if (deleteObj.dopplerShift > 10) {
        judgeColor = ofColor(195, 30, 30);
      }
      else if (deleteObj.lifetime > 20) {
        judgeColor = ofColor(195, 30, 30);
      }
      else {
        judgeColor = ofColor(57, 195, 59);

        // イベントディスパッチ
        EventManager::getInstance().lifetime = deleteObj.lifetime + 1;
        EventDispatcher* eventDispatcher = EventManager::getInstance().getEventDispatcher();
        eventDispatcher->dispatchEvent(new Event(Event::ECHO_DETECT));
      }
      //std::cerr << "convert start3 " << std::endl;
      int _idx;
      if (deleteObj.lifetime == 0) {
        deleteObj.dots.push_back(deleteObj.check);
      }
      for (int k = 0; k < deleteObj.dots.size(); k++) {
        vector<int> del = deleteObj.dots[k];
        for (int l = 0; l < del.size(); l++) {
          _idx = 600 - 1 - (deleteObj.dots.size() - k);
          if (0 <= (_idx - 1) && (_idx - 1) < 600 && 0 <= del[l] && del[l] < 50) newPix.setColor(del[l], _idx - 1, judgeColor);
          if (0 <= _idx && _idx < 600 && 0 <= del[l] && del[l] < 50) newPix.setColor(del[l], _idx, judgeColor);
        }
      }
      pixelObjectVec[i].deactivate();
      pixelObjectVec.erase(pixelObjectVec.begin() + i);
    }
  }

  //std::cerr << "checkIdxVec:" << checkIdxVec.size() << std::endl;

  for (int i = 0; i < 50; i++) {
    ofColor col = newPix.getColor(i, 600 - 1);
    //std::cerr << col.getBrightness() << std::endl;
    if (col.getBrightness() >= 255) {

      bool renzoku = false;
      for (int j = 0; j < checkIdxVec.size(); j++) {
        if (checkIdxVec[j] == i) {
          renzoku = true;
        }
      }

      if (!renzoku) {
        PixelObject pixelObject(i);
        pixelObjectVec.push_back(pixelObject);
        newPix.setColor(i, 600 - 1, ofColor::yellow);
      }
    }
  }

  //std::cerr << newPix[newPix.size() - i * 3 - 2 - 1] << std::endl;
  //std::cerr << "convert end " << std::endl;
  return newPix;
}