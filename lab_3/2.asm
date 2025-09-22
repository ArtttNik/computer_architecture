.model small
.stack 100h


.data
    A db 0,1,2,3,4,5,6,7,8,9
    B db 0,0,0,0,0,0,0,0,0,0

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
    mov ax,@data
    mov ds,ax
    mov es,ax

    cld
    lea si,A
    lea di,B
    mov cx,10
    rep movsb

    lea si,B
    mov cx,10
print_loop:
    lodsb
    PRINT_DIGIT al
    PRINT_CHAR ' '
    loop print_loop

    mov ax,4C00h
    int 21h
main endp
end main
