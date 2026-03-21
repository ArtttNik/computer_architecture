#include <cstdio>
#include <mmintrin.h>   // MMX   (__m64)
#include <xmmintrin.h>  // SSE   (__m128)
#include <emmintrin.h>  // SSE2
#include <immintrin.h>  // AVX   (__m256)

// ---------------------------------------------------------------------------
// 1. MMX функция (__m64)
//    paddusb — сложение байт с насыщением без знака (особенная команда)
//    200 + 100 = 255 (не 300), 255 + 1 = 255 — наглядный эффект насыщения
// ---------------------------------------------------------------------------
__m64 mmx_saturating_add(__m64 a, __m64 b) {
    return _mm_adds_pu8(a, b);  // paddusb
}

// ---------------------------------------------------------------------------
// 2. SSE функция (__m128)
//    cmpps  — сравнение упакованных float (особенная команда)
//    shufps — перестановка элементов      (особенная команда)
//    andps  — маскирование: оставляем shuf только там где a > b
// ---------------------------------------------------------------------------
__m128 sse_compare_shuffle(__m128 a, __m128 b) {
    __m128 cmp = _mm_cmpgt_ps(a, b);          // cmpps: (a > b) ? 0xFFFFFFFF : 0
    __m128 shuf = _mm_shuffle_ps(a, b, 0x1B);  // shufps: разворот [b1,b0,a1,a0]
    return _mm_and_ps(shuf, cmp);               // andps: маскирование
}

// ---------------------------------------------------------------------------
// 3. AVX функция (__m256)
//    vhaddps — горизонтальное сложение пар float (особенная AVX-команда)
//    Результат: [a0+a1, a2+a3, b0+b1, b2+b3, a4+a5, a6+a7, b4+b5, b6+b7]
// ---------------------------------------------------------------------------
__m256 avx_hadd(__m256 a, __m256 b) {
    return _mm256_hadd_ps(a, b);  // vhaddps
}

