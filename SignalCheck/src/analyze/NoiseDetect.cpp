#include "NoiseDetect.h"

using namespace std;

NoiseDetect::NoiseDetect()
{
  //pixelObjectVec.push_back(PixelObject());
}


NoiseDetect::~NoiseDetect()
{
}

ofPixels NoiseDetect::convert(ofPixels _dat)
{
  ofPixels newPix;
  newPix = _dat;

  checkIdxVec.clear();
  vector<PixelObject> _pixelObjectVec;
  for (int i = 0; i < pixelObjectVec.size(); i++) {
    PixelObject v1 = pixelObjectVec[i];
    pixelObjectVec[i].updateHead();

    bool isActive = false;
    for (int j = 0; j < v1.check.size(); j++) {
      ofColor col = newPix.getColor(v1.check[j], 600 - 1);
      if (col.getBrightness() >= 255) {
        isActive = true;
        vector<int> v2 = pixelObjectVec[i].checkHeadIdx(v1.check[j]);
        std::copy(v2.begin(), v2.end(), std::back_inserter(checkIdxVec));

        newPix.setColor(v1.check[j], 600 - 1, ofColor::yellow);
      }
    }

    if (!isActive) {
      PixelObject deleteObj = v1;
      std::cerr << "deleteVec:" << deleteObj.lifetime << std::endl;
      /*std::cerr << "deleteVec:" << deleteVec.size() << std::endl;*/
      /*for (int j = 0; j < deleteObj.check.size(); j++) {
        std::cerr << "deleteVec:" << deleteObj.size() << std::endl;
      }*/
      
      //pixelObjectVec[i].detectend();
     
    }
    else {
      _pixelObjectVec.push_back(pixelObjectVec[i]);
    }

    //std::copy(v2.begin(), v2.end(), std::back_inserter(checkIdxVec));
  }

  pixelObjectVec = _pixelObjectVec;



  //std::sort(checkIdxVec.begin(), checkIdxVec.end());
  //checkIdxVec.erase(std::unique(checkIdxVec.begin(), checkIdxVec.end()), checkIdxVec.end());
  ////std::cerr << checkIdxVec << std::endl;

  //for (int i = 0; i < checkIdxVec.size(); i++) {
  //  std::cerr << checkIdxVec[i] << std::endl;
  //}

  for (int i = 0; i < 50; i++) {
    ofColor col = newPix.getColor(i, 600 - 1);
    if (col.getBrightness() >= 255) {

      bool renzoku = false;
      for (int j = 0; j < checkIdxVec.size(); j++) {
        if (checkIdxVec[j] == i) {
          renzoku = true;
        }
      }
      

      if (!renzoku) {
        PixelObject pixelObject(i);
        //pixelObjectVec.add(i);

        pixelObjectVec.push_back(pixelObject);

        newPix.setColor(i, 600 - 1, ofColor::yellow);
      
      }

     
    }
  }

  //std::cerr << newPix[newPix.size() - i * 3 - 2 - 1] << std::endl;
  return newPix;
}