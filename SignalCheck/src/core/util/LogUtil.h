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
};

