#include <stdint.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

static uint32_t pack_fields(uint8_t r, uint8_t g, uint8_t b) {
  return r | (g << 10) | (b << 20);
}

void v3_darken(union Pixel* first, union Pixel* last, int darkness)
{
  int factor = darkness / 8;
  for (; first < last; ++first) {
    uint32_t fields = pack_fields(
        first->c[0], first->c[1], first->c[2]);
    fields *= factor;
    fields += pack_fields(31, 31, 31);
    first->c[0] -= (fields >>  5) & 31;
    first->c[1] -= (fields >> 15) & 31;
    first->c[2] -= (fields >> 25) & 31;
  }
}
