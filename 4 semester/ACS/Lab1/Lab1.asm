.model tiny
.386

.stack 100h

.data

	no_root db "The equation has no roots!$" ; If discriminant is less than zero
	root db "x = $" ; If discriminant equals zero
	root1 db "x1 = $" ; If discriminant is above zero
	root2 db ", x2 = $"
	a  dw 5
	b  dw 7
	c  dw 2
	c4 dw 4
	c2 dw 2
	c10 dw 10
	discrim dw ? ; Discriminant
	sqrtdiscrim dd ? ; The square root of the discriminant
	x1 dw ?
	x2 dw ?
	
	ccc dd 1.23

.code

; The number of output should be in st(0)
WriteFloat proc
	
    push ax cx dx ; Saving all registers in the stack

    push bp ; Creating a part of stack for keeping there number ten and any number
    mov bp, sp
    push 10
    push 0
	
    ftst ; Comparing the number with zero
    fstsw ax ; Saving SWR register state into register ax
    sahf ; Putting the value of the register ah into the low byte of the flag register EFLAGS
    
    jnc _positive ; If number is positive of equals

    mov     ah, 02h ; Deriving minus on the screen
    mov     dl, '-'
    int     21h

    fchs ; Changing the sign of a number

	_positive:
	
		fld1 ; Pushing number 1 into stack
		fld st(1) ; Loading number we need to display on top of the stack

		fprem ; Getting fraction from remainder of the division by 1
	
		fsub st(2), st ; Getting integer part by taking away the fraction
		fxch st(2) ; Exchanging values of st(0) and st(2)
		
		; Finding out the number of digits:
		xor cx, cx

		letters_count:  
	
			fidiv word ptr [bp - 2] ;Divide the integer part by ten
			fxch st(1) ; Exchanging values of st(0) and st(1)
			fld st(1) ; Loading that number again into st(0)

			fprem ; Separate the fraction part

			fsub st(2), st ; Leaving only int part of the quotient

			fimul word ptr [bp - 2] ; Multiplying by ten
			fistp word ptr [bp - 4] 
			inc cx ; Increasing the counter
			push word ptr [bp - 4] ; Saving last number in the stack
			
			fxch st(1) ; Loading integer part of the quotient into st(0)

			ftst
			fstsw ax
			sahf
			
			jnz short letters_count ; Repeating it, until integer part becomes zero !!!!!!!!!!!!!!

		mov ah, 02h
		
		writeinteger: ; Displaying on the screen
		
			pop dx ; Another digit
			add dl, '0' ; Translating the number into a character
			int 21h

			loop writeinteger ; Repeat until all the numbers are displayed

        fstp st(0) ; Getting fraction part
        fxch st(1) ; into st(0)

        ftst ; Whether fraction part exists (not zero)
        fstsw ax
        sahf
		
        jz short exit ; If not exists

        mov ah, 02h ; If exists, displaying a point
        mov dl, '.'
        int 21h

        mov cx, 3 ; The fractional part is not more than three digits

		writefraction:
			
			fimul word ptr [bp - 2] ; Multiplying fraction by ten
			fxch st(1) 
			fld st(1) ; Loading it into st(0)

			fprem ; Separate the fraction part

			fsub st(2), st(0) ; Leaving only integer part
			fxch st(2)
			fistp word ptr [bp - 4] ; Saving this digit in memory

			mov ah, 02h
			mov dl, [bp - 4] ; Displaying last digit
			add dl, '0' ; Translating into symbol
			int 21h

			fxch st(1) ; Loading the quotient into st(0)
			
			ftst ; Whether fraction part exists (not zero)
			fstsw ax
			sahf

			loopnz writefraction ; If exists or is less than 3 digits

		exit:

			fstp st(0) ; Cleaning the stack
			fstp st(0) ;

			leave
			pop dx cx ax ; Restoring registers
			
			ret
	
WriteFloat endp

; Only for integer values
Write proc

	push ax bx cx dx

	mov cx, 0

	test ax, 8000h
	jz positive 

	push ax
	mov dl, '-'
	mov ah, 02h
	int 21h
	pop ax
	neg ax

	positive:

	loop1:

		mov dx, 0
		mov bx, 10
		div bx

		push dx 
		inc cx 
		cmp ax, 0
		jnz loop1

	loop2:

		pop dx

		add dl, '0' 
		mov ah, 02h
		int 21h

		dec cx 
		jnz loop2

		pop dx cx bx ax

	ret

Write endp

main:

	mov ax, @data
	mov ds, ax
	
	finit ; Initializing coprocessor
	
	fild b ; Loading coefficient b into st(0)
	fimul b ; Squaring b
	
	fild c4 ; 4
	fimul a ; 4 * a
	fimul c ; 4 * a * c
	
	fsubp st(1), st(0) ; b^2 - 4*a*c
	fist discrim ; Loading result into variable
	
	ftst ; Comparing discriminant to zero
	fstsw ax
	sahf
	
	jz equal ; If discriminant equals zero
	jc less ; If discriminant is less than zero
	
	more: ; If discriminant is above zero
		
		fsqrt ; Finding the square root of the discriminant
		fst sqrtdiscrim ; Saving it to variable
		
		neg b ; Changing sign of coefficient b
		
		fild b ; Loading coefficient b into st(0)
		fadd sqrtdiscrim ; Adding square root of the discriminant
		
		fild a ; Loading coefficient a into st(0)
		fimul c2 ; Multiplying by 2
		
		fdivp st(1), st(0) ; ( -b + sqrt(disc) ) / (2 * a)
		
		fist x1 ; Saving first root of equation to variable
		
		fild b ; Loading coefficient b into st(0)
		fsub sqrtdiscrim ; Subtract square root of the discriminant
		
		fild a ; a
		fimul c2 ; 2 * a
		
		fdivp st(1), st(0) ; ( -b - sqrt(disc) ) / (2 * a)
		
		fist x2 ; Saving second root of equation to variable
		
		; Displaying first root of equation
		lea dx, root1
		mov ah, 09h
		int 21h
		
		mov ax, x1
		
		call WriteFloat
		
		; Displaying second root of equation
		lea dx, root2
		mov ah, 09h
		int 21h
		
		mov ax, x2
		
		call WriteFloat
		
		jmp continue
		
	less:
		
		; Display message
		lea dx, no_root
		mov ah, 09h
		int 21h
		
		jmp continue
		
	equal:
		
		neg b ; Changing sign of coefficient b
		
		fild b ; Loading coefficient b into st(0)
		
		fild a ; a
		fimul c2 ; 2 * a
		
		fdivp st(1), st(0) ; ( -b / (2 * a) )
		
		fist x1 ; Saving root of equation to variable
		
		; Displaying root of equation
		lea dx, root
		mov ah, 09h
		int 21h
		
		mov ax, x1
		
		call WriteFloat
		
		jmp continue
	
	continue:
	
		mov ah, 4Ch
		int 21h

end main