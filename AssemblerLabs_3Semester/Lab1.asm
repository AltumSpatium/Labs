model small
.stack 200h
.data
	a dw 1
	b dw 2
	c dw 3
	d dw 4
	result dw 0
.code
	mov ax, @data
	mov ds, ax
	
	Main:
	
	; left expression:
	
	mov ax, a	; a * c
	mov bx, c
	mul bx
	mov cx, ax
	
	mov ax, b	; b * d
	mov bx, d
	mul bx
	add cx, ax
	
	; right expression:
	
	mov ax, a	; a * d
	mov bx, d
	mul bx
	mov dx, ax
	
	mov ax, b	; b * c
	mov bx, c
	mul bx
	add dx, ax
	
	cmp cx, dx
	JZ Equals	; if left expression = right expression
	JNZ NotEquals	; if not
	
	Equals:
	
	mov ax, a
	mov bx, a
	mul bx
	mov result, ax
	JMP EndPr
	
	NotEquals:
	
	mov ax, a
	mov bx, c
	cmp ax, bx
	JNC Higher	; if a > c
	JC Lower	; if a < c
	
	Higher:
	
	mov ax, c
	add ax, b
	mov result, ax
	JMP EndPr
	
	Lower:
	
	mov ax, a
	mov bx, b
	or bx, c
	sub ax, bx
	mov result, ax
	JMP EndPr
	
	EndPr:
	
	int 21h
	end Main
	
