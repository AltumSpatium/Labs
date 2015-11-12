.model small
.stack 200h
.data
	errorstr db "incorrect input!$"
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
	
	cwd
	idiv bx
	
	call Write ; вывод на экран целой части
	
	pop dx
	pop bx
	pop ax
	
	ret
	
Main endp

errormessage:

	mov dx, offset errorstr
	mov ah, 09h
	int 21h
	mov ah, 4Ch
	int 21h

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
	
	    cmp al, '-' ; если минус
        jnz checkdel
        		
	    or cx, 8000h ; взводим старший бит	
	    jmp startloop 
	
	checkdel:
	
		cmp al, 8 ; если нажата клавиша Backspace
		jnz checkvalue
		
        ; удаление последнего символа
		mov ax, cx
		and ax, 7FFFh ; модуль ax
		mov bx, 10
		xor dx, dx
		div bx
		and cx, 8000h ; оставляем в cx знак
		or ax, cx ; возвращаем знак в ax
		mov cx, ax
		
		jmp startloop	
	
	checkvalue:
	
		; проверка на не цифру
		cmp al, '0'
		jl errormessage
		cmp al, '9'
		jg errormessage
	
		push ax
	
		; проверка на размер
		mov ax, cx
		and ax, 7FFFh ; убираем знак в ax
		mov bx, 10
		mul bx
	
	    cmp dx, 0 
		jnz errormessage ; если превысило размер
        test ax, 8000h 
		jnz errormessage ; если занимает старший бит - бит знака
    			
		and cx, 8000h ; оставляем в cx только знак
		or ax, cx ; добавляем знак в ax
		mov cx, ax
		
		pop ax
		
		; получаем само число
		mov ah, 0
		sub al, '0'
		
		push cx
		
		and cx, 7FFFh ; сбрасываем знак в cx
		add cx, ax ; добавляем само число

        test cx, 8000h
		jnz errormessage ; если занимает старший бит
		
		pop ax
		
		and ax, 8000h ; получаем знак
		or cx, ax ; добавляем знак
				
		jmp startloop
	
	returnvalue:
    
        mov ax, cx
  
        test cx, 8000h ; если число положительное		
	    jz lastpop
	   
	    ; меняем знак числа
	    and ax, 7FFFh
        neg ax	  
				
	lastpop:
	
		pop cx
		pop bx
		pop dx
	
    ret

Read endp

; на входе: ax - число, которое нужно вывести
Write proc

	push ax
	push bx
	push cx
	push dx
	
	mov cx, 0
	
	test ax, 8000h
	jz Positive ; если положительное
	
	push ax
	
	; выводим минус
	mov dl, '-' 
	mov ah, 02h
	int 21h
	
	pop ax
	
	neg ax
	
	Positive:
	
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

end start