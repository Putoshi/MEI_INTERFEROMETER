#include "Hough.h"

using namespace std;

Hough::Hough()
{
}


Hough::~Hough()
{
}

void Hough::init() {
  w = 50;
  h = 600;
}

const int kAngleSplits = 16;

// ‰æ‘œ‚ğHough•ÏŠ·‚·‚é(’¼ü•Î)
std::vector<int16_t> Hough::getHoughLine(unsigned char * src_image) {
  diagonal = (int)sqrt(w * w + h * h);
  int d2 = diagonal * 2;
  int dst_image_len = kAngleSplits * d2 * 3;

  //std::cerr << dst_image_len << std::endl;

  //unsigned char dst_image[3698688];
  std::vector<int16_t> dst_image;
  //ArrayList<Integer> dst_image = new ArrayList<Integer>(kAngleSplits * d2);
  for (int r = 0; r < d2; r++) {
    for (int t = 0; t < kAngleSplits; t++) {
      dst_image.push_back(0);
    }
  }

  

  // À•W•ÏŠ·‚ğs‚¢AŒ‹‰Ê‚É‰ÁZ‚µ‚Ä‚¨‚­
  //for (int y = 0; y < h; y++) {
  //  for (int x = 0; x < w; x++) {
  //    if (src_image.get(y * w + x) == 0) continue;
  //    for (int t = 0; t < kAngleSplits; t++) {
  //      int r = (int)(x * cos_table.get(t) + y * sin_table.get(t) + 0.5);
  //      int p = (r + diagonal) * kAngleSplits + t;
  //      pixs.set(p, dst_image.get(p) + 1);
  //    }
  //  }
  //}
  return dst_image;
}