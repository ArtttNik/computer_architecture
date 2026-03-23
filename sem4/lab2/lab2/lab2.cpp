#include <iostream>
#include <iomanip>
#include <mmintrin.h>     // MMX
#include <xmmintrin.h>    // SSE
#include <emmintrin.h>    // SSE2
#include <pmmintrin.h>    // SSE3
#include <immintrin.h>    // AVX
#include "dvec.h"         // dvec
// dvec.h подключён из твоего .h файла :contentReference[oaicite:0]{index=0}

using namespace std;

// ------------------------- ПЕЧАТЬ -------------------------

void print_m64_u8(__m64 v, const char* name)
{
    unsigned char a[8];
    *reinterpret_cast<__m64*>(a) = v;

    cout << name << ": ";
    for (int i = 0; i < 8; ++i)
        cout << (int)a[i] << ' ';
    cout << endl;
}

void print_m128_f(__m128 v, const char* name)
{
    float a[4];
    _mm_storeu_ps(a, v);

    cout << name << ": ";
    for (int i = 0; i < 4; ++i)
        cout << fixed << setprecision(2) << a[i] << ' ';
    cout << endl;
}

void print_m128_i8(__m128i v, const char* name)
{
    signed char a[16];
    _mm_storeu_si128(reinterpret_cast<__m128i*>(a), v);

    cout << name << ": ";
    for (int i = 0; i < 16; ++i)
        cout << (int)a[i] << ' ';
    cout << endl;
}

void print_m256_f(__m256 v, const char* name)
{
    float a[8];
    _mm256_storeu_ps(a, v);

    cout << name << ": ";
    for (int i = 0; i < 8; ++i)
        cout << fixed << setprecision(2) << a[i] << ' ';
    cout << endl;
}

void print_u16x8(const unsigned short* a, const char* name)
{
    cout << name << ": ";
    for (int i = 0; i < 8; ++i)
        cout << a[i] << ' ';
    cout << endl;
}

// ------------------------- ПУНКТ 2 -------------------------
// 3 функции с __m64, __m128, __m256

__m64 mmx_sat_add_u8(__m64 a, __m64 b)
{
    return _mm_adds_pu8(a, b);   // насыщение
}

__m128 sse3_hadd_ps_demo(__m128 a, __m128 b)
{
    return _mm_hadd_ps(a, b);    // горизонтальная команда
}

__m256 avx_add_ps_demo(__m256 a, __m256 b)
{
    return _mm256_add_ps(a, b);  // AVX YMM
}

// ------------------------- ПУНКТ 1 -------------------------

