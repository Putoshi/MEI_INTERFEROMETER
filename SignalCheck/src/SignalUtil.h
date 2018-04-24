#pragma once
#include "./config/Const.h"
#include "./util/FileUtil.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iterator>
#include <windows.h>  // Sleep�p

class SignalUtil
{
public:
  SignalUtil();
  ~SignalUtil();

  void init();

  const int IDX_BODY = 8 * 4 + 4;
  int CHANNELS;

  // FFT SETTING
  float AD_1S_N;						// AD�{�[�h��1�b���Ƃ̕W�{��
  int N;								// FFT �W�{��
  float FFT_SPAN;						// FFT����Ԋu ms

  // adi�o�C�i������͂��ĐM����Ԃ�
  std::vector<float *> parseBinary(const int frameCnt);

  // ���g���G���f�B�A�� �r�b�O�G���f�B�A���̔���
  bool checkIsLittleEndian();

  // �o�C�i���̃t�@�C���T�C�Y����
  const size_t getFileByteSize(std::ifstream& file);

  // 16bit �� Int16 Convert
  void convertSigned16bitIntEndian(std::vector<int16_t>* target_vector);

  // Read 16bit Binary File
  bool readSigned16bitIntBinary(const std::string& file_full_path, int _binIdx);

  // Write 16bit Binary File
  bool writeSigned16bitIntBinary(const std::string& file_full_path);

private:
  //std::vector<std::vector<int16_t>> binValues;						// �o�C�i��
  std::vector<std::vector<int16_t>> binValues;						// �o�C�i��
  int binIdx;
  int totalCnt;								// FFT �W�{��
  int onceReadRow;            // ���adi��ǂݍ��񂾎��Ɏ擾�ł���s��
  bool isReading;

};

