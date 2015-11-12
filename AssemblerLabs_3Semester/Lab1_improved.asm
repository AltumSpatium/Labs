model small
.stack 200h
.data
	a dw 1
	b dw 2
	c dw 3
	d dw 4
  lexl dw 0
  lexh dw 0
  rexl dw 0
  rexh dw 0
	result dw 0
.code
	mov ax, @data
	mov ds, ax
	
	Main:
	
	; left expression:
	
	mov ax, a	; a * c
	mov bx, c
	mul bx
  mov lexl, ax
	mov lexh, dx

	mov ax, b	; b * d
	mov bx, d
	mul bx
	
	add ax, lexl ;
	adc dx, lexh ; a * c + b * d

	mov lexl, ax
	mov lexh, dx

	; right expression:
	
	mov ax, a	; a * d
	mov bx, d
	mul bx
	mov rexl, ax
	mov rexh, dx
	
	mov ax, b	; b * c
	mov bx, c
	mul bx
	
	add ax, rexl
	adc dx, rexh
	
	mov rexl, ax
	mov rexh, dx
	
  ; comparing:
	
	mov ax, lexh
	mov bx, rexh
	cmp ax, bx

	JNZ NotEquals	; if not

  mov ax, lexl
  mov bx, rexl
  cmp ax, bx
  
  JNZ NotEquals

	mov ax, a
	mov bx, a
	mul bx
	mov result, ax
	JMP EndPr
	
	NotEquals:
	
	mov ax, a
	mov bx, c
	cmp ax, bx
	JC Lower	; if a < c
	JZ Lower	; if a == c

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
	
