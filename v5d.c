#include <stdint.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v5_darken(union Pixel* first, union Pixel* last, int darkness)
{
  uint32_t lightness = 256 - darkness;
  for (; first < last; ++first) {
      union Pixel p = *first;
      p.c[0] = p.c[0] * lightness / 256;
      p.c[1] = p.c[1] * lightness / 256;
      p.c[2] = p.c[2] * lightness / 256;
      *first = p;
  }
}
