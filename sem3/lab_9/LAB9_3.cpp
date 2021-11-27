#include <dos.h>
#include <conio.h>
#include <stdio.h>

void interrupt (*old_int9)(...);
volatile unsigned char scan_code = 0;

void interrupt new_int9(...) {
    unsigned char code = inp(0x60);
    scan_code = code;
    (*old_int9)();

    outp(0x20, 0x20);
}

int main() {
    clrscr();
    printf("Press keys to see scan codes and status\n");
    printf("Press SPACE to exit\n\n");

    disable();
    old_int9 = getvect(9);
    setvect(9, new_int9);
    enable();

    while (1) {
        if (scan_code != 0) {
            unsigned char status1, status2;

            asm {
                push ds
                mov  ax, 0x40
                mov  ds, ax
                mov  si, 0x17
                mov  bx, [si]
                pop  ds
            }

            status1 = _BL;
            status2 = _BH;

            printf("Scan = %02X   Status1 = %02X  Status2 = %02X\n",
                   scan_code, status1, status2);

            scan_code = 0;
        }

        if (kbhit() && getch() == ' ') break;
    }

    disable();
    setvect(9, old_int9);
    enable();

    printf("\nRestored old interrupt handler. Exiting...\n");
    return 0;
}