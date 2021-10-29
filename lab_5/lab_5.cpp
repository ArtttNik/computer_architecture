#include <iostream>
using namespace std;

unsigned int rotate(unsigned int value, unsigned int count, int dir) {
    unsigned int result = 0;
    int zf = 0;
    int cf = 0;

    __asm {
        mov ecx, count
        mov eax, value
        cmp dir, 0
        jne rotate_right

        rotate_left :
        rol eax, cl
            jmp done

            rotate_right :
        ror eax, cl

            done :
        mov result, eax

            test eax, eax
            

         
    }

    
    return result;
}

int main() {
    unsigned int x = 0x91;

    cout << hex;
    cout << x << endl;

    unsigned int res1 = rotate(x, 1, 0);
    cout <<  res1 << endl;

    unsigned int res2 = rotate(x, 1, 1);
    cout << res2 << endl;

    return 0;
}
