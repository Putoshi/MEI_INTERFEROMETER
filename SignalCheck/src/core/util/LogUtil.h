#pragma once

#include "ofMain.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>
#include <sstream>

class LogUtil {
private:
  LogUtil() {
  };

public:
  static LogUtil& LogUtil::getInstance() {
    static LogUtil lu;
    return lu;
  }

  
  string getIndentStr() {
    return "　　　　　　　";
  }

  string getTabStr(int cnt = 1) {
    string tab = "";
    for (int i = 0; i < cnt; i++) {
      tab += "　　";
    }
    return tab;
  }

  void saveCsv(string _path, string _name, string _label, string _content, string _logAlpha, string _logBeta, string _logAlpha5ch, string _logBeta5ch) {
    string filePath = _path + _name + "/data.csv";

    ofBuffer ofbuf;
    string str;
    //vector<int>x;
    //x.push_back(1);
    //x.push_back(2);
    //x.push_back(3);
    //x.push_back(4);
    //x.push_back(5);

    //for (int i = 0; i < x.size(); i++) {
    //  str = str + ofToString(x[i]) + "\r\n"; // \r\nは改行コード
    //}
    //str = _label + "\r\n" + _content + "\r\n\r\n" + "EAST-WEST Diff\r\n" + _logAlpha + "\r\n\r\nSOUTH-NORTH Diff\r\n" + _logBeta + "\r\n";
    ofbuf.append(str);
    ofBufferToFile(ofToDataPath(filePath), ofbuf);

    string filePathPhase = _path + _name + "/phase.csv";
    ofBuffer ofbufPhase;
    string strPhase;

    strPhase = "0.5λ EAST-WEST Diff\r\n" + _logAlpha + "\r\n\r\n0.5λ SOUTH-NORTH Diff\r\n" + _logBeta + "\r\n";
    strPhase = strPhase + "\r\n\r\n2.5λ EAST-WEST Diff\r\n" + _logAlpha5ch + "\r\n\r\n2.5λ SOUTH-NORTH Diff\r\n" + _logBeta5ch + "\r\n";
    ofbufPhase.append(strPhase);
    ofBufferToFile(ofToDataPath(filePathPhase), ofbufPhase);
    

    //ofFile file(filePath);
    //ofBuffer buffer(file);
  }
   
  void saveDailyCsv(string _path, string _name, string _content) {
    string filePath = _path + _name + ".csv";
    ofFile out(ofToDataPath(filePath));

    if (!out.exists()) {
      string label = "Date,Time,Duration(s),AzimuthAngle,ElevationAngle\r\n";
      ofBuffer ofbuf;
      ofbuf.append(label + _content + "\r\n");
      ofBufferToFile(ofToDataPath(filePath), ofbuf);
    }
    else {
      ofBuffer ofbuf;
      ofbuf.append(string(ofBufferFromFile(filePath)));
      ofbuf.append(_content + "\r\n");
      ofBufferToFile(ofToDataPath(filePath), ofbuf);
    }
    //std::cerr << out.exists() << std::endl;
  }
};

