#include <stdint.h>
#include <x86intrin.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v2_darken(union Pixel* first, union Pixel* last, int darkness);

void v7_darken(union Pixel* first, union Pixel* last, int darkness)
{
  uint16_t lightness = 256 - darkness;
  uint64_t multiplier_16x4 = lightness * (uint64_t)0x000100010001 + ((uint64_t)256 << 48);
  __m256i multiplier_16x16 = _mm256_set1_epi64x(multiplier_16x4);
  for (; first < last-7; first += 8) {
    __m128i p1_8x16 = _mm_loadu_si128((__m128i*)first);
    __m128i p2_8x16 = _mm_loadu_si128((__m128i*)first + 1);
    __m256i p1_16x16 = _mm256_cvtepu8_epi16(p1_8x16);
    __m256i p2_16x16 = _mm256_cvtepu8_epi16(p2_8x16);
    __m256i q1_16x16 = _mm256_srli_epi16(_mm256_mullo_epi16(p1_16x16, multiplier_16x16), 8);
    __m256i q2_16x16 = _mm256_srli_epi16(_mm256_mullo_epi16(p2_16x16, multiplier_16x16), 8);
    __m256i q_8x32 = _mm256_permute4x64_epi64(_mm256_packus_epi16(q1_16x16, q2_16x16), 0b11011000);
    _mm256_storeu_si256((__m256i*)first, q_8x32);
  }
  v2_darken(first, last, darkness);
}
