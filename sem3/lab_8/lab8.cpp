#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <iostream.h>

void interrupt(*old_handler)(...);
void interrupt(*old_int3)(...);

int a = 1;

void interrupt step_handler(...)
{
    unsigned ax_value;

    asm {
        push ax
        mov ax_value, ax
        pop ax
    }

    cout << "DEBUG: AX = " << hex << ax_value << dec << ", a = " << a << endl;
    cout << "Press any key to continue (q to quit)...";
    char c = getch();
    cout << endl;

    if (c == 'q') {
        disable();
        setvect(1, old_handler);
        enable();
        cout << "Debugging stopped" << endl;

        asm {
            push bp
            mov bp, sp
            and word ptr [bp + 12], 0FEFFh
            pop bp
        }
    }
}


void interrupt breakpoint_handler(...)
{
    cout << "BREAKPOINT HIT! AX = ";
    unsigned ax_value;
    asm {
        mov ax, [bp + 8]
        mov ax_value, ax
    }
    cout << hex << ax_value << dec << endl;
    cout << "Press any key to continue...";
    getch();
    cout << endl;
}

void debugged_program()
{
    asm {
        mov ax, 1000h
        add ax, 2000h
        sub ax, 500h
        mov bx, ax
        inc ax
        int 3
        dec ax
    }

    for(int i = 0; i < 3; i++) {
        a *= 2;
    }
    cout << "Program completed: a = " << a << endl;
}

void main(void)
{
    clrscr();
    cout << "Starting debugger..." << endl;

    old_int3 = getvect(3);
    disable();
    setvect(3, breakpoint_handler);
    enable();

    old_handler = getvect(1);
    disable();
    setvect(1, step_handler);
    enable();

    cout << "Enabling single-step mode..." << endl;
    asm {
        pushf
        pop ax
        or ax, 100h
        push ax
        popf
    }

    debugged_program();

    disable();
    setvect(1, old_handler);
    setvect(3, old_int3);
    enable();

    cout << "End of program" << endl;
    getch();
}