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

  int loop = pixelObjectVec.size();
  
  for (int i = loop - 1; i >= 0; i--) {
    //std::cerr << i  << std::endl;
    PixelObject v1 = pixelObjectVec[i];
    pixelObjectVec[i].updateHead();

    bool isActive = false;
    for (int j = 0; j < v1.check.size(); j++) {
      ofColor col = newPix.getColor(v1.check[j], 600 - 1);
      if (col.getBrightness() >= 255) {
        isActive = true;
        std::vector<int> v2 = pixelObjectVec[i].checkHeadIdx(v1.check[j]);
        std::sort(v2.begin(), v2.end());

        std::sort(checkIdxVec.begin(), checkIdxVec.end());
        std::vector<int> v3;
        v3.resize(v2.size() + checkIdxVec.size());

        set_union(v2.begin(), v2.end(), checkIdxVec.begin(), checkIdxVec.end(), v3.begin());

        checkIdxVec.clear();
        //v3 = checkIdxVec;
        checkIdxVec.resize(v3.size());
        checkIdxVec = v3;

        //std::cerr << "v2:" << v2.size() << std::endl;
        
        vector<int>().swap(v2);
        vector<int>().swap(v3);

        newPix.setColor(v1.check[j], 600 - 1, ofColor::yellow);
      }
    }
    


    if (!isActive) {
      PixelObject deleteObj = v1;

      // Dead”»’è‚ÌPixel
      for (int k = 0; k < deleteObj.dots.size(); k++) {
        vector<int> del = deleteObj.dots[k];
        for (int l = 0; l < del.size(); l++) {
          newPix.setColor(del[l], 600 - 1 - (deleteObj.dots.size() - k) - 1, ofColor(200, 0, 0));
          newPix.setColor(del[l], 600 - 1 - (deleteObj.dots.size() - k), ofColor(200, 0, 0));
        }
      }
      


      std::cerr << "deleteVec:" << deleteObj.lifetime << std::endl;
      /*std::cerr << "deleteVec:" << deleteVec.size() << std::endl;*/
      /*for (int j = 0; j < deleteObj.check.size(); j++) {
        std::cerr << "deleteVec:" << deleteObj.size() << std::endl;
      }*/

      pixelObjectVec[i].deactivate();
      pixelObjectVec.erase(pixelObjectVec.begin() + i);
      
      //pixelObjectVec[i].detectend();
     
    }
    //else {
    //  _pixelObjectVec.push_back(pixelObjectVec[i]);
    //}

    //std::copy(v2.begin(), v2.end(), std::back_inserter(checkIdxVec));
  }
  //pixelObjectVec.clear();
  //pixelObjectVec = _pixelObjectVec;
  //_pixelObjectVec.clear();


  //std::sort(checkIdxVec.begin(), checkIdxVec.end());
  //checkIdxVec.erase(std::unique(checkIdxVec.begin(), checkIdxVec.end()), checkIdxVec.end());
  ////std::cerr << checkIdxVec << std::endl;

  //for (int i = 0; i < checkIdxVec.size(); i++) {
  //  std::cerr << checkIdxVec[i] << std::endl;
  //}

  std::cerr << "checkIdxVec:" << checkIdxVec.size() << std::endl;

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