int main() {

    // -----------------------------------------------------------------------
    // === MMX ДЕМОНСТРАЦИЯ (inline asm) ===
    // paddusb: насыщение — 200+100=255 (не 300), 255+1=255
    // -----------------------------------------------------------------------
    unsigned char mmx_arr1[8] = { 200, 100, 150,  50, 255, 100, 200,  50 };
    unsigned char mmx_arr2[8] = { 100, 200, 150, 250,   1, 200, 100, 250 };
    unsigned char mmx_result[8] = {};

    __asm {
        movq    mm0, mmx_arr1   // MM0 = mmx_arr1
        movq    mm1, mmx_arr2   // MM1 = mmx_arr2
        paddusb mm0, mm1        // НАСЫЩЕНИЕ: MM0 = saturate(MM0 + MM1)
        movq    mmx_result, mm0 // сохраняем результат
        emms                    // очищаем MMX-состояние
    }

    printf("=== MMX: paddusb (saturating add unsigned bytes) ===\n");
    printf("arr1:   "); for (int i = 0; i < 8; i++) printf("%d ", mmx_arr1[i]);
    printf("\narr2:   "); for (int i = 0; i < 8; i++) printf("%d ", mmx_arr2[i]);
    printf("\nresult: "); for (int i = 0; i < 8; i++) printf("%d ", mmx_result[i]);
    printf("\n\n");

    // -----------------------------------------------------------------------
    // === SSE ДЕМОНСТРАЦИЯ (inline asm) ===
    // cmpps предикат 6 = nle = not-less-or-equal = строго больше
    // shufps 0x1B = 00|01|10|11 => разворот порядка элементов
    // -----------------------------------------------------------------------
    float sse_arr1[4] = { 1.5f, 3.0f, 2.0f, 4.5f };
    float sse_arr2[4] = { 2.0f, 1.0f, 4.0f, 3.0f };
    float sse_cmp[4] = {};
    float sse_shuf[4] = {};

    __asm {
        movups xmm0, sse_arr1
        movups xmm1, sse_arr2

        movaps xmm2, xmm0
        cmpps  xmm2, xmm1, 6    // СРАВНЕНИЕ: xmm2[i] = (xmm0[i] > xmm1[i]) ? 0xFFFFFFFF : 0
        movups sse_cmp, xmm2

        movups xmm0, sse_arr1
        shufps xmm0, xmm1, 0x1B // ПЕРЕСТАНОВКА: 0x1B = 00 01 10 11
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

    // -----------------------------------------------------------------------
    // === AVX ДЕМОНСТРАЦИЯ (интринсики) ===
    // vaddps  — 8 сложений float за одну инструкцию (256-бит YMM)
    // vhaddps — горизонтальное сложение соседних пар
    // -----------------------------------------------------------------------
    float avx_arr1[8] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
    float avx_arr2[8] = { 8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f };
    float avx_add_result[8] = {};
    float avx_hadd_result[8] = {};

    __m256 ymm0 = _mm256_loadu_ps(avx_arr1);
    __m256 ymm1 = _mm256_loadu_ps(avx_arr2);
    __m256 ymm_add = _mm256_add_ps(ymm0, ymm1);   // vaddps
    __m256 ymm_hadd = avx_hadd(ymm0, ymm1);         // vhaddps

    _mm256_storeu_ps(avx_add_result, ymm_add);
    _mm256_storeu_ps(avx_hadd_result, ymm_hadd);

    printf("=== AVX: vaddps (8 x float in YMM) ===\n");
    printf("arr1:   "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_arr1[i]);
    printf("\narr2:   "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_arr2[i]);
    printf("\nresult: "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_add_result[i]);
    printf("\n\n");

    printf("=== AVX: vhaddps (horizontal add) ===\n");
    printf("format: [a0+a1, a2+a3, b0+b1, b2+b3, a4+a5, a6+a7, b4+b5, b6+b7]\n");
    printf("result: "); for (int i = 0; i < 8; i++) printf("%.0f ", avx_hadd_result[i]);
    printf("\n\n");

    // -----------------------------------------------------------------------
    // === ВЫЗОВ ТРЁХ ФУНКЦИЙ (задание п.2) ===
    // -----------------------------------------------------------------------

    // 1. __m64 — MMX: сложение с насыщением
    __m64 mmx_a, mmx_b;
    mmx_a.m64_u64 = *(unsigned long long*)mmx_arr1;
    mmx_b.m64_u64 = *(unsigned long long*)mmx_arr2;
    __m64 mmx_func_res = mmx_saturating_add(mmx_a, mmx_b);
    _mm_empty();  // emms

    printf("=== Function mmx_saturating_add (__m64) ===\n");
    printf("result: ");
    unsigned char* p64 = (unsigned char*)&mmx_func_res;
    for (int i = 0; i < 8; i++) printf("%d ", p64[i]);
    printf("\n\n");

    // 2. __m128 — SSE: сравнение + перестановка + маскирование
    __m128 sse_a = _mm_loadu_ps(sse_arr1);
    __m128 sse_b = _mm_loadu_ps(sse_arr2);
    __m128 sse_func_res = sse_compare_shuffle(sse_a, sse_b);
    float sse_func_out[4];
    _mm_storeu_ps(sse_func_out, sse_func_res);

    // Результат — битовая маска, выводим как hex (избегаем NaN в printf/cout)
    printf("=== Function sse_compare_shuffle (__m128) ===\n");
    printf("shuf masked by cmp: ");
    unsigned int* bits = (unsigned int*)sse_func_out;
    for (int i = 0; i < 4; i++) printf("0x%08X ", bits[i]);
    printf("\n  (0x0 = arr1<=arr2, non-zero = float bits from shuf where arr1>arr2)\n\n");

    // 3. __m256 — AVX: горизонтальное сложение
    __m256 avx_a = _mm256_loadu_ps(avx_arr1);
    __m256 avx_b = _mm256_loadu_ps(avx_arr2);
    __m256 avx_func_res = avx_hadd(avx_a, avx_b);
    float avx_func_out[8];
    _mm256_storeu_ps(avx_func_out, avx_func_res);

    printf("=== Function avx_hadd (__m256) ===\n");
    printf("result: ");
    for (int i = 0; i < 8; i++) printf("%.0f ", avx_func_out[i]);
    printf("\n\n");

    printf("=== SIMD Demo Complete ===\n");
    return 0;
}