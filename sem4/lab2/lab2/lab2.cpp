#include <cstdio>
#include <cstdint>
#include <mmintrin.h> 
#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>
#include "dvec.h"          // Подключаем библиотеку dvec.h (находится в папке проекта)

// ----------------------------------------------------------------------
// 1. MMX: сложение байт с насыщением (paddusb)
// ----------------------------------------------------------------------
__m64 mmx_saturating_add(__m64 a, __m64 b) {
    return _mm_adds_pu8(a, b);
}

// ----------------------------------------------------------------------
// 2. SSE: сравнение (cmpps) + перестановка (shufps) + маскирование (andps)
// ----------------------------------------------------------------------
__m128 sse_compare_shuffle(__m128 a, __m128 b) {
    __m128 cmp = _mm_cmpgt_ps(a, b);
    __m128 shuf = _mm_shuffle_ps(a, b, 0x1B);
    return _mm_and_ps(shuf, cmp);
}

// ----------------------------------------------------------------------
// 3. AVX: горизонтальное сложение (vhaddps)
// ----------------------------------------------------------------------
__m256 avx_hadd(__m256 a, __m256 b) {
    return _mm256_hadd_ps(a, b);
}

// ----------------------------------------------------------------------
// 4. Демонстрация работы с библиотекой dvec.h (пункт 4 задания)
// ----------------------------------------------------------------------
void demo_dvec() {
    // ---------- Целочисленные векторы (unsigned char) ----------
    unsigned char uc1[16] = { 200,100,150,50,255,100,200,50,10,20,30,40,50,60,70,80 };
    unsigned char uc2[16] = { 100,200,150,250,1,200,100,250,1,2,3,4,5,6,7,8 };

    // Загружаем данные в 128-битные регистры через интринсики, затем оборачиваем в класс Iu8vec16
    __m128i m1 = _mm_loadu_si128((__m128i*)uc1);
    __m128i m2 = _mm_loadu_si128((__m128i*)uc2);
    Iu8vec16 a(m1), b(m2);

    // Насыщенное сложение (paddusb) – функция из dvec.h
    Iu8vec16 c = sat_add(a, b);

    printf("=== dvec: Iu8vec16 sat_add (paddusb) ===\n");
    printf("arr1:   "); for (int i = 0; i < 16; ++i) printf("%d ", uc1[i]); printf("\n");
    printf("arr2:   "); for (int i = 0; i < 16; ++i) printf("%d ", uc2[i]); printf("\n");
    printf("result: "); for (int i = 0; i < 16; ++i) printf("%d ", (unsigned char)c[i]); printf("\n\n");

    // ---------- Векторы с плавающей точкой (AVX, 8 элементов) ----------
    float f1[8] = { 1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f };
    float f2[8] = { 8.0f,7.0f,6.0f,5.0f,4.0f,3.0f,2.0f,1.0f };

    // Загружаем в 256-битные регистры и создаём объекты F32vec8
    __m256 y1 = _mm256_loadu_ps(f1);
    __m256 y2 = _mm256_loadu_ps(f2);
    F32vec8 fa(y1), fb(y2);

    // Сложение (vaddps) – перегруженный оператор +
    F32vec8 fc = fa + fb;

    printf("=== dvec: F32vec8 add (vaddps) ===\n");
    printf("arr1:   "); for (int i = 0; i < 8; ++i) printf("%.0f ", f1[i]); printf("\n");
    printf("arr2:   "); for (int i = 0; i < 8; ++i) printf("%.0f ", f2[i]); printf("\n");
    printf("result: "); for (int i = 0; i < 8; ++i) printf("%.0f ", (float)fc[i]); printf("\n\n");

    // В дизассемблере для sat_add будет одна инструкция paddusb (или paddb для AVX2?),
    // для сложения – одна инструкция vaddps. Можно подсчитать общее количество машинных команд,
    // затраченных на эти операции: загрузка, собственно векторная команда, сохранение.
}

