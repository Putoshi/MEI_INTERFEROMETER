#pragma once

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iterator>

class SignalUtil
{
public:
  SignalUtil();
  ~SignalUtil();

  const int IDX_BODY = 8 * 4 + 4;
  int CHANNELS;

  // FFT SETTING
  float AD_1S_N;						// ADボードの1秒ごとの標本数
  int N;								// FFT 標本数
  float FFT_SPAN;						// FFTする間隔 ms

  // adiバイナリを解析して信号を返す
  std::vector<float *> parseBinary(const int frameCnt);

  // リトルエンディアン ビッグエンディアンの判定
  bool checkIsLittleEndian();

  // バイナリのファイルサイズ測定
  const size_t getFileByteSize(std::ifstream& file);

  // 16bit → Int16 Convert
  void convertSigned16bitIntEndian(std::vector<int16_t>* target_vector);

  // Read 16bit Binary File
  bool readSigned16bitIntBinary(const std::string& file_full_path);

  // Write 16bit Binary File
  bool writeSigned16bitIntBinary(const std::string& file_full_path);

private:
  std::vector<int16_t> binValues;						// バイナリ
  int totalCnt;								// FFT 標本数
  int onceReadRow;            // 一回adiを読み込んだ時に取得できる行数

};

