#include <dos.h>
#include <conio.h>
#include <stdio.h>

int main() {
    unsigned char delay, rate;
    clrscr();
    printf("Delay(0-3): ");
    delay = getch() - '0';
    printf("%c\nRate(0-31): ", delay + '0');
    rate = getch() - '0';
    printf("%c\nSet\n", rate + '0');

    unsigned char value = ((delay & 0x03) << 5) | (rate & 0x1F);
    outp(0x60, 0xF3);
    outp(0x60, value);

    printf("OK\n");
    getch();
    return 0;
}
