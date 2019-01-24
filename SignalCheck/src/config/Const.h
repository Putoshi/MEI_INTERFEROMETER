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

    // 読み込み中のadiファイルのキャッシュ出力パス
    string cache_s = XML.getValue("settings:path:cache", "./tmp/Cache/DaqLog.bak");
    CACHE_PATH = new char[cache_s.size() + 1]; // メモリ確保
    std::char_traits<char>::copy(CACHE_PATH, cache_s.data(), cache_s.size() + 1);

    // adiファイルの出力パス
    string dst_s = XML.getValue("settings:path:dest", "./tmp/");
    DST_PATH = new char[dst_s.size() + 1]; // メモリ確保
    std::char_traits<char>::copy(DST_PATH, dst_s.data(), dst_s.size() + 1);

    // チャンネルの設定
    CENTER_ANT = XML.getValue("settings:channel:centerAnt", 0);
    EAST_ANT = XML.getValue("settings:channel:eastAnt", 3);
    SOUTH_ANT = XML.getValue("settings:channel:southAnt", 2);
    WEST_ANT = XML.getValue("settings:channel:westAnt", 4);
    NORTH_ANT = XML.getValue("settings:channel:northAnt", 1);

    // FPS
    frameRate = XML.getValue("settings:fps", 60);

    // Debug
    enableDebug = int2bool(XML.getValue("settings:debug", 0));

    // ログ保存
    enableLogCsv = int2bool(XML.getValue("settings:save:csv", 0));

    // キャプ画像
    enableCapture = int2bool(XML.getValue("settings:save:captureImage", 0));

    // 遅延キャプ時間
    delayCapTime = XML.getValue("settings:save:delayCaptureTime", 5);

    // adiファイル保存
    enableSaveAdi = int2bool(XML.getValue("settings:save:adi", 0));

    // デイリーのログ保存
    enableDailyLogCsv = int2bool(XML.getValue("settings:save:dailyCsv", 0));

    // ノイズ判定
    thresholdMaxDuration = XML.getValue("settings:threshold:maxDuration", 30.0f);

    // 流星判定後の連続回避
    thresholdLockTime = XML.getValue("settings:threshold:lockTime", 0);

    // ロングエコーの閾値
    thresholdLongEcho = XML.getValue("settings:threshold:longEcho", 10);

    // ドップラーシフトの閾値
    thresholdDopplerShift = XML.getValue("settings:threshold:dopplerShift", 10);

    // 位相差の閾値 ブレ
    thresholdDispersion = XML.getValue("settings:threshold:dispersion", 5.0f);

    // 2極化の閾値
    thresholdBipolar = XML.getValue("settings:threshold:bipolar", 45);

    // スペクトログラムにしたときの増幅地
    thresholdAmp = XML.getValue("settings:threshold:amp", 1);

    // 内部位相差
    antPhaseDiff.resize(5);
    antPhaseDiff[0] = XML.getValue("settings:antenna:phasediff0", 0);
    antPhaseDiff[1] = XML.getValue("settings:antenna:phasediff1", 0);
    antPhaseDiff[2] = XML.getValue("settings:antenna:phasediff2", 0);
    antPhaseDiff[3] = XML.getValue("settings:antenna:phasediff3", 0);
    antPhaseDiff[4] = XML.getValue("settings:antenna:phasediff4", 0);

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
    XML.setValue("settings:debug", bool2int(enableDebug));
    XML.setValue("settings:save:csv", bool2int(enableLogCsv));
    XML.setValue("settings:save:captureImage", bool2int(enableCapture));
    XML.setValue("settings:save:delayCaptureTime", delayCapTime);
    XML.setValue("settings:save:adi", bool2int(enableSaveAdi));
    XML.setValue("settings:save:dailyCsv", bool2int(enableDailyLogCsv));
    XML.setValue("settings:threshold:maxDuration", thresholdMaxDuration);
    XML.setValue("settings:threshold:lockTime", thresholdLockTime);
    XML.setValue("settings:threshold:longEcho", thresholdLongEcho);
    XML.setValue("settings:threshold:dopplerShift", thresholdDopplerShift);
    XML.setValue("settings:threshold:dispersion", thresholdDispersion);
    XML.setValue("settings:threshold:bipolar", thresholdBipolar);
    XML.setValue("settings:threshold:amp", thresholdAmp);
    XML.setValue("settings:antenna:phasediff0", antPhaseDiff[0]);
    XML.setValue("settings:antenna:phasediff1", antPhaseDiff[1]);
    XML.setValue("settings:antenna:phasediff2", antPhaseDiff[2]);
    XML.setValue("settings:antenna:phasediff3", antPhaseDiff[3]);
    XML.setValue("settings:antenna:phasediff4", antPhaseDiff[4]);

    XML.save("settings.xml");
  };

  // adiファイルのパス
  char * SRC_PATH;

  // 一時保存パス
  char * CACHE_PATH;

  // 出力パス
  char * DST_PATH;

  // 中央アンテナのチャンネル
  int CENTER_ANT;

  // 東アンテナのチャンネル
  int EAST_ANT;

  // 南アンテナのチャンネル
  int SOUTH_ANT;

  // 西アンテナのチャンネル
  int WEST_ANT;

  // 北アンテナのチャンネル
  int NORTH_ANT;

  // FPS
  int frameRate;

  // Debug
  bool enableDebug;

  // ログ保存
  bool enableLogCsv;

  // キャプ画像
  bool enableCapture;

  // 遅延キャプチャ時間
  int delayCapTime;

  // adiファイルを保存する
  bool enableSaveAdi;

  // 1日単位でログ保存
  bool enableDailyLogCsv;

  // ノイズ判定時間
  int thresholdMaxDuration;

  // 流星判定後の連続回避
  int thresholdLockTime;

  // ロングエコーの閾値
  int thresholdLongEcho;

  // ドップラーシフトの閾値
  int thresholdDopplerShift;

  // 位相差の閾値
  int thresholdDispersion;

  // 2極化の閾値
  int thresholdBipolar;

  // スペクトログラムにしたときの増幅地
  int thresholdAmp;

  // 内部位相差
  vector<int> antPhaseDiff;

};

