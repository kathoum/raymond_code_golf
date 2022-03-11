#include <stdint.h>
#include <immintrin.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v2_darken(union Pixel* first, union Pixel* last, int darkness);

void vc_darken(union Pixel* first, union Pixel* last, int darkness)
{
  int lightness = 256 - darkness;
  uint64_t multiplier = lightness * (uint64_t)0x000100010001 + ((uint64_t)256 << 48);
  __m256i mult = _mm256_set1_epi64x(multiplier);
  for (; first < last-7; first += 8) {
    __m256i val = _mm256_loadu_si256((__m256i*)first);
    __m256i vlo = _mm256_unpacklo_epi8(val, _mm256_setzero_si256());
    vlo = _mm256_mullo_epi16(vlo, mult);
    vlo = _mm256_srli_epi16(vlo, 8);
    __m256i vhi = _mm256_unpackhi_epi8(val, _mm256_setzero_si256());
    vhi = _mm256_mullo_epi16(vhi, mult);
    vhi = _mm256_srli_epi16(vhi, 8);
    val = _mm256_packus_epi16(vlo, vhi);
    _mm256_storeu_si256((__m256i*)first, val);
  }
  v2_darken(first, last, darkness);
}
