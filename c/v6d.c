#include <stdint.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v6_darken(union Pixel* first, union Pixel* last, int darkness)
{
  uint32_t lightness = 256 - darkness;
  for (; first < last; ++first) {
      uint32_t p = first->v;
      uint32_t c0 = ((p & 0xff) * lightness / 256) & 0xff;
      uint32_t c1 = ((p & 0xff00) * lightness / 256) & 0xff00;
      uint32_t c2 = ((p & 0xff0000) * lightness / 256) & 0xff0000;
      uint32_t c3 = p & 0xff000000;
      first->v = c0 | c1 | c2 | c3;
  }
}
