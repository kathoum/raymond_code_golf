#include <stdint.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v1_darken(union Pixel* first, union Pixel* last, int darkness)
{
  int lightness = 256 - darkness;
  for (; first < last; ++first) {
    for (int i = 0; i < 3; ++i) {
      first->c[i] = (uint8_t)(first->c[i] * lightness / 256);
    }
  }
}
