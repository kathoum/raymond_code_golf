#include <stdint.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

static uint32_t pack_fields(uint8_t r, uint8_t g, uint8_t b) {
  return r | (g << 10) | (b << 20);
}

void v8_darken(union Pixel* first, union Pixel* last, int darkness)
{
  int factor = darkness / 8;
  uint32_t mask2 = factor >= 2 ? 0xFFFFFFFF : 0;
  uint32_t mask3 = factor >= 3 ? 0xFFFFFFFF : 0;
  for (; first < last; ++first) {
    uint32_t v = first->v;
    uint32_t fields = (v & 0xFF) |
                     ((v & 0xFF00) << 2) |
                     ((v & 0xFF0000) << 4);
    fields += (fields & mask2) + (fields & mask3);
    fields += pack_fields(31, 31, 31);
    v -= (fields >> 5) & 0x1F;
    v -= (fields >> 7) & 0x1F00;
    v -= (fields >> 9) & 0x1F0000;
    first->v = v;
  }
}
