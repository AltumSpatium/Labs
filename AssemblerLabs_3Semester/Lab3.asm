.model small
.stack 200h
.data
  errorstr db 0ah, 0dh, "incorrect input !$"
  avaluestr db 0ah, 0dh, "first value  : $"
  bvaluestr db 0ah, 0dh, "second value : $"
  rvaluestr db 0ah, 0dh, "result value : $"
.code

; -----------------------------------------------
; �窠 �室� � �ணࠬ��

start:

    mov ax, @data
    mov ds, ax
    
    call Main
    
    mov ah, 01h
    int 21h
    
    mov ah, 4Ch
    int 21h

; -----------------------------------------------
; �����襭�� �ணࠬ�� � �뢮��� ᮮ�饭�� �� �訡��

errormessage:

    mov dx, offset errorstr
    mov ah, 09h
    int 21h
    
    mov ah, 01h
    int 21h
    
    mov ah, 4Ch
    int 21h

; -----------------------------------------------------------------------------
; �室:
; dx - ᬥ饭�� ���ଠ樮���� ��ப� �� ds
; ��室:
; ax - ��������� �᫮

Read proc

    push dx
    push bx
    push cx

    ; �뢮��� ���ଠ樮���� ��ப�
    mov ah, 09h
    int 21h

  startin:

    mov cx, 0 ; ����塞 ���稪 ��������� ᨬ����� (cl) � �ਧ��� ����⥫쭮�� �᫠ (ch)
    mov bx, 0 ; ����塞 �������� �᫮

  startloop:

    ; ����砥� ��।��� ᨬ���
    mov ah, 01h
    int 21h

    cmp al, 13 ; �᫨ ���� �����祭, ����� ������ Enter
    jnz checkdel
    jmp returnvalue

  checkdel:

    cmp al, 8 ; �᫨ �� ����� ������ Backspace
    jnz checksymbol

    ; ��ࠡ�⪠ ������ Backspace

    mov dl, ' ' ; ���ࠥ� ࠭�� �������� ᨬ���
    mov ah, 02h
    int 21h

    cmp cl, 0 ; �� ��⠫��� �������� ᨬ����
    jz startloop

    dec cl ; 㬥��訬 �᫮ ��������� ᨬ�����

    mov dl, 8 ; �����頥� ���ᮢ �� ᨬ��� �����
    mov ah, 02h
    int 21h

    cmp cl, 0 ; ��᫥���� ����� ᨬ���
    jz startin

    ; 㤠�塞 ��᫥���� ���� �᫠

    mov ax, bx
    cwd
    mov bx, 10
    idiv bx
    mov bx, ax
    jmp startloop

   checksymbol:

    ; �஢�ઠ ���������� ᨬ����

    cmp al, '-' ; �᫨ �� �� ᨬ��� �����
    jnz checkvalue

    cmp cl, 0 ; �᫨ �� �� ���� ᨬ���
    jnz errormessage

    ; ��⠭�������� �ਧ��� ����⥫쭮�� �᫠

    mov ch, 1
    inc cl
    jmp startloop

  checkvalue:

    ; �஢�ઠ �� �� ����

    cmp al, '0'
    jl errormessage
    cmp al, '9'
    jg errormessage

  setvalue:

    inc cl ; 㢥��稬 �᫮ ��������� ᨬ�����
    sub al, '0' ; ����砥� ���祭�� �᫠ ��� ���������� ᨬ����
    mov ah, 0

    cmp bx, 0 ; �� �� ��ࢠ� ������ ���
    jnz addvalue

    ; ���४��㥬 ���祭�� ��� ����� ����

    mov bx, ax
    cmp ch, 0 ; ������ ������⥫쭮� �᫮
    jz startloop
    
    neg bx ; ������ ����⥫쭮� �᫮
    jmp startloop

    addvalue:

    ; ���४��㥬 ���祭�� ��� ��।��� ����

    push ax
    mov ax, bx
    xor dx, dx
    mov bx, 10
    imul bx
    mov bx, ax
    pop ax

    ; �஢�ਬ १���� �� �����⨬����

    cmp ch, 0 ; ������ ������⥫쭮� �᫮
    jz checkres

    ; ���४�஢�� ����⥫쭮�� �᫠
    
    sub bx, ax
    sbb dx, 0

    ; �஢�ઠ ���४⭮�� ����⥫쭮�� �᫠

    cmp dx, 0FFFFh
    jnz toerror
    test bx, 8000h
    jz toerror
    jmp startloop

  checkres:

    ; ���४�஢�� ������⥫쭮�� �᫠

    add bx, ax
    adc dx, 0

    ; �஢�ઠ ���४⭮�� ������⥫쭮�� �᫠

    cmp dx, 0000h
    jnz toerror
    test bx, 8000h
    jnz toerror
    jmp startloop

  toerror:

    ; ���室 �� �����襭�� � �訡���

    jmp errormessage

  returnvalue:

    mov ax, bx ; �����頥� १����

    ; �஢��ਬ ���४⭮��� ����⥫쭮�� �᫠
    
    cmp cl, 0 ; �� �����-���� ����
    jz  toerror
    cmp bx, 0 ; ���� �� �㫥��� �᫮
    jnz restreg
    cmp ch, 0 ; �� ������ ᨬ��� ����⥫쭮�� �᫠
    jnz  toerror

  restreg:

    ; ���⠭�������� ॣ����� � ��室��
    
    pop cx
    pop bx
    pop dx
    ret

Read endp

; -----------------------------------------------

; �室:
; dx - ᬥ饭�� ���ଠ樮���� ��ப� �� ds
; ax - �᫮, ���஥ �㦭� �뢥��

Write proc

  push ax
  push bx
  push cx
  push dx

  ; �뢮��� ���ଠ樮���� ��ப�
  push ax
  mov ah, 09h
  int 21h
  pop ax

  mov cx, 0

  test ax, 8000h
  jz Positive ; �᫨ ������⥫쭮�

  ; �뢮��� �����
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

    push dx ; ��࠭塞 ��᫥���� ����
    inc cx ; 㢥��稢��� ������⢮ ��� � �⥪�
    cmp ax, 0
    jnz loop1

  loop2:

    pop dx

    add dl, '0' ; ����砥� ��� ᨬ����
    mov ah, 02h
    int 21h

    dec cx ; 㬥��蠥� ���-�� ��� � �⥪�
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
  call Read ; ���� ��ࢮ�� �᫠
  push ax

  mov dx, offset bvaluestr
  call Read ; ���� ��ண� �᫠
  push ax

  pop bx
  pop ax

  cwd
  idiv bx

  mov dx, offset rvaluestr
  call Write ; �뢮� �� �࠭ 楫�� ���

  pop dx
  pop bx
  pop ax

  ret

Main endp

end start