.model small
.stack 100h
.data
	array db 256 dup(0)
.code

Read proc

    push ax
    push bx
    push cx
    push dx
    push di

    mov cx, 0
    mov di, offset array
    cld 

  rstart:

    mov ah, 01h
    int 21h

    cmp al, 13
    jz rend

    cmp al, 8
    jnz rsymbol

    cmp cx, 0
    jz rstart

    dec cx
    dec di

    mov dl, ' '
    mov ah, 02h
    int 21h

    mov dl, 8
    mov ah, 02h
    int 21h
	
    jmp rstart

   rsymbol:

    stosb
    inc cx
    cmp cx, size array - 1
    jb rstart

  rend:

    mov al, '$'
    stosb
    inc cx
    
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
	
    ret

Read endp


  Main:

	mov ax, @data
	mov ds, ax
	mov es, ax
	
	call Read
	
	mov dx, offset array
	mov ah, 09h
	int 21h
	
	mov ax, 4c00h
	int 21h
	end Main
	
