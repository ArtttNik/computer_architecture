#include <iostream>
#include <iomanip>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <immintrin.h>
#include "dvec.h"

using namespace std;

// ------------------------- ПЕЧАТЬ -------------------------

void print_m64_u8(__m64 v, const char* name)
{
    unsigned char a[8];
    *reinterpret_cast<__m64*>(a) = v;
    cout << name << ": ";
    for (int i = 0; i < 8; ++i) cout << (int)a[i] << ' ';
    cout << endl;
}

void print_m128_f(__m128 v, const char* name)
{
    float a[4];
    _mm_storeu_ps(a, v);
    cout << name << ": ";
    for (int i = 0; i < 4; ++i) cout << fixed << setprecision(2) << a[i] << ' ';
    cout << endl;
}

void print_m128_i8(__m128i v, const char* name)
{
    signed char a[16];
    _mm_storeu_si128(reinterpret_cast<__m128i*>(a), v);
    cout << name << ": ";
    for (int i = 0; i < 16; ++i) cout << (int)a[i] << ' ';
    cout << endl;
}

void print_m256_f(__m256 v, const char* name)
{
    float a[8];
    _mm256_storeu_ps(a, v);
    cout << name << ": ";
    for (int i = 0; i < 8; ++i) cout << fixed << setprecision(2) << a[i] << ' ';
    cout << endl;
}

void print_u16x8(const unsigned short* a, const char* name)
{
    cout << name << ": ";
    for (int i = 0; i < 8; ++i) cout << a[i] << ' ';
    cout << endl;
}

// ------------------------- ФУНКЦИИ ПУНКТ 2 -------------------------

__m64 mmx_sat_add_u8(__m64 a, __m64 b)
{
    // Ассемблерная вставка: беззнаковое сложение с насыщением (paddusb)
    __m64 result;
    __asm {
        movq  mm0, a
        movq  mm1, b
        paddusb mm0, mm1      // насыщение: результат не превысит 255
        movq  result, mm0
        emms
    }
    return result;
}

__m128 sse3_hadd_ps_demo(__m128 a, __m128 b)
{
    // Ассемблерная вставка: горизонтальное сложение (SSE3 haddps)
    __m128 result;
    __asm {
        movups xmm0, a
        movups xmm1, b
        haddps xmm0, xmm1     // горизонт.: xmm0[0]=a[0]+a[1], xmm0[1]=a[2]+a[3],
        //            xmm0[2]=b[0]+b[1], xmm0[3]=b[2]+b[3]
        movups result, xmm0
    }
    return result;
}

__m256 avx_add_ps_demo(__m256 a, __m256 b)
{
    // Для AVX inline asm в MSVC x64 недоступен — используем intrinsic
    return _mm256_add_ps(a, b);
}

// ------------------------- ПУНКТ 1 -------------------------

