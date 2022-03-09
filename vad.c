#include <stdint.h>
#include <x86intrin.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v2_darken(union Pixel* first, union Pixel* last, int darkness);

void va_darken(union Pixel* first, union Pixel* last, int darkness)
{
  int lightness = 256 - darkness;
  __m256i mask_rb = _mm256_set1_epi16(0x00ff);
  __m256i mult_rb = _mm256_set1_epi16(lightness);
  __m256i mult_ga = _mm256_set1_epi32((256 << 16) | lightness);
  for (; first < last-7; first += 8) {
    __m256i p = _mm256_loadu_si256((__m256i*)first);
    __m256i rb = _mm256_and_si256(mask_rb, _mm256_srli_epi16(_mm256_mullo_epi16(mult_rb, _mm256_and_si256(mask_rb, p)), 8));
    __m256i ga = _mm256_andnot_si256(mask_rb, _mm256_mullo_epi16(mult_ga, _mm256_and_si256(mask_rb, _mm256_srli_epi16(p, 8))));
    _mm256_storeu_si256((__m256i*)first, _mm256_or_si256(rb, ga));
  }
  v2_darken(first, last, darkness);
}
