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
    string src_s = XML.getValue("settings:path:src", "C:/Users/Putoshi/Documents/MEI/DaqLog/DaqLog_.bak");
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

    //delete[] cstr; // ���������

    /*std::cerr << SRC_PATH << std::endl;*/
  };
  ofxXmlSettings XML;

public:
  static Const& Const::getInstance() {
    static Const cst;
    return cst;
  }

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


};

