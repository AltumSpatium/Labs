.model small
.stack 200h

.data
  array      db 256 dup(0)
  arraycount   dw 0
  vowels    db 'eyuioa', 0
  vowelscount dw 7
  loadstr    db 0Dh, 0Ah, 'Input text:', 0Dh, 0Ah, '$'
  resstr     db 0Dh, 0Ah, 'Result words:', 0Dh, 0Ah, '$'
  linestr    db 0Dh, 0Ah, '$'

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
; выход: в array - помещаются введенные символы
;        в arraycount - помещается число введенных символов

Read proc

    push ax
    push bx
    push cx
    push dx
    push di

    ; выводим информационную строку
    mov dx, offset loadstr
    mov ah, 09h
    int 21h

    mov cx, 0 ; обнуляем счетчик введенных символов
    mov di, offset array ; загружаем смещение начала буфера строки
    cld ; сброс флага направления

  rstart:

    ; получаем очередной символ
    mov ah, 01h
    int 21h

    cmp al, 13 ; если ввод закончен, нажата клавиша Enter
    jz rend

    cmp al, 8 ; если не нажата клавиша Backspace
    jnz rsymbol

    ; обработка клавиши Backspace

    cmp cx, 0 ; еще остались введенные символы
    jz rstart

    dec cx ; уменьшим число введенных символов
    dec di ; уменьшим индекс сохраняемого символа

    mov dl, ' ' ; затираем ранее введенный символ
    mov ah, 02h
    int 21h

    mov dl, 8 ; возвращаем курсор на символ назад
    mov ah, 02h
    int 21h
	
    jmp rstart

   rsymbol:

    ; обработка введенного символа

    stosb
    inc cx
    cmp cx, size array - 1
    jb rstart

  rend:

    ; дополним ввод разделителем
    mov al, ' '
    stosb
    inc cx
    mov arraycount, cx

    ; востанавливаем регистры и выходим
    
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
	
    ret

Read endp

; -----------------------------------------------
; вход: ds:si - индекс начала выводимого слова

Write proc

    push ax
    push bx
    push cx
    push dx
    push di
    push si

    ; выводим текущее слово

  wstart:

    lodsb
    cmp al, ' '
    jz wend

    ; выводим текущий символ

    mov dl, al
    mov ah, 02h
    int 21h
	
    jmp wstart

  wend:

    ; выводим строку завершения
    mov dx, offset linestr
    mov ah, 09h
    int 21h

    ; востанавливаем регистры и выходим

    pop si
    pop di
    pop dx
    pop cx
    pop bx
    pop ax

    ret

Write endp

; -----------------------------------------------
; вход: al - искомый символ
; выход: cf=0 - не найден, cf=1 - найден

Find proc

    push cx
    push di

    ; поиск заданного символа в массиве допустимых

    mov cx, vowelscount
    mov di, offset vowels
    repne scasb
    jcxz fclr ; прошли весь массив и ничего не нашли
    stc ; нашли и устанавливаем cf
    jmp fend

  fclr:

    clc ; не нашли и сбрасываем cf

  fend:

    ; востанавливаем регистры и выходим

    pop di
    pop cx

    ret

Find endp

; -----------------------------------------------
; вход: ds:si - индекс начала проверяемого слова
; выход: cf=0 - не подходит, cf=1 - подходит

Check proc

    push ax
    push si

    mov ah, 0 ; предыдущий символ не из допустимых

  cstart:

    lodsb
    cmp al, ' ' ; если нарвались на разделитель,
    jz cbad     ; то слово закончилось

    call Find ; ищем символ среди допустимых
    jc clast  ; нашли символ среди допустимых

    mov ah, 0 ; последний проверенный символ недопустим
    jmp cstart

  clast:

    cmp ah, 0 ; предыдущий символ был допустим
    jnz cok ; текущий и предыдущий символ был допустим

    mov ah, 1 ; последний проверенный символ допустим
    jmp cstart

   cbad:

     clc ; не наше слово и сбрасываем cf
     jmp cend

   cok:

     stc ; наше слово и устанавливаем cf

  cend:

    ; востанавливаем регистры и выходим

    pop si
    pop ax

  ret

Check endp

; -----------------------------------------------

Main proc

  push ax
  push bx
  push cx
  push dx
  push di
  push si

  call Read ; ввод текста в буффер

  ; выводим строку завершения
  mov dx, offset linestr
  mov ah, 09h
  int 21h

  ; выводим информационную строку
  mov dx, offset resstr
  mov ah, 09h
  int 21h

  mov cx, arraycount ; счетчик доступных символов
  mov di, offset array
  cld

  mforstart:

  ; поиск начала слова

  mov al, ' '
  repe scasb
  jcxz mforend ; закончился счетчик доступных символов

  ; возвращаемся на последний проанализированный символ (на начало слова)

  dec di
  inc cx

  ; проверка текущего слова

  mov si, di
  call Check
  jnc mignore ; текущее слово не подходит

  ; выводим текущее слово

  call Write

  mignore:

  ; обходим текущее слово (ищем его окончание, т.е. первый после него разделитель)

  repne scasb
  jcxz mforend

  ; возвращаемся на первый разделитель после слова

  dec di
  inc cx

  jmp mforstart

  mforend:

  ; востанавливаем регистры и выходим

  pop si
  pop di
  pop dx
  pop cx
  pop bx
  pop ax

  ret

Main endp

end start
