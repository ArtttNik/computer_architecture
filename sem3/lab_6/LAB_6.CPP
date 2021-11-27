#include <stdio.h>
#include <dos.h>
#include <iostream.h>
#include <conio.h>


long long A = 125;
long long B = 0;


void interrupt(*old)(...);

void interrupt handler1(...)
{
	cout << "int 1" << endl;
	asm {
		lea si, A
		lea di, B

		mov ax, [si]
		mov bx, [di]
		mov [di], ax
		mov [si], bx
	}
}


void interrupt handler2(...)
{
	cout << "int 2" << endl;
	asm {
		lea di, B
		mov ax, [di]
		inc ax
		mov [di], ax
	}
	asm {
		mov ax, [bp+18]
		add ax, 2
		mov [bp+18], ax
	}
}


void main(void)
{
	clrscr();

	int usual = 0xf0;
	old = getvect(usual);
	disable();
	setvect(usual, handler1);
	enable();
	geninterrupt(usual);
	cout << "A = " << A << ", B = " << B << endl;
	setvect(usual, old);
	getch();

	int special = 0;
	old = getvect(special);
	setvect(special, handler2);
	int a = 1;
	a / 0;
	setvect(special, old);
	cout << A << ' ' << B << '\n';
	getch();
}