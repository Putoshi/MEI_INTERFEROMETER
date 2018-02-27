#include "SignalUtil.h"

using namespace std;

SignalUtil::SignalUtil()
{
}


SignalUtil::~SignalUtil()
{
}

void SignalUtil::init() {

  binIdx = 0;
  binValues.resize(2);
  isReading = false;

  // .adiファイルを開く
  bool isReaded = readSigned16bitIntBinary(SRC_FILE, 0);
  // _DaqLog.bakファイルに保存
  bool isWrote = writeSigned16bitIntBinary(DST_FILE);

  if (isReaded && isWrote) {
    remove(DST_FILE); // ファイル削除
  }
}

std::vector<float *> SignalUtil::parseBinary(const int frameCnt) {
  totalCnt++;

  // 事前読み込み
  if (totalCnt > floor((onceReadRow / AD_1S_N) / (FFT_SPAN / 1000)) * 0.8 && !isReading) {
    isReading = true;
    readSigned16bitIntBinary(SRC_FILE, (binIdx + 1) % 2);
  }
  //std::cerr << floor((onceReadRow / AD_1S_N) / (FFT_SPAN / 1000)) << std::endl;

  // 14s読み込んだらリセット
  if (totalCnt >= floor((onceReadRow / AD_1S_N) / (FFT_SPAN / 1000)) ) {
    std::cerr << "LOOP" << std::endl;
    totalCnt = 1;
    binIdx = (binIdx + 1) % 2;
    isReading = false;
  }

  //const size_t fileSize = targetVector.size() * 2; // int16_t (16 bit) is 2 byte.
  //std::cerr << frameCnt << std::endl;
  std::vector<float *> signal(CHANNELS);
  for (int i = 0; i < CHANNELS; i++) {
    float * sig = (float *)malloc(sizeof(float) * N);
    signal[i] = sig;
  }

  for (int j = 0, size = N; j < size; ++j) {

    // インデックスADボードの時間単位の標本数と
    // FFTに掛ける時間単位の標本数(2のべき乗)が合わないので、
    // ADボードの標本数に合わせたインデックスを与える
    int idx = totalCnt *  roundf(AD_1S_N * (FFT_SPAN / 1000)) + j;
    //std::cerr << idx << " ";

    std::vector<unsigned short> data(8);

    for (int k = 0, size = 8; k < size; ++k) {


      char buf[20];
     
      snprintf(buf, 20, "%#x", binValues[binIdx][idx * 16 + k * 2 + IDX_BODY]);

      char t[5];
      strncpy(t, &buf[strlen(buf) - 4], 4);
      t[4] = '\0';            //取り出した文字数分の最後に'\0'を入れる

      char hexStr[7];
      hexStr[0] = '0';
      hexStr[1] = 'x';
      hexStr[2] = t[0];
      hexStr[3] = t[1];
      hexStr[4] = t[2];
      hexStr[5] = t[3];
      hexStr[6] = '\0';
      //puts(hexStr);

      unsigned short number = (unsigned short)strtoul(hexStr, NULL, 0);
      //std::cerr << number << " ";

      //long lon = (long)number;
      data[k] = number;
    }
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 0]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 2]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 4]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 6]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 8]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 10]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 12]);
    //printf("%02x ", targetVector[idx * 16 + IDX_BODY + 14]);

    for (int i = 0; i < CHANNELS; ++i) {
      long lon = (long)data[i];
      float flo = (float)lon;
      signal[i][j] = (flo - 65535 / 2) / (65535 / 2);
    }
    //std::cerr << sig[j] << std::endl;
  }

  

  //cout << endl;
  return signal;
}


bool SignalUtil::checkIsLittleEndian() {
  const int16_t value = 1;
  return (*(char*)&value) ? true : false;
}

const size_t SignalUtil::getFileByteSize(std::ifstream& file) {
  file.seekg(0, std::ios::end);
  const size_t fileSize = (size_t)file.tellg();
  file.seekg(0, std::ios::beg);

  return fileSize;
}

void SignalUtil::convertSigned16bitIntEndian(std::vector<int16_t>* targetVector) {
  for (auto& value : *targetVector) {
    value = (value << 8) | ((value >> 8) & 0xFF); // The most left value does not change even if bit-shifted, so the mask by 0xFF is necessary.
  }
}

bool SignalUtil::readSigned16bitIntBinary(const std::string& file_full_path, int _binIdx) {
  //std::cerr << "readSigned16bitIntBinary START" << std::endl;
  std::ifstream file(file_full_path, std::ios::in | std::ios::binary);
  if (!file) {
    std::cout << "Error: The file path is incorrect. There is no file." << std::endl;
    return false;
  }
  const size_t fileSize = getFileByteSize(file);
  binValues[_binIdx].clear();
  binValues[_binIdx].resize(fileSize / 2); // 16 bit is 2 bytes.

  file.read((char*)binValues[_binIdx].data(), fileSize);
  /*if (checkIsLlittleEndian()) {
  std::cout << "is Little endian" << std::endl;
  convertSigned16bitIntEndian(targetVector);
  }
  else {
  std::cout << "is Big endian" << std::endl;
  }*/

  //convertSigned16bitIntEndian(targetVector);
  file.close();

  onceReadRow = (binValues[_binIdx].size() - IDX_BODY) / 16;

  //std::cerr << "readSigned16bitIntBinary END" << std::endl;
  return true;
}

bool SignalUtil::writeSigned16bitIntBinary(const std::string& file_full_path) {
  std::ofstream file(file_full_path, std::ios::out | std::ios::binary);
  if (!file) {
    std::cout << "Error: The file to write could not be opend." << std::endl;
    return false;
  }
  const size_t fileSize = binValues[binIdx].size() * 2; // int16_t (16 bit) is 2 byte.
  if (checkIsLittleEndian()) {
    std::vector<int16_t> temp_vector = binValues[binIdx]; // deep copy.
    convertSigned16bitIntEndian(&temp_vector);
    file.write((char*)temp_vector.data(), fileSize);
  }
  else {
    file.write((char*)binValues[binIdx].data(), fileSize);
  }

  file.close();
  return true;
}
