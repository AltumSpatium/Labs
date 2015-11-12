.model small

.stack 200h

.data
  enterNstr db 'Enter N:', 0Dh, 0Ah, '$'
  enterMstr db 0Dh, 0Ah, 'Enter M:', 0Dh, 0Ah, '$'
  enterAstr db 0Dh, 0Ah, 'Enter A:', 0Dh, 0Ah, '$'
  N dw 0
  M dw 0
  A dw 0
  array dw 1000 dup (0)
  inputmatrix db 0Dh, 0Ah, 'Enter matrix element:', 0Dh, 0Ah, '$'
  originalmatrix db 0Dh, 0Ah, 'Original matrix:', 0Dh, 0Ah, '$'
  resultmatrix  db 0Dh, 0Ah, 'Result matrix:', 0Dh, 0Ah, '$'
  errorstr db 0Dh, 0Ah, "Incorrect input!", 0Dh, 0Ah, '$'
  errordimstr db 0Dh, 0Ah, "Incorrect dimension!", 0Dh, 0Ah, '$'

.code

; -----------------------------------------------

; точка входа в программу
start:

    mov ax, @data
    mov ds, ax
    mov es, ax
    
    call Main
    
    mov ah, 01h
    int 21h
    
    mov ah, 4Ch
    int 21h

; -----------------------------------------------------------------------------

; на входе: dx - информационная строка
; на выходе: ax - введенное число
Read proc

    push dx
    push bx
    push cx

    ; выводим информационную строку
    mov ah, 09h
    int 21h

  startin:

    mov cx, 0 ; обнуляем счетчик введенных символов (cl) и признак отрицательного числа (ch)
    mov bx, 0 ; обнуляем вводимое число

  startloop:

    ; получаем очередной символ
    mov ah, 01h
    int 21h

    cmp al, 13 ; если ввод закончен, нажата клавиша Enter
    jnz checkdel
    jmp returnvalue

  checkdel:

    cmp al, 8 ; если не нажата клавиша Backspace
    jnz checksymbol

    ; обработка клавиши Backspace

    mov dl, ' ' ; затираем ранее введенный символ
    mov ah, 02h
    int 21h

    cmp cl, 0 ; еще остались введенные символы
    jz startloop

    dec cl ; уменьшим число введенных символов

    mov dl, 8 ; возвращаем курсов на символ назад
    mov ah, 02h
    int 21h

    cmp cl, 0 ; последний стертый символ
    jz startin

    ; удаляем последний ввод числа

    mov ax, bx
    cwd
    mov bx, 10
    idiv bx
    mov bx, ax
    jmp startloop

   checksymbol:

    ; проверка введенного символа

    cmp al, '-' ; если это не символ минус
    jnz checkvalue

    cmp cl, 0 ; если это не первый символ
    jnz errormessage

    ; устанавливает признак отрицательного числа

    mov ch, 1
    inc cl
    jmp startloop

  checkvalue:

    ; проверка на не цифру

    cmp al, '0'
    jl errormessage
    cmp al, '9'
    jg errormessage

  setvalue:

    inc cl ; увеличим число введенных символов
    sub al, '0' ; получаем значение числа для введенного символа
    mov ah, 0

    cmp bx, 0 ; это не первая значащая цифра
    jnz addvalue

    ; корректируем значение под первую цифру

    mov bx, ax
    cmp ch, 0 ; вводим положительное число
    jz startloop
    
    neg bx ; вводим отрицательное число
    jmp startloop

    addvalue:

    ; корректируем значение под очередную цифру

    push ax
    mov ax, bx
    xor dx, dx
    mov bx, 10
    imul bx
    mov bx, ax
    pop ax

    ; проверим результат на допустимость

    cmp ch, 0 ; вводим положительное число
    jz checkres

    ; корректировка отрицательного числа
    
    sub bx, ax
    sbb dx, 0

    ; проверка корректности отрицательного числа

    cmp dx, 0FFFFh
    jnz toerror
    test bx, 8000h
    jz toerror
    jmp startloop

  checkres:

    ; корректировка положительного числа

    add bx, ax
    adc dx, 0

    ; проверка корректности положительного числа

    cmp dx, 0000h
    jnz toerror
    test bx, 8000h
    jnz toerror
    jmp startloop

  toerror:

    ; переход на завершение с ошибкой

    jmp errormessage

  returnvalue:

    mov ax, bx ; возвращаем результат

    ; проверрим корректность отрицательного числа
    
    cmp cl, 0 ; был какой-либо ввод
    jz  toerror
    cmp bx, 0 ; есть не нулевое число
    jnz restreg
    cmp ch, 0 ; был введен символ отрицательного числа
    jnz  toerror

  restreg:

    ; востанавливаем регистры и выходим
    
    pop cx
    pop bx
    pop dx
    ret

