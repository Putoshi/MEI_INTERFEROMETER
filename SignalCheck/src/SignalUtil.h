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
	int N;
	float FFT_SPAN;						//FFTする間隔 ms

	std::vector<float *> parseBinary(const int frameCnt, const std::vector<int16_t>& targetVector);
	bool checkIsLittleEndian();
	const size_t getFileByteSize(std::ifstream& file);
	void convertSigned16bitIntEndian(std::vector<int16_t>* target_vector);

	bool readSigned16bitIntBinary(const std::string& file_full_path, std::vector<int16_t>* targetVector);
	bool writeSigned16bitIntBinary(const std::string& file_full_path, const std::vector<int16_t>& targetVector);

private:
	
};