void demo_mmx_sse_avx()
{
    cout << "===== ПУНКТ 1. MMX-SSE-AVX =====" << endl;

    // MMX: насыщение
    unsigned char mmxA_arr[8] = { 250, 251, 252, 253, 254, 255, 100, 200 };
    unsigned char mmxB_arr[8] = { 10, 20, 30, 40, 50, 60, 200, 100 };

    __m64 mmxA = *reinterpret_cast<__m64*>(mmxA_arr);
    __m64 mmxB = *reinterpret_cast<__m64*>(mmxB_arr);
    __m64 mmxR = mmx_sat_add_u8(mmxA, mmxB);

    print_m64_u8(mmxA, "MMX A");
    print_m64_u8(mmxB, "MMX B");
    print_m64_u8(mmxR, "MMX sat_add");

    _mm_empty();

    cout << endl;

    // SSE2: сравнение
    signed char sseA_arr[16] = { 5, 10, 15, 20, 25, 30, 35, 40, -1, -2, -3, -4, 50, 60, 70, 80 };
    signed char sseB_arr[16] = { 1, 20, 10, 20, 30, 10, 40, 35, -2, -2, -1, -5, 45, 65, 60, 90 };

    __m128i sseA = _mm_loadu_si128(reinterpret_cast<const __m128i*>(sseA_arr));
    __m128i sseB = _mm_loadu_si128(reinterpret_cast<const __m128i*>(sseB_arr));
    __m128i sseCmp = _mm_cmpgt_epi8(sseA, sseB);

    print_m128_i8(sseA, "SSE A");
    print_m128_i8(sseB, "SSE B");
    print_m128_i8(sseCmp, "SSE cmpgt_epi8");

    cout << endl;

    // SSE3: горизонтальное сложение
    float f1[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    float f2[4] = { 10.0f, 20.0f, 30.0f, 40.0f };

    __m128 x1 = _mm_loadu_ps(f1);
    __m128 x2 = _mm_loadu_ps(f2);
    __m128 xh = sse3_hadd_ps_demo(x1, x2);

    print_m128_f(x1, "SSE3 A");
    print_m128_f(x2, "SSE3 B");
    print_m128_f(xh, "SSE3 hadd");

    cout << endl;

    // AVX: работа с YMM и перестановка
    float avxA_arr[8] = { 1,2,3,4,5,6,7,8 };
    float avxB_arr[8] = { 8,7,6,5,4,3,2,1 };

    __m256 avxA = _mm256_loadu_ps(avxA_arr);
    __m256 avxB = _mm256_loadu_ps(avxB_arr);
    __m256 avxAdd = avx_add_ps_demo(avxA, avxB);
    __m256 avxPerm = _mm256_permute2f128_ps(avxA, avxB, 0x21); // перестановка 128-битных половин

    print_m256_f(avxA, "AVX A");
    print_m256_f(avxB, "AVX B");
    print_m256_f(avxAdd, "AVX add");
    print_m256_f(avxPerm, "AVX permute2f128");

    cout << endl;
}

// ------------------------- ПУНКТ 2 -------------------------

void demo_three_functions()
{
    cout << "===== ПУНКТ 2. ФУНКЦИИ __m64, __m128, __m256 =====" << endl;

    // __m64 над char
    unsigned char a1[8] = { 250, 240, 100, 50, 255, 1, 2, 3 };
    unsigned char b1[8] = { 20, 30, 200, 100, 10, 250, 10, 20 };

    __m64 m1 = *reinterpret_cast<__m64*>(a1);
    __m64 m2 = *reinterpret_cast<__m64*>(b1);
    __m64 mr = mmx_sat_add_u8(m1, m2);

    print_m64_u8(m1, "Func __m64 A");
    print_m64_u8(m2, "Func __m64 B");
    print_m64_u8(mr, "Func __m64 Result");

    _mm_empty();

    cout << endl;

    // __m128 над float
    float a2[4] = { 2.0f, 4.0f, 6.0f, 8.0f };
    float b2[4] = { 1.0f, 3.0f, 5.0f, 7.0f };

    __m128 s1 = _mm_loadu_ps(a2);
    __m128 s2 = _mm_loadu_ps(b2);
    __m128 sr = sse3_hadd_ps_demo(s1, s2);

    print_m128_f(s1, "Func __m128 A");
    print_m128_f(s2, "Func __m128 B");
    print_m128_f(sr, "Func __m128 Result");

    cout << endl;

    // __m256 над float
    float a3[8] = { 1,2,3,4,5,6,7,8 };
    float b3[8] = { 10,20,30,40,50,60,70,80 };

    __m256 y1 = _mm256_loadu_ps(a3);
    __m256 y2 = _mm256_loadu_ps(b3);
    __m256 yr = avx_add_ps_demo(y1, y2);

    print_m256_f(y1, "Func __m256 A");
    print_m256_f(y2, "Func __m256 B");
    print_m256_f(yr, "Func __m256 Result");

    cout << endl;
}

// ------------------------- ПУНКТ 3 -------------------------
// dvec.h

void demo_dvec()
{
    cout << "===== ПУНКТ 3. DVEC =====" << endl;

    // 8 unsigned short
    Iu16vec8 a(1000, 2000, 3000, 4000, 5000, 6000, 65000, 65530);
    Iu16vec8 b(100, 200, 300, 400, 500, 600, 1000, 1000);

    Iu16vec8 sum = a + b;            // обычное сложение
    Iu16vec8 sat = sat_add(a, b);    // сложение с насыщением
    Iu16vec8 avg = simd_avg(a, b);   // среднее
    Iu16vec8 lo = unpack_low(a, b);  // распаковка младших элементов

    alignas(16) unsigned short outSum[8];
    alignas(16) unsigned short outSat[8];
    alignas(16) unsigned short outAvg[8];
    alignas(16) unsigned short outLo[8];

    _mm_storeu_si128(reinterpret_cast<__m128i*>(outSum), (__m128i)sum);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(outSat), (__m128i)sat);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(outAvg), (__m128i)avg);
    _mm_storeu_si128(reinterpret_cast<__m128i*>(outLo), (__m128i)lo);

    print_u16x8(outSum, "dvec sum");
    print_u16x8(outSat, "dvec sat_add");
    print_u16x8(outAvg, "dvec avg");
    print_u16x8(outLo, "dvec unpack_low");

    cout << endl;
}

// ------------------------- MAIN -------------------------

int main()
{
    setlocale(LC_ALL, "Russian");

    demo_mmx_sse_avx();
    demo_three_functions();
    demo_dvec();

    return 0;
}