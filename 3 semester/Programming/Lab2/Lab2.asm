.model small
.stack 200h
.data
	errorstr db "....Incorrect input!$"
.code
start:

	mov ax, @data
	mov ds, ax
	
	call Main
	
	mov ah, 4Ch
	int 21h
		
Main proc

	push ax
	push bx
	push dx
	
	call Read ; ввод первого числа
	
	push ax

	call Read ; ввод второго числа
	
	push ax
	pop bx
	pop ax
	
	xor dx, dx
	div bx
	
	call Write ; вывод на экран целой части
	
	pop dx
	pop bx
	pop ax

	ret
	
Main endp

; на входе: ax - число, которое нужно вывести
Write proc

	push ax
	push bx
	push cx
	push dx
	
	mov cx, 0
	
	loop1:
	
		mov dx, 0
		mov bx, 10
		div bx
		
		push dx ; сохраняем последнюю цифру
		
		inc cx ; увеличиваем количество цифр в стеке
		cmp ax, 0
		jnz loop1
	
	loop2:
	
		pop dx
		
		add dl, '0' ; получаем код символа
		mov ah, 02h
		int 21h
	
		dec cx ; уменьшаем кол-во цифр в стеке
		jnz loop2
	
		pop dx
		pop cx
		pop bx
		pop ax
	
	ret
  
Write endp

; на выходе: ax - введенное число
Read proc

	push dx
	push bx
	push cx
	
	mov cx, 0
	
    startloop:
  
		mov ah, 01h
		int 21h
	
		cmp al, 13 ; если ввод закончен, нажата клавиша Enter
		jz returnvalue
	
		cmp al, 8 ; если нажата клавиша Backspace
		jnz checkvalue
		
		; удаление последнего символа
		mov ax, cx
		mov bx, 10
		xor dx, dx
		div bx
		mov cx, ax
		
		jmp startloop	
	
	checkvalue:
	
		; проверка на не цифру
		cmp al, '0'
		jc errormessage
		cmp al, '9'
		jnc errormessage
	
		push ax
	
		; проверка на размер
		mov ax, cx
		mov bx, 10
		mul bx
		mov cx, ax
	
		cmp dx, 0 
		jnz errormessage ; если превысило размер
	
		pop ax
		
		; получаем само число
		mov ah, 0
		sub al, '0'
		add cx, ax
		jc errormessage ; если превысило размер
		
		jmp startloop
	
	returnvalue:
  
		mov ax, cx
	
		pop cx
		pop bx
		pop dx
	
    ret

Read endp	
	
errormessage:

	mov dx, offset errorstr
	mov ah, 09h
	int 21h
	mov ah, 4Ch
	int 21h
	
end start
