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

  void saveCsv(string _path, string _name, string _label, string _content, string _logAlpha, string _logBeta) {
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
    str = _label + "\r\n" + _content + "\r\n\r\n" + "EAST Diff\r\n" + _logAlpha + "\r\n\r\nSOUTH Diff\r\n" + _logBeta + "\r\n";
    ofbuf.append(str);
    ofBufferToFile(ofToDataPath(filePath), ofbuf);

    //ofFile file(filePath);
    //ofBuffer buffer(file);
  }
};

