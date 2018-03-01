#include "Hough.h"

using namespace std;

Hough::Hough()
{
}


Hough::~Hough()
{
}


const int kAngleSplits = 128 * 2 * 4;
const int kMinCount = 10;       //�J�E���g�����̐������Ȃ疳������
const float kTableConst = M_PI / kAngleSplits;
const float kDegRad = 180 / M_PI;    //�ϊ��p�萔

void Hough::init() {
  w = 600;
  h = 50;

  // ���l�v�Z�p�̃e�[�u�������O�ɗp�ӂ���
  sin_table = std::vector<float>(kAngleSplits);
  cos_table = std::vector<float>(kAngleSplits);
  for (int t = 0; t < kAngleSplits; t++) {
    sin_table[t] = sin(kTableConst * t);
    cos_table[t] = cos(kTableConst * t);
    //std::cerr << cos_table[t] << std::endl;
  }
 
}



// �摜��Hough�ϊ�����(������)
std::vector<int16_t> Hough::getHoughLine(std::vector<int16_t> src_image) {
  diagonal = (int)sqrt(w * w + h * h);
  d2 = diagonal * 2;
  //int dst_image_len = kAngleSplits * d2;

  //std::cerr << src_image.size() << std::endl;

  //unsigned char dst_image[3698688];
  std::vector<int16_t> dst_image(kAngleSplits * d2);
  for (int r = 0; r < d2; r++) {
    for (int t = 0; t < kAngleSplits; t++) {
      dst_image.push_back(0);
    }
  }

  // ���W�ϊ����s���A���ʂɉ��Z���Ă���
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      //std::cerr << src_image[y * w + x] << std::endl;
      if (src_image[y * w + x] == 0) continue;
      //std::cerr << "-- " << src_image[y * w + x] << std::endl;
      for (int t = 0; t < kAngleSplits; t++) {
        int r = (int)(x * cos_table[t] + y * sin_table[t] + 0.5);
        int p = (r + diagonal) * kAngleSplits + t;
        dst_image[p] = dst_image[p] + 1;
      }
    }
  }
  return dst_image;
}


// ���������o����
vector<Tuple2> Hough::calcHoughLine(std::vector<int16_t> src_image) {
  std::vector<int16_t> counter = getHoughLine(src_image);

  // �ł��J�E���g���������̂���A�����Ƃ��ĕ\������
  vector<Tuple2> result;
  //ArrayList<Tuple2> result = new ArrayList<Tuple2>();
  std::cerr << "�����̌��o���ʁF" << std::endl;
  int max_count = 0;
  while (true) {
    // �ł��J�E���g���������̂���������
    max_count = 0;
    int t_max = 0, r_max = 0;
    int cnt = 0;
    for (int r = 0; r < d2; r++) {
      for (int t = 0; t < kAngleSplits; t++) {
       
        cnt = counter[r * kAngleSplits + t];
        if (max_count < cnt) {
          max_count = cnt;
          t_max = t;
          r_max = r;
        }
      }
    }

    /*std::cerr << r_max << std::endl;
    std::cerr << diagonal << std::endl;*/

    if (max_count < kMinCount) break;
    result.push_back(Tuple2(t_max, r_max));

    //System.out.println("�@�J�E���g�F" + max_count + " �p�x�F" + (t_max * kTableConst * kDegRad) + " �����F" + (r_max - diagonal));

    std::cerr << "�@�J�E���g�F" << max_count << " �p�x�F" << ((float)t_max * kTableConst * kDegRad) << " �����F" << (r_max - diagonal) << std::endl;

    // �ގ��������ӂ̃J�E���g����������
    for (int r = -10; r <= 10; r++) {
      for (int t = -30; t <= 30; t++) {
        int t2 = t_max + t, r2 = r_max + r;
        if (t2 < 0) {
          t2 += kAngleSplits;
          r2 = -r2;
        }
        if (t2 >= kAngleSplits) {
          t2 -= kAngleSplits;
          r2 = -r2;
        }
        if (r2 < 0 || r2 >= d2) continue;
        counter[r2 * kAngleSplits + t2] = 0;
      }
    }
  }
  return result;
}