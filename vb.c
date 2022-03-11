#include <stdint.h>
#include <emmintrin.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void vb_darken(union Pixel* first, union Pixel* last, int darkness)
{
  int lightness = 256 - darkness;
  __m128i alpha128 = _mm_set_epi16(
        256, lightness, lightness, lightness,
        256, lightness, lightness, lightness);
  void* end = last;
  for (__m128i* pixels = (__m128i*)first; pixels < (__m128i*)end; pixels++) {
    __m128i val = _mm_loadu_si128(pixels);
    __m128i vlo = _mm_unpacklo_epi8(val, _mm_setzero_si128());
    vlo = _mm_mullo_epi16(vlo, alpha128);
    vlo = _mm_srli_epi16(vlo, 8);
    __m128i vhi = _mm_unpackhi_epi8(val, _mm_setzero_si128());
    vhi = _mm_mullo_epi16(vhi, alpha128);
    vhi = _mm_srli_epi16(vhi, 8);
    val = _mm_packus_epi16(vlo, vhi);
    _mm_storeu_si128(pixels, val);
  }
}
