#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

union Pixel
{
    uint8_t c[4]; // four channels: red, green, blue, alpha
    uint32_t v;   // full pixel value as a 32-bit integer
};

void v1_darken(union Pixel* first, union Pixel* last, int darkness);
void v2_darken(union Pixel* first, union Pixel* last, int darkness);
void v3_darken(union Pixel* first, union Pixel* last, int darkness);
void v4_darken(union Pixel* first, union Pixel* last, int darkness);
void v5_darken(union Pixel* first, union Pixel* last, int darkness);
void v6_darken(union Pixel* first, union Pixel* last, int darkness);
void v7_darken(union Pixel* first, union Pixel* last, int darkness);
void v8_darken(union Pixel* first, union Pixel* last, int darkness);
void v9_darken(union Pixel* first, union Pixel* last, int darkness);
void va_darken(union Pixel* first, union Pixel* last, int darkness);

void fill(union Pixel* first, size_t count);

void timing(
        const char* name, void (*darken)(union Pixel* first, union Pixel* last, int darkness),
        union Pixel* image, size_t len, int darkness, union Pixel* reference, clock_t reftime) {
    size_t size_bytes = len * sizeof(union Pixel);
    union Pixel* copy = (union Pixel*)malloc(size_bytes);
    memcpy(copy, image, size_bytes);
    clock_t t = clock();
    darken(copy, copy+len, darkness);
    t = clock() - t;
    printf("%s: %f ms (%.2fx)", name, t * 1e3 / CLOCKS_PER_SEC, (double)reftime / t);
    if (memcmp(reference, copy, size_bytes) != 0) {
        printf(" === not matching!");
    }
    printf("\n");
}

int main(int argc, char** argv) {
    size_t len = 10000*10000;
    size_t size_bytes = len * sizeof(union Pixel);
    union Pixel* image = (union Pixel*)malloc(size_bytes);
    fill(image, len);

    int darkness = argc > 1 ? atoi(argv[1]) : 24;

    union Pixel* v1 = (union Pixel*)malloc(size_bytes);
    memcpy(v1, image, size_bytes);
    clock_t t1 = clock();
    v1_darken(v1, v1+len, darkness);
    t1 = clock() - t1;
    printf("v1: %f ms\n", t1 * 1e3 / CLOCKS_PER_SEC);

    timing("v2", v2_darken, image, len, darkness, v1, t1);
    timing("v3", v3_darken, image, len, darkness, v1, t1);
    timing("v4", v4_darken, image, len, darkness, v1, t1);
    timing("v5", v5_darken, image, len, darkness, v1, t1);
    timing("v6", v6_darken, image, len, darkness, v1, t1);
    timing("v7", v7_darken, image, len, darkness, v1, t1);
    timing("v8", v8_darken, image, len, darkness, v1, t1);
    timing("v9", v9_darken, image, len, darkness, v1, t1);
    timing("va", va_darken, image, len, darkness, v1, t1);
}

/*  xoshiro128++ 1.0

Written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

uint32_t rotl(uint32_t x, int k) {
    return (x << k) | (x >> (32 - k));
}

void fill(union Pixel* first, size_t count) {
    uint32_t s[4] = {751175984, 3028317382, 2043922487, 508515939};
    for (size_t i = 0; i < count; ++i) {
        first[i].v = rotl(s[0] + s[3], 7) + s[0];

        uint32_t t = s[1] << 9;
        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];
        s[2] ^= t;
        s[3] = rotl(s[3], 11);
    }
}
