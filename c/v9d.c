#include <stdint.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v2_darken(union Pixel* first, union Pixel* last, int darkness);

void v9_darken(union Pixel* first, union Pixel* last, int darkness)
{
  uint32_t lightness = 256 - darkness;
  uint64_t mask_rb = 0x00ff00ff00ff00ff;
  uint64_t mask_g = 0x0000ff000000ff00;
  uint64_t mask_a = 0xff000000ff000000;
  for (; first < last-1; first += 2) {
      uint64_t p = first[0].v | ((uint64_t)first[1].v << 32);
      uint64_t rb = ((p & mask_rb) * lightness / 256) & mask_rb;
      uint64_t g = ((p & mask_g) * lightness / 256) & mask_g;
      uint64_t a = p & mask_a;
      uint64_t res = rb | g | a;
      first[0].v = (uint32_t)res;
      first[1].v = (uint32_t)(res >> 32);
  }
  v2_darken(first, last, darkness);
}