int main() {
    // ---------- MMX: paddusb (насыщение) ----------
    unsigned char mmx_arr1[8] = { 200, 100, 150,  50, 255, 100, 200,  50 };
    unsigned char mmx_arr2[8] = { 100, 200, 150, 250,   1, 200, 100, 250 };
    unsigned char mmx_result[8] = {};

    __asm {
        movq    mm0, mmx_arr1
        movq    mm1, mmx_arr2
        paddusb mm0, mm1
        movq    mmx_result, mm0
        emms
    }

    printf("=== MMX: paddusb (saturating add unsigned bytes) ===\n");
    printf("arr1:   "); for (int i = 0; i < 8; i++) printf("%d ", mmx_arr1[i]);
    printf("\narr2:   "); for (int i = 0; i < 8; i++) printf("%d ", mmx_arr2[i]);
    printf("\nresult: "); for (int i = 0; i < 8; i++) printf("%d ", mmx_result[i]);
    printf("\n\n");

    // ---------- SSE: cmpps + shufps ----------
    float sse_arr1[4] = { 1.5f, 3.0f, 2.0f, 4.5f };
    float sse_arr2[4] = { 2.0f, 1.0f, 4.0f, 3.0f };
    float sse_cmp[4] = {};
    float sse_shuf[4] = {};

    __asm {
        movups xmm0, sse_arr1
        movups xmm1, sse_arr2

        movaps xmm2, xmm0
        cmpps  xmm2, xmm1, 6          // 6 = greater than
        movups sse_cmp, xmm2

        movups xmm0, sse_arr1
        shufps xmm0, xmm1, 0x1B       // reverse order
        movups sse_shuf, xmm0
    }

    printf("=== SSE: cmpps predicate 6 (greater than) ===\n");
    printf("arr1:              "); for (int i = 0; i < 4; i++) printf("%.1f  ", sse_arr1[i]);
    printf("\narr2:              "); for (int i = 0; i < 4; i++) printf("%.1f  ", sse_arr2[i]);
    printf("\ncmp (1=arr1>arr2): ");
    for (int i = 0; i < 4; i++) printf("%d     ", *(unsigned int*)&sse_cmp[i] ? 1 : 0);
    printf("\n\n");

    printf("=== SSE: shufps 0x1B (reverse) ===\n");
    printf("shuf result: ");
    for (int i = 0; i < 4; i++) printf("%.1f ", sse_shuf[i]);
    printf("\n\n");

    // ---------- AVX: vhaddps ----------
    float avx_arr1[8] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
    float avx_arr2[8] = { 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };
    float avx_hadd_result[8] = {};

    __m256 ymm0 = _mm256_loadu_ps(avx_arr1);
    __m256 ymm1 = _mm256_loadu_ps(avx_arr2);
    __m256 ymm_hadd = avx_hadd(ymm0, ymm1);
    _mm256_storeu_ps(avx_hadd_result, ymm_hadd);

    printf("=== AVX: vhaddps (horizontal add) ===\n");
    printf("arr1:   "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_arr1[i]);
    printf("\narr2:   "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_arr2[i]);
    printf("\nresult: "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_hadd_result[i]);
    printf("\n\n");

    // ---------- Функции с __m64, __m128, __m256 ----------
    __m64 mmx_a, mmx_b;
    mmx_a.m64_u64 = *(unsigned long long*)mmx_arr1;
    mmx_b.m64_u64 = *(unsigned long long*)mmx_arr2;
    __m64 mmx_func_res = mmx_saturating_add(mmx_a, mmx_b);
    _mm_empty();

    printf("=== Function mmx_saturating_add (__m64) ===\n");
    printf("result: ");
    unsigned char* p64 = (unsigned char*)&mmx_func_res;
    for (int i = 0; i < 8; i++) printf("%d ", p64[i]);
    printf("\n\n");

    __m128 sse_a = _mm_loadu_ps(sse_arr1);
    __m128 sse_b = _mm_loadu_ps(sse_arr2);
    __m128 sse_func_res = sse_compare_shuffle(sse_a, sse_b);
    float sse_func_out[4];
    _mm_storeu_ps(sse_func_out, sse_func_res);

    printf("=== Function sse_compare_shuffle (__m128) ===\n");
    printf("shuf masked by cmp: ");
    unsigned int* bits = (unsigned int*)sse_func_out;
    for (int i = 0; i < 4; i++) printf("0x%08X ", bits[i]);
    printf("\n  (0x0 = arr1<=arr2, non-zero = float bits from shuf where arr1>arr2)\n\n");

    __m256 avx_a = _mm256_loadu_ps(avx_arr1);
    __m256 avx_b = _mm256_loadu_ps(avx_arr2);
    __m256 avx_func_res = avx_hadd(avx_a, avx_b);
    float avx_func_out[8];
    _mm256_storeu_ps(avx_func_out, avx_func_res);

    printf("=== Function avx_hadd (__m256) ===\n");
    printf("result: ");
    for (int i = 0; i < 8; i++) printf("%.0f ", avx_func_out[i]);
    printf("\n\n");

    // ---------- Демонстрация dvec.h (пункт 4) ----------
    demo_dvec();

    printf("=== SIMD Demo Complete ===\n");
    return 0;
}