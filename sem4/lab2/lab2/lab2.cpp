#include <cstdio>
#include <cstdint>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>
#include "dvec.h"

__m64 mmx_saturating_add(__m64 a, __m64 b) {
    return _mm_adds_pu8(a, b);
}

__m128 sse_compare_shuffle(__m128 a, __m128 b) {
    __m128 cmp = _mm_cmpgt_ps(a, b);
    __m128 shuf = _mm_shuffle_ps(a, b, 0x1B);
    return _mm_and_ps(shuf, cmp);
}

__m256 avx_hadd(__m256 a, __m256 b) {
    return _mm256_hadd_ps(a, b);
}

void demo_dvec() {
    unsigned char uc1[16] = { 200, 100, 150, 50, 255, 100, 200, 50, 10, 20, 30, 40, 50, 60, 70, 80 };
    unsigned char uc2[16] = { 100, 200, 150, 250, 1, 200, 100, 250, 1, 2, 3, 4, 5, 6, 7, 8 };

    __m128i m1 = _mm_loadu_si128((__m128i*)uc1);
    __m128i m2 = _mm_loadu_si128((__m128i*)uc2);

    Iu8vec16 a(m1), b(m2);
    Iu8vec16 c = sat_add(a, b);

    printf("=== dvec: Iu8vec16 sat_add (paddusb) ===\n");

    printf("arr1:   ");
    for (int i = 0; i < 16; ++i)
        printf("%d ", uc1[i]);
    printf("\n");

    printf("arr2:   ");
    for (int i = 0; i < 16; ++i)
        printf("%d ", uc2[i]);
    printf("\n");

    printf("result: ");
    for (int i = 0; i < 16; ++i)
        printf("%d ", (unsigned char)c[i]);
    printf("\n\n");

    float f1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    float f2[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };

    __m256 y1 = _mm256_loadu_ps(f1);
    __m256 y2 = _mm256_loadu_ps(f2);

    F32vec8 fa(y1), fb(y2);
    F32vec8 fc = fa + fb;

    printf("=== dvec: F32vec8 add (vaddps) ===\n");

    printf("arr1:   ");
    for (int i = 0; i < 8; ++i)
        printf("%.0f ", f1[i]);
    printf("\n");

    printf("arr2:   ");
    for (int i = 0; i < 8; ++i)
        printf("%.0f ", f2[i]);
    printf("\n");

    printf("result: ");
    for (int i = 0; i < 8; ++i)
        printf("%.0f ", (float)fc[i]);
    printf("\n\n");
}

int main() {
    unsigned char mmx_arr1[8] = { 200, 100, 150, 50, 255, 100, 200, 50 };
    unsigned char mmx_arr2[8] = { 100, 200, 150, 250, 1, 200, 100, 250 };
    unsigned char mmx_result[8] = {};

    __m64 mmx_a, mmx_b;
    mmx_a.m64_u64 = *(unsigned long long*)mmx_arr1;
    mmx_b.m64_u64 = *(unsigned long long*)mmx_arr2;

    __m64 mmx_res = mmx_saturating_add(mmx_a, mmx_b);
    _mm_empty();

    *(unsigned long long*)mmx_result = mmx_res.m64_u64;

    printf("=== MMX: paddusb (saturating add unsigned bytes) ===\n");

    printf("arr1:   ");
    for (int i = 0; i < 8; i++)
        printf("%d ", mmx_arr1[i]);
    printf("\n");

    printf("arr2:   ");
    for (int i = 0; i < 8; i++)
        printf("%d ", mmx_arr2[i]);
    printf("\n");

    printf("result: ");
    for (int i = 0; i < 8; i++)
        printf("%d ", mmx_result[i]);
    printf("\n\n");

    float sse_arr1[4] = { 1.5f, 3.0f, 2.0f, 4.5f };
    float sse_arr2[4] = { 2.0f, 1.0f, 4.0f, 3.0f };

    __m128 sse_a = _mm_loadu_ps(sse_arr1);
    __m128 sse_b = _mm_loadu_ps(sse_arr2);
    __m128 sse_res = sse_compare_shuffle(sse_a, sse_b);

    float sse_out[4];
    _mm_storeu_ps(sse_out, sse_res);

    printf("=== SSE: cmpps (predicate 6) + shufps 0x1B ===\n");

    printf("arr1:           ");
    for (int i = 0; i < 4; i++)
        printf("%.1f  ", sse_arr1[i]);
    printf("\n");

    printf("arr2:           ");
    for (int i = 0; i < 4; i++)
        printf("%.1f  ", sse_arr2[i]);
    printf("\n");

    printf("shuf masked by cmp: ");
    unsigned int* bits = (unsigned int*)sse_out;
    for (int i = 0; i < 4; i++)
        printf("0x%08X ", bits[i]);
    printf("\n\n");

    float avx_arr1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    float avx_arr2[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };

    __m256 avx_a = _mm256_loadu_ps(avx_arr1);
    __m256 avx_b = _mm256_loadu_ps(avx_arr2);
    __m256 avx_res = avx_hadd(avx_a, avx_b);

    float avx_out[8];
    _mm256_storeu_ps(avx_out, avx_res);

    printf("=== AVX: vhaddps (horizontal add) ===\n");

    printf("arr1:   ");
    for (int i = 0; i < 8; i++)
        printf("%.0f ", avx_arr1[i]);
    printf("\n");

    printf("arr2:   ");
    for (int i = 0; i < 8; i++)
        printf("%.0f ", avx_arr2[i]);
    printf("\n");

    printf("result: ");
    for (int i = 0; i < 8; i++)
        printf("%.0f ", avx_out[i]);
    printf("\n\n");

    demo_dvec();

    return 0;
}