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

; �窠 �室� � �ணࠬ��
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

; �� �室�: dx - ���ଠ樮���� ��ப�
; �� ��室�: ax - ��������� �᫮
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

    ; �뢮��� ���ଠ樮���� ��ப�
    mov dx, offset enterNstr
  
    call Read
	mov N, ax
	
	; �뢮��� ���ଠ樮���� ��ப�
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