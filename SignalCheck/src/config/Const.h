#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>

class Const {
private:
  Const() {
    //std::cerr << "Const" << std::endl;
    XML.loadFile("settings.xml");


    // adiファイルのパス
    string src_s = XML.getValue("settings:path:src", "C:/Users/Putoshi/Documents/MEI/DaqLog/___DaqLog_.bak");
    SRC_PATH = new char[src_s.size() + 1]; // メモリ確保
    std::char_traits<char>::copy(SRC_PATH, src_s.data(), src_s.size() + 1);

    // adiファイルの出力パス
    string dst_s = XML.getValue("settings:path:dest", "C:/Users/Putoshi/Documents/MEI/DaqLog/__DaqLog_.bak");
    DST_PATH = new char[dst_s.size() + 1]; // メモリ確保
    std::char_traits<char>::copy(DST_PATH, dst_s.data(), dst_s.size() + 1);

    // チャンネルの設定
    CENTER_ANT = XML.getValue("settings:channel:centerAnt", 0);
    EAST_ANT = XML.getValue("settings:channel:eastAnt", 4);
    SOUTH_ANT = XML.getValue("settings:channel:southAnt", 2);

    // FPS
    frameRate = XML.getValue("settings:fps", 60);

    // ログ保存
    enableLogTxt = int2bool(XML.getValue("settings:save:txt", 0));

    // キャプ画像
    enableCapture = int2bool(XML.getValue("settings:save:captureImage", 0));

    // adiファイル保存
    enableSaveAdi = int2bool(XML.getValue("settings:save:adi", 0));

    // 位相差の閾値 ブレ
    thresholdDispersion = XML.getValue("settings:threshold:dispersion", 5.0f);

    // 位相差の閾値 ブレ
    thresholdDispersion = XML.getValue("settings:threshold:dispersion", 5.0f);

    // 2極化の閾値
    thresholdBipolar = XML.getValue("settings:threshold:bipolar", 45);

    //delete[] cstr; // メモリ解放

    /*std::cerr << SRC_PATH << std::endl;*/
  };
  ofxXmlSettings XML;

  bool int2bool(int n) {
    bool b = false;
    if (n == 1) {
      b = true;
    }
    return b;
  };

  int bool2int(bool b) {
    int n = 0;
    if (b) {
      n = 1;
    }
    return n;
  };

public:
  static Const& Const::getInstance() {
    static Const cst;
    return cst;
  }

  void saveXml() {
    XML.setValue("settings:fps", frameRate);
    XML.setValue("settings:save:txt", bool2int(enableLogTxt));
    XML.setValue("settings:save:captureImage", bool2int(enableCapture));
    XML.setValue("settings:save:adi", bool2int(enableSaveAdi));
    XML.setValue("settings:threshold:dispersion", thresholdDispersion);
    XML.setValue("settings:threshold:bipolar", thresholdBipolar);

    XML.save("settings.xml");
  };

  // adiファイルのパス
  char * SRC_PATH;

  // 一時保存パス
  char * DST_PATH;

  // 中央アンテナのチャンネル
  int CENTER_ANT;

  // 東アンテナのチャンネル
  int EAST_ANT;

  // 南アンテナのチャンネル
  int SOUTH_ANT;

  // FPS
  int frameRate;

  // ログ保存
  bool enableLogTxt;
  
  // キャプ画像
  bool enableCapture;

  // adiファイルを保存する
  bool enableSaveAdi;

  // 位相差の閾値
  int thresholdDispersion;

  // 2極化の閾値
  int thresholdBipolar;

};

