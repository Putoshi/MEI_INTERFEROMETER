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


    // adi�t�@�C���̃p�X
    string src_s = XML.getValue("settings:path:src", "C:/Users/Putoshi/Documents/MEI/DaqLog/___DaqLog_.bak");
    SRC_PATH = new char[src_s.size() + 1]; // �������m��
    std::char_traits<char>::copy(SRC_PATH, src_s.data(), src_s.size() + 1);

    // adi�t�@�C���̏o�̓p�X
    string dst_s = XML.getValue("settings:path:dest", "C:/Users/Putoshi/Documents/MEI/DaqLog/__DaqLog_.bak");
    DST_PATH = new char[dst_s.size() + 1]; // �������m��
    std::char_traits<char>::copy(DST_PATH, dst_s.data(), dst_s.size() + 1);

    // �`�����l���̐ݒ�
    CENTER_ANT = XML.getValue("settings:channel:centerAnt", 0);
    EAST_ANT = XML.getValue("settings:channel:eastAnt", 4);
    SOUTH_ANT = XML.getValue("settings:channel:southAnt", 2);

    // FPS
    frameRate = XML.getValue("settings:fps", 60);

    // ���O�ۑ�
    enableLogTxt = int2bool(XML.getValue("settings:save:txt", 0));

    // �L���v�摜
    enableCapture = int2bool(XML.getValue("settings:save:captureImage", 0));

    // adi�t�@�C���ۑ�
    enableSaveAdi = int2bool(XML.getValue("settings:save:adi", 0));

    // �ʑ�����臒l �u��
    thresholdDispersion = XML.getValue("settings:threshold:dispersion", 5.0f);

    // �ʑ�����臒l �u��
    thresholdDispersion = XML.getValue("settings:threshold:dispersion", 5.0f);

    // 2�ɉ���臒l
    thresholdBipolar = XML.getValue("settings:threshold:bipolar", 45);

    //delete[] cstr; // ���������

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

  // adi�t�@�C���̃p�X
  char * SRC_PATH;

  // �ꎞ�ۑ��p�X
  char * DST_PATH;

  // �����A���e�i�̃`�����l��
  int CENTER_ANT;

  // ���A���e�i�̃`�����l��
  int EAST_ANT;

  // ��A���e�i�̃`�����l��
  int SOUTH_ANT;

  // FPS
  int frameRate;

  // ���O�ۑ�
  bool enableLogTxt;
  
  // �L���v�摜
  bool enableCapture;

  // adi�t�@�C����ۑ�����
  bool enableSaveAdi;

  // �ʑ�����臒l
  int thresholdDispersion;

  // 2�ɉ���臒l
  int thresholdBipolar;

};

