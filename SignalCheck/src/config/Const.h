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

    // �ǂݍ��ݒ���adi�t�@�C���̃L���b�V���o�̓p�X
    string cache_s = XML.getValue("settings:path:cache", "./tmp/Cache/DaqLog.bak");
    CACHE_PATH = new char[cache_s.size() + 1]; // �������m��
    std::char_traits<char>::copy(CACHE_PATH, cache_s.data(), cache_s.size() + 1);

    // adi�t�@�C���̏o�̓p�X
    string dst_s = XML.getValue("settings:path:dest", "./tmp/");
    DST_PATH = new char[dst_s.size() + 1]; // �������m��
    std::char_traits<char>::copy(DST_PATH, dst_s.data(), dst_s.size() + 1);

    // �`�����l���̐ݒ�
    CENTER_ANT = XML.getValue("settings:channel:centerAnt", 0);
    EAST_ANT = XML.getValue("settings:channel:eastAnt", 3);
    SOUTH_ANT = XML.getValue("settings:channel:southAnt", 2);
    WEST_ANT = XML.getValue("settings:channel:westAnt", 4);
    NORTH_ANT = XML.getValue("settings:channel:northAnt", 1);

    // FPS
    frameRate = XML.getValue("settings:fps", 60);

    // Debug
    enableDebug = int2bool(XML.getValue("settings:debug", 0));

    // ���O�ۑ�
    enableLogCsv = int2bool(XML.getValue("settings:save:csv", 0));

    // �L���v�摜
    enableCapture = int2bool(XML.getValue("settings:save:captureImage", 0));

    // �x���L���v����
    delayCapTime = XML.getValue("settings:save:delayCaptureTime", 5);

    // adi�t�@�C���ۑ�
    enableSaveAdi = int2bool(XML.getValue("settings:save:adi", 0));

    // �f�C���[�̃��O�ۑ�
    enableDailyLogCsv = int2bool(XML.getValue("settings:save:dailyCsv", 0));

    // �m�C�Y����
    thresholdMaxDuration = XML.getValue("settings:threshold:maxDuration", 30.0f);

    // ���������̘A�����
    thresholdLockTime = XML.getValue("settings:threshold:lockTime", 0);

    // �����O�G�R�[��臒l
    thresholdLongEcho = XML.getValue("settings:threshold:longEcho", 10);

    // �h�b�v���[�V�t�g��臒l
    thresholdDopplerShift = XML.getValue("settings:threshold:dopplerShift", 10);

    // �ʑ�����臒l �u��
    thresholdDispersion = XML.getValue("settings:threshold:dispersion", 5.0f);

    // 2�ɉ���臒l
    thresholdBipolar = XML.getValue("settings:threshold:bipolar", 45);

    // �X�y�N�g���O�����ɂ����Ƃ��̑����n
    thresholdAmp = XML.getValue("settings:threshold:amp", 1);

    // �����ʑ���
    antPhaseDiff.resize(5);
    antPhaseDiff[0] = XML.getValue("settings:antenna:phasediff0", 0);
    antPhaseDiff[1] = XML.getValue("settings:antenna:phasediff1", 0);
    antPhaseDiff[2] = XML.getValue("settings:antenna:phasediff2", 0);
    antPhaseDiff[3] = XML.getValue("settings:antenna:phasediff3", 0);
    antPhaseDiff[4] = XML.getValue("settings:antenna:phasediff4", 0);

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

  // adi�t�@�C���̃p�X
  char * SRC_PATH;

  // �ꎞ�ۑ��p�X
  char * CACHE_PATH;

  // �o�̓p�X
  char * DST_PATH;

  // �����A���e�i�̃`�����l��
  int CENTER_ANT;

  // ���A���e�i�̃`�����l��
  int EAST_ANT;

  // ��A���e�i�̃`�����l��
  int SOUTH_ANT;

  // ���A���e�i�̃`�����l��
  int WEST_ANT;

  // �k�A���e�i�̃`�����l��
  int NORTH_ANT;

  // FPS
  int frameRate;

  // Debug
  bool enableDebug;

  // ���O�ۑ�
  bool enableLogCsv;

  // �L���v�摜
  bool enableCapture;

  // �x���L���v�`������
  int delayCapTime;

  // adi�t�@�C����ۑ�����
  bool enableSaveAdi;

  // 1���P�ʂŃ��O�ۑ�
  bool enableDailyLogCsv;

  // �m�C�Y���莞��
  int thresholdMaxDuration;

  // ���������̘A�����
  int thresholdLockTime;

  // �����O�G�R�[��臒l
  int thresholdLongEcho;

  // �h�b�v���[�V�t�g��臒l
  int thresholdDopplerShift;

  // �ʑ�����臒l
  int thresholdDispersion;

  // 2�ɉ���臒l
  int thresholdBipolar;

  // �X�y�N�g���O�����ɂ����Ƃ��̑����n
  int thresholdAmp;

  // �����ʑ���
  vector<int> antPhaseDiff;

};

