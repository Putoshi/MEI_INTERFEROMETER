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
    string src_s = XML.getValue("settings:path:src", "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog_.bak");
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

    //delete[] cstr; // メモリ解放

    /*std::cerr << SRC_PATH << std::endl;*/
  };
  ofxXmlSettings XML;

public:
  static Const& Const::getInstance() {
    static Const cst;
    return cst;
  }

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


};

