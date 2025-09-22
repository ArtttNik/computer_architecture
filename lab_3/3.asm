model small
.stack 100h

.data
    A db 5, 12, 123, 222, 123, 1, 111
    B db 7 dup(0)
.code

PRINT_CHAR MACRO sym
    mov dl, sym
    mov ah, 02h
    int 21h
ENDM

PRINT_DIGIT MACRO reg
    mov dl, reg
    add dl, '0'
    mov ah, 02h
    int 21h
ENDM

main proc
    mov ax, @data
    mov ds, ax
    mov es, ax

    cld
    lea si, A
    lea di, B
    mov cl, 7
    mov ch, 0
    rep movsb

    lea si, B
    mov cx, 7
print_loop:
    lodsb
    
    push ax
    push bx
    push cx
    push dx
    
    mov ah, 0
    mov cx, 0
DIV_LOOP:
    mov bl, 10
    div bl
    push ax
    inc cx
    mov ah, 0
    test al, al
    jnz DIV_LOOP
    
PRINT_LOOP_INNER:
    pop ax
    mov dl, ah
    add dl, '0'
    mov ah, 02h
    int 21h
    loop PRINT_LOOP_INNER
    
    PRINT_CHAR ' '
    
    pop dx
    pop cx
    pop bx
    pop ax
    
    loop print_loop

    mov ax, 4C00h
    int 21h
main endp
end main