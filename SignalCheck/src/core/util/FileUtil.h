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

class FileUtil {
private:
  FileUtil() {
  };

public:
  static FileUtil& FileUtil::getInstance() {
    static FileUtil fu;
    return fu;
  }

  void copyFile(string in, string out) {
    // 外部ファイルのパスを指定
    ofFile file(ofToDataPath(in));

    bool flg = file.exists();

    if (flg) {
      file.copyTo(out);
    }
  }

};

