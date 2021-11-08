#include <dos.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>

int main() {
    clrscr();
    outp(0x60, 0xF5);
    printf("Wait\n");
    time_t t = time(NULL);
    while (time(NULL) - t < 10) {
        delay(1000);
        printf(".");
    }
    outp(0x60, 0xF6);
    printf("\nOK\n");
    getch();
    return 0;
}
