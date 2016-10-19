.model small
.stack 200h

.data
  errorstr db 0ah, 0dh, "incorrect input !$"
  avaluestr db 0ah, 0dh, "first value  : $"
  bvaluestr db 0ah, 0dh, "second value : $"
  rvaluestr db 0ah, 0dh, "result value : $"

.code

; -----------------------------------------------
; точка входа в программу

start:

    mov ax, @data
    mov ds, ax
    call Main
    mov ah, 01h
    int 21h
    mov ah, 4Ch
    int 21h

; -----------------------------------------------
; завершение программы с выводом сообщения об ошибке

errormessage:

    mov dx, offset errorstr
    mov ah, 09h
    int 21h
    mov ah, 01h
    int 21h
    mov ah, 4Ch
    int 21h

; -----------------------------------------------------------------------------
; вход:
; dx - смещение информационной строки по ds
; выход:
; ax - введенное число

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

; вход:
; dx - смещение информационной строки по ds
; ax - число, которое нужно вывести

Write proc

  push ax
  push bx
  push cx
  push dx

  ; выводим информационную строку
  push ax
  mov ah, 09h
  int 21h
  pop ax

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


Main proc

  push ax
  push bx
  push dx

  mov dx, offset avaluestr
  call Read ; ввод первого числа
  push ax

  mov dx, offset bvaluestr
  call Read ; ввод второго числа
  push ax

  pop bx
  pop ax

  cwd
  idiv bx

  mov dx, offset rvaluestr
  call Write ; вывод на экран целой части

  pop dx
  pop bx
  pop ax

  ret

Main endp

end start