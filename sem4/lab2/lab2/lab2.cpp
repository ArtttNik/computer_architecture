/** @file sample_code.cpp
 *  @brief ММХ-SSE-SSE2.
 *  @details This is a sample teacher's code for learning MMX-SSE-SSE2 instructions tweaked by me.
 *  @author Baranov Konstantin (seigtm) <gh@seig.ru>
 *  @version 1.0
 *  @date 2024-03-05
 */

#include <iostream>

int main() {
    // 1. Using MMX instructions to compare elements of qw1 and qw2.
    char qw1[8]{ 1, 0, 1, 1, 1, 1, 0, 1 };
    char qw2[8]{ 1, 2, 2, 1, 1, 2, 2, 1 };

    std::cout << "qw1 = ";
    for (int i{}; i < 8; ++i)
        std::cout << +qw1[i] << ' ';
    std::cout << "\nqw2 = ";
    for (int i{}; i < 8; ++i)
        std::cout << +qw2[i] << ' ';

    std::cout << "\nComparing qw1 and qw2 by using MMX instructions (0 = not equal, -1 = equal):\n";

    __asm {
        movq mm0, qword ptr[qw1]
        movq mm1, qword ptr[qw2]
        pcmpeqb mm0, mm1
        movq qword ptr[qw1], mm0
        emms  // Важно: очистка состояния MMX
    }

    std::cout << "qw1 = ";
    for (int i{}; i < 8; ++i)
        std::cout << +qw1[i] << ' ';


    // 2. Using SSE instructions to add elements of c and d.
    float c[4]{ 1.0f, 2.0f, 3.0f, 4.0f };
    float d[4]{ 5.0f, 6.0f, 7.0f, 8.0f };

    std::cout << "\n\nc = ";
    for (int i{}; i < 4; ++i)
        std::cout << c[i] << " ";
    std::cout << "\nd = ";
    for (int i{}; i < 4; ++i)
        std::cout << d[i] << " ";

    std::cout << "\nSumming elements of vectors c + d by using SSE instructions:\n";

    __asm {
        movups xmm0, xmmword ptr[c]
        movups xmm1, xmmword ptr[d]
        addps xmm0, xmm1
        movups xmmword ptr[c], xmm0
    }

    std::cout << "c = ";
    for (int i{}; i < 4; ++i)
        std::cout << c[i] << ' ';


    // 3. Using SSE2 instructions to compute square root of elements in f.
    double f[2]{ 16.0, 4.0 };
    std::cout << "\n\nf = " << f[0] << " " << f[1];

    std::cout << "\nComputing square root of elements in f by using SSE2 instructions:\n";

    __asm {
        movupd xmm1, xmmword ptr[f]
        sqrtpd xmm0, xmm1
        movupd xmmword ptr[f], xmm0
    }

    std::cout << "Square root of f elements: " << f[0] << " " << f[1];


    // 4. Using SSE2 instructions to find the minimum of elements in a128 and b128.
    char a128[16]{ 1, 18, 3, 19, 5, 21, 7, 23, 9, 25, 11, 27, 13, 29, 15, 31 };
    char b128[16]{ 17, 2, 19, 4, 21, 6, 23, 8, 25, 10, 27, 12, 29, 14, 31, 16 };

    std::cout << "\n\na128 = ";
    for (int i{}; i < 16; ++i)
        std::cout << +a128[i] << ' ';
    std::cout << "\nb128 = ";
    for (int i{}; i < 16; ++i)
        std::cout << +b128[i] << ' ';

    std::cout << "\nFinding minimum of elements in a128 and b128 by using SSE2 instructions:\n";

    __asm {
        movdqu xmm0, xmmword ptr[a128]
        movdqu xmm1, xmmword ptr[b128]
        pminub xmm0, xmm1
        movdqu xmmword ptr[a128], xmm0
    }

    std::cout << "Minimum elements: a128 = ";
    for (int i{}; i < 16; ++i)
        std::cout << +a128[i] << ' ';

    std::cout << '\n';

    return 0;
}