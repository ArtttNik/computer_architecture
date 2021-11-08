#include <iostream.h>
#include <stdio.h>

#include <conio.h>
#include <dos.h>
#include <bios.h>

void main()
{
    clrscr();
    cout << "press keys to get codes\npress space for end\n";
    char scan_code, asci_code, status;
    int adress;
    do
    {
        asm {ы
            push ds
            push si
            push di
            mov ax, 0x40
            mov ds, ax
            mov si, 0x1A
            mov di, 0x1C
        }
    wait_kbd:
        asm {
            mov ax, [si]
            cmp ax, [di]
            je wait_kbd

            mov si, [si]
        }
        adress = _SI;

        asm mov ax, [si]

        asm mov si, 0x17
        asm mov bx, [si]

        asm pop di
        asm pop si
        asm pop ds

        scan_code = _AH;
        asci_code = _AL;
        status = _BL;

        printf("adress = %x scan code = %d ASCII = %d\n", adress, scan_code, asci_code);
        printf("status %x\n", status);
    } while(getch() != ' ');

    cout << "Press any key...\n";
    getch();
}