void demo_mmx_sse_avx()
{
    cout << "===== ПУНКТ 1. MMX-SSE-AVX =====" << endl;

    // --- MMX: насыщение paddusb ---
    unsigned char mmxA_arr[8] = { 250, 251, 252, 253, 254, 255, 100, 200 };
    unsigned char mmxB_arr[8] = { 10,  20,  30,  40,  50,  60, 200, 100 };
    unsigned char mmxR_arr[8] = {};

    __asm {
        movq  mm0, mmxA_arr
        movq  mm1, mmxB_arr
        paddusb mm0, mm1          // беззнаковое сложение с насыщением
        movq  mmxR_arr, mm0
        emms
    }

    cout << "MMX paddusb (насыщение):" << endl;
    cout << "A: "; for (int i = 0; i < 8; i++) cout << (int)mmxA_arr[i] << ' '; cout << endl;
    cout << "B: "; for (int i = 0; i < 8; i++) cout << (int)mmxB_arr[i] << ' '; cout << endl;
    cout << "R: "; for (int i = 0; i < 8; i++) cout << (int)mmxR_arr[i] << ' '; cout << endl;
    cout << endl;

    // --- SSE2: сравнение pcmpgtb ---
    signed char sseA_arr[16] = { 5,10,15,20,25,30,35,40,-1,-2,-3,-4,50,60,70,80 };
    signed char sseB_arr[16] = { 1,20,10,20,30,10,40,35,-2,-2,-1,-5,45,65,60,90 };
    signed char sseCmp_arr[16] = {};

    __asm {
        movups xmm0, sseA_arr
        movups xmm1, sseB_arr
        pcmpgtb xmm0, xmm1        // сравнение: 0xFF если A[i]>B[i], иначе 0x00
        movups sseCmp_arr, xmm0
    }

    cout << "SSE2 pcmpgtb (сравнение побайтово A > B):" << endl;
    cout << "A:   "; for (int i = 0; i < 16; i++) cout << (int)sseA_arr[i] << ' '; cout << endl;
    cout << "B:   "; for (int i = 0; i < 16; i++) cout << (int)sseB_arr[i] << ' '; cout << endl;
    cout << "Cmp: "; for (int i = 0; i < 16; i++) cout << (int)(unsigned char)sseCmp_arr[i] << ' '; cout << endl;
    cout << endl;

    // --- SSE3: горизонтальное сложение haddps ---
    float f1[4] = { 1.0f, 2.0f,  3.0f,  4.0f };
    float f2[4] = { 10.0f, 20.0f, 30.0f, 40.0f };
    float fh[4] = {};

    __asm {
        movups xmm0, f1
        movups xmm1, f2
        haddps xmm0, xmm1         // горизонт. сложение: [a0+a1, a2+a3, b0+b1, b2+b3]
        movups fh, xmm0
    }

    cout << "SSE3 haddps (горизонтальное сложение):" << endl;
    cout << "A: "; for (int i = 0; i < 4; i++) cout << f1[i] << ' '; cout << endl;
    cout << "B: "; for (int i = 0; i < 4; i++) cout << f2[i] << ' '; cout << endl;
    cout << "R: "; for (int i = 0; i < 4; i++) cout << fh[i] << ' '; cout << endl;
    cout << endl;

    // --- AVX: сложение и перестановка ymm (intrinsic — MSVC x64 не поддерживает asm для AVX) ---
    float avxA_arr[8] = { 1,2,3,4,5,6,7,8 };
    float avxB_arr[8] = { 8,7,6,5,4,3,2,1 };

    __m256 avxA = _mm256_loadu_ps(avxA_arr);
    __m256 avxB = _mm256_loadu_ps(avxB_arr);
    __m256 avxAdd = _mm256_add_ps(avxA, avxB);
    __m256 avxPerm = _mm256_permute2f128_ps(avxA, avxB, 0x21); // перестановка 128-битных половин

    print_m256_f(avxA, "AVX A");
    print_m256_f(avxB, "AVX B");
    print_m256_f(avxAdd, "AVX vaddps");
    print_m256_f(avxPerm, "AVX vperm2f128(0x21)");
    cout << endl;
}

// ------------------------- ПУНКТ 2 -------------------------

void demo_three_functions()
{
    cout << "===== ПУНКТ 2. ФУНКЦИИ __m64, __m128, __m256 =====" << endl;

    // __m64 — paddusb
    unsigned char a1[8] = { 250, 240, 100, 50, 255,   1,  2,  3 };
    unsigned char b1[8] = { 20,  30, 200, 100,  10, 250, 10, 20 };
    __m64 m1 = *reinterpret_cast<__m64*>(a1);
    __m64 m2 = *reinterpret_cast<__m64*>(b1);
    __m64 mr = mmx_sat_add_u8(m1, m2);   // внутри — asm paddusb
    print_m64_u8(m1, "Func __m64 A");
    print_m64_u8(m2, "Func __m64 B");
    print_m64_u8(mr, "Func __m64 paddusb");
    cout << endl;

    // __m128 — haddps
    float a2[4] = { 2.0f, 4.0f, 6.0f, 8.0f };
    float b2[4] = { 1.0f, 3.0f, 5.0f, 7.0f };
    __m128 s1 = _mm_loadu_ps(a2);
    __m128 s2 = _mm_loadu_ps(b2);
    __m128 sr = sse3_hadd_ps_demo(s1, s2);  // внутри — asm haddps
    print_m128_f(s1, "Func __m128 A");
    print_m128_f(s2, "Func __m128 B");
    print_m128_f(sr, "Func __m128 haddps");
    cout << endl;

    // __m256 — vaddps (intrinsic)
    float a3[8] = { 1,2,3,4,5,6,7,8 };
    float b3[8] = { 10,20,30,40,50,60,70,80 };
    __m256 y1 = _mm256_loadu_ps(a3);
    __m256 y2 = _mm256_loadu_ps(b3);
    __m256 yr = avx_add_ps_demo(y1, y2);
    print_m256_f(y1, "Func __m256 A");
    print_m256_f(y2, "Func __m256 B");
    print_m256_f(yr, "Func __m256 vaddps");
    cout << endl;
}

// ------------------------- ПУНКТ 3 -------------------------

void demo_dvec()
{
    cout << "===== ПУНКТ 3. DVEC =====" << endl;

    Iu16vec8 a(1000, 2000, 3000, 4000, 5000, 6000, 65000, 65530);
    Iu16vec8 b(100, 200, 300, 400, 500, 600, 1000, 1000);

    Iu16vec8 sum = a + b;
    Iu16vec8 sat = sat_add(a, b);
    Iu16vec8 avg = simd_avg(a, b);
    Iu16vec8 lo = unpack_low(a, b);

    alignas(16) unsigned short outSum[8], outSat[8], outAvg[8], outLo[8];
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