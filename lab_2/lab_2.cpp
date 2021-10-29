#include <iostream>

int main() {
    
    std::cout << "TASK 1:" << std::endl;
    int a = 5, b = 10, c = 0;
    int result = 0, flag = 0;
    __asm {
        mov eax, a
        add eax, b // 03 45 E8
        sub eax, 2 // 83 E8 02

        mov result, eax

        add ebx, eax // 03 D8

        mov ebx, a
        mov ecx, b
        mov [c], ebx
        add ecx, [c]

        mov result, ecx

        mov eax, a
        or eax, b // 0B 45 E8
        mov result, eax

        mov ecx, a
        shl ecx, 2 // C1 E1 02
        mov result, ecx

        mov eax, a
        cmp eax, b
        je equal // 74 09
        mov flag, 0
        jmp done

        equal :
        mov flag, 1

        done :
        mov result, eax
    };

    std::cout << "result = " << result << ", flag = " << flag << std::endl;

    std::cout << std::endl << "TASK 2 AND 3:" << std::endl;
    const int N = 5;
    int B[N] = { 1, 2, 3, 4, 5 };
    int C[N] = { 0, 0, 0, 0, 0 };

    __asm {
        lea esi, B
        lea edi, C
        mov ecx, N

        copy :
            mov eax, [esi]
            mov ebx, [edi]
            mov[esi], ebx
            mov[edi], eax

            add esi, 4
            add edi, 4

            loop copy
    }

    std::cout << "C: ";
    for (int i : C) {
        std::cout << i << " ";
    }
    std::cout << std::endl << "B: ";
    for (int i : B) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl << "TASK 4:" << std::endl;

    int E[N] = { 1, 2, 3, 4, 5 };
    int G[N] = { 0, 0, 0, 0, 0 };

    __asm {
        lea esi, E
        lea edi, G
        mov ecx, N

        cld
        rep movsd
    }

    std::cout << "Array E: ";
    for (int i : E) {
        std::cout << i << " ";
    }

    std::cout << std::endl;
    std::cout << "Array G: ";
    for (int i : G) {
        std::cout << i << " ";
    }
    std::cout << std::endl;


    std::cout << std::endl << "TASK 5:" << std::endl;


    //глянуть в for () {}

    /*0015299D 8D 85 E8 FE FF FF    lea         eax,[G]  
001529A3 89 85 AC FE FF FF    mov         dword ptr [ebp-154h],eax  
001529A9 8B 85 AC FE FF FF    mov         eax,dword ptr [ebp-154h]  
001529AF 89 85 A0 FE FF FF    mov         dword ptr [ebp-160h],eax  
001529B5 8B 85 AC FE FF FF    mov         eax,dword ptr [ebp-154h]  
001529BB 83 C0 14             add         eax,14h  
001529BE 89 85 94 FE FF FF    mov         dword ptr [ebp-16Ch],eax  
001529C4 EB 0F                jmp         copy+30Eh (01529D5h)  
001529C6 8B 85 A0 FE FF FF    mov         eax,dword ptr [ebp-160h]  
001529CC 83 C0 04             add         eax,4  
001529CF 89 85 A0 FE FF FF    mov         dword ptr [ebp-160h],eax  
001529D5 8B 85 A0 FE FF FF    mov         eax,dword ptr [ebp-160h]  
001529DB 3B 85 94 FE FF FF    cmp         eax,dword ptr [ebp-16Ch]  
001529E1 74 3A                je          copy+356h (0152A1Dh)  
001529E3 8B 85 A0 FE FF FF    mov         eax,dword ptr [ebp-160h]  
001529E9 8B 08                mov         ecx,dword ptr [eax]  
001529EB 89 8D 88 FE FF FF    mov         dword ptr [ebp-178h],ecx  */

    return 0;
}