Read endp

; -----------------------------------------------

errormessage:

	mov dx, offset errorstr
	mov ah, 09h
	int 21h
	mov ah, 01h
	int 21h
	mov ah, 4Ch
	int 21h


; -----------------------------------------------

Write proc

  push ax
  push bx
  push cx
  push dx

  mov cx, 0

  test ax, 8000h
  jz Positive ; если положительное

  ; выводим минус
  push ax
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

; -----------------------------------------------


EnterMatrix proc

	push ax
	push cx
	push dx
	push di
  
	mov dx, offset inputmatrix

	mov di, offset array  
	mov cx, M
  
	rows:
	
		push cx
		mov cx, N
	
	columns:
  
		call Read
	
		stosw
		
	loop columns
		
	pop cx
		
	loop rows
	
	mov dx, offset originalmatrix
	mov ah, 09h
	int 21h
	
	call ShowMatrix
		
	pop di
	pop dx
	pop cx
	pop ax

	ret

EnterMatrix endp

; -----------------------------------------------

ShowMatrix proc

	push ax
	push cx
	push si
	
	mov si, offset array
	mov cx, M
	
	external:
	
		push cx
		mov cx, N
		
	internal:
	
		lodsw
		
		call Write
		
		mov dl, ' '
		mov ah, 02h
		int 21h
		
	loop internal
	
		mov dl, 0Dh
		mov ah, 02h
		int 21h
		
		mov dl, 0Ah
		mov ah, 02h
		int 21h
		
		pop cx
		
	loop external
	
	pop si
	pop cx
	pop ax
	
	ret

ShowMatrix endp

; -----------------------------------------------

Task proc

	push ax
	push bx
	push cx
	push si
	push di

	xor	ax, ax
	mov	si, offset array
	mov di, offset array
	mov	bx, 0	
	mov	cx, M	
	           
	outcycle:
	
		push cx	
		mov	cx, N	
		
	incycle:
		
        lodsw
		
		cmp	ax, A
		jle svcycle
		mov ax, 0
	
     svcycle:
	    
		stosw
	
	loop incycle
	
		pop cx
			
	loop outcycle
	
	pop di
	pop si
	pop cx
	pop bx
	pop ax
	
	ret

Task endp

; -----------------------------------------------

Main proc

	push ax
	push bx
	push dx

    ; выводим информационную строку
    mov dx, offset enterNstr
  
    call Read
	mov N, ax
	
	; выводим информационную строку
    mov dx, offset enterMstr
	
	call Read
	mov M, ax
	
	mul N
	cmp dx, 0
	jnz dimensionerror
	
	cmp ax, 0
	jz dimensionerror
	
	cmp ax, 1000
	ja dimensionerror
	
	startenter:

	call EnterMatrix
	
	mov dx, offset enterAstr
	;mov ah, 09h
	;int 21h
	
	call Read
	mov A, ax
	
	call Task
	
	mov dx, offset resultmatrix
	mov ah, 09h
	int 21h
	
	call ShowMatrix

    pop dx
    pop bx
    pop ax

    ret

Main endp

; -----------------------------------------------

dimensionerror:

	mov dx, offset errordimstr
	mov ah, 09h
	int 21h
	mov ah, 01h
	int 21h
	mov ah, 4Ch
	int 21h

end start