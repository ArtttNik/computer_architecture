#include <stdio.h>
#include <dos.h>
#include <mem.h>
#include <conio.h>

struct ProcStats
{
    unsigned int rax, rbx, rcx, rdx;
    unsigned int rsi, rdi, rbp, rsp;
    unsigned int rcs, rds, res, rss;
    unsigned int rip, rflags, a, b;
    int priority;
} Stats[3];

unsigned int current_proc;
unsigned int stats_offset;
void interrupt (*oldHandler)(...);
volatile int switch_flag = 0;
int priorities[3] = {2, 1, 3};

void interrupt IntHandler(...)
{
    asm {
        in al, 60h
        cmp al, 57
        jne no_switch
        mov [switch_flag], 1
    no_switch:
    }
    
    oldHandler();
    
    if(switch_flag) {
        switch_flag = 0;
        
        // Сохраняем контекст текущей задачи
        asm {
            mov si, [current_proc]
            mov cl, 5
            shl si, cl
            mov ax, [stats_offset]
            add si, ax
            pop ax
            mov [si+24], ax  // ip
            pop ax
            mov [si+16], ax  // cs
            pop ax
            mov [si+26], ax  // flags
            mov [si+0], ax   // ax
            mov [si+2], bx   // bx
            mov [si+4], cx   // cx
            mov [si+6], dx   // dx
            mov [si+8], si   // si
            mov [si+10], di  // di
            mov [si+12], bp  // bp
            mov ax, sp
            mov [si+14], ax  // sp
            mov ax, ss
            mov [si+22], ax  // ss
            mov ax, ds
            mov [si+18], ax  // ds
            mov ax, es
            mov [si+20], ax  // es
        }

        // Выбираем следующую задачу по приоритету
        unsigned int next_task = current_proc;
        int max_priority = -1;
        
        for(int i = 0; i < 3; i++) {
            if(i != current_proc && priorities[i] > max_priority) {
                max_priority = priorities[i];
                next_task = i;
            }
        }
        
        if(max_priority > priorities[current_proc]) {
            current_proc = next_task;
        }

        // Восстанавливаем контекст новой задачи
        asm {
            mov si, [current_proc]
            mov cl, 5
            shl si, cl
            mov ax, [stats_offset]
            add si, ax
            mov ax, [si+22]  // ss
            mov ss, ax
            mov ax, [si+14]  // sp
            mov sp, ax
            mov ax, [si+26]  // flags
            push ax
            mov ax, [si+16]  // cs
            push ax
            mov ax, [si+24]  // ip
            push ax
            mov ax, [si+18]  // ds
            mov ds, ax
            mov ax, [si+20]  // es
            mov es, ax
            mov ax, [si+0]   // ax
            mov bx, [si+2]   // bx
            mov cx, [si+4]   // cx
            mov dx, [si+6]   // dx
            mov di, [si+10]  // di
            mov bp, [si+12]  // bp
            mov si, [si+8]   // si
        }
    }
}

void Proc1(void);
void Proc2(void);
void Proc3(void);

int main(void)
{
    unsigned int i, rd, rc, re, rs, ri0, ri1, ri2, f;

    for(i=0;i<3;i++)
        memset(&Stats[i],0,sizeof(ProcStats));
    
    Stats[0].priority = priorities[0];
    Stats[1].priority = priorities[1]; 
    Stats[2].priority = priorities[2];

    asm {
        mov ax, cs
        mov [rc], ax
        mov ax, ds
        mov [rd], ax
        mov ax, es
        mov [re], ax
        mov ax, ss
        mov [rs], ax
        mov [ri0], offset Proc1
        mov [ri1], offset Proc2
        mov [ri2], offset Proc3
        pushf
        pop ax
        mov [f], ax
    }
    
    for(i=0;i<3;i++) {
        Stats[i].rcs = rc;
        Stats[i].rds = rd;
        Stats[i].res = re;
        Stats[i].rss = rs;
        Stats[i].rflags = f;
    }
    
    Stats[0].rip = ri0;
    Stats[1].rip = ri1;
    Stats[2].rip = ri2;
    
    current_proc = 0;
    stats_offset = (unsigned)&Stats;
    oldHandler = getvect(0x9);
    setvect(0x9,IntHandler);
    
    printf("Press SPACE to switch tasks\n");
    printf("Priorities: Task1=%d, Task2=%d, Task3=%d\n", priorities[0], priorities[1], priorities[2]);
    printf("Starting Task1...\n");
    
    Proc1();
    setvect(0x9,oldHandler);

    return 0;
}

void Proc1(void)
{
    while(1) {
        printf("1");
        delay(100);
    }
}

void Proc2(void)
{
    while(1) {
        printf("2");
        delay(100);
    }
}

void Proc3(void)
{
    while(1) {
        printf("3");
        delay(100);
    }
}