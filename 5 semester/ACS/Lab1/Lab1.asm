P286
model large

; 8-byte descriptor structure:
descriptor struc
	limit    dw 0 ; 2-byte limit (size_of_segment - 1)
	base_l   dw 0 ; low word of phys address
	base_h   db 0 ; high byte of phys address
	access   db 0 ; access byte
	reserved dw 0 ; reserved field
descriptor ends

; access bytes:
ACCESS_PRESENT        equ 10000000b ; segment is present in RAM
ACCESS_CODE_SEGMENT   equ 00011000b ; this segment is code segment
ACCESS_DATA_SEGMENT   equ 00010000b ; this segment is data segment
ACCESS_EXTENDS_DOWN   equ 00000100b ; segment can extends down (for stack)
ACCESS_WRITING_ALOWED equ 00000100b ; segment can be modified
ACCESS_CONFORMED      equ 00000010b ; segment can be accessed on all privilege levels

; segment types:
DATA_SEGMENT  = ACCESS_PRESENT or ACCESS_DATA_SEGMENT or ACCESS_WRITING_ALOWED
CODE_SEGMENT  = ACCESS_PRESENT or ACCESS_CODE_SEGMENT or ACCESS_CONFORMED
STACK_SEGMENT = ACCESS_PRESENT or ACCESS_DATA_SEGMENT or ACCESS_WRITING_ALOWED or ACCESS_EXTENDS_DOWN

; constants:
STACK_SIZE            equ 0400 ; size of stack

BIOS_DATA_SIZE        equ 0300 ; size of BIOS data area
BIOS_DATA_ADDRESS     equ 0400 ; address of BIOS data area

VIDEOMEM_MONOCHR      equ 0b000h ; segment of monochrome videoadapter's videomemory
VIDEOMEM_MONOCHR_SIZE equ 1000 ; size of segment of monochrome videoadapter's videomemory
VIDEOMEM_MONOCHR_LOW  equ 0000 ; low byte of phys address of segment of monochrome videoadapter's videomemory
VIDEOMEM_COLOR        equ 0b800h ; segment of color videoadapter's videomemory
VIDEOMEM_COLOR_SIZE   equ 4000; size of segment of color videoadapter's videomemory
VIDEOMEM_COLOR_LOW    equ 8000 ; low byte of phys address of segment of color videoadapter's videomemory
VIDEOMEM_HIGH         equ 0Bh ; high byte of phys address of videomemory segment

PROTECTED_MODE_BIT    equ 0001h ; protected mode transition bit
SHUT_DOWN_COMMAND     equ 0feh ; processor shut down command

; selectors:
SELECTOR_DS      = (gdt_ds - gdt_0) ; data segment selector
SELECTOR_CS      = (gdt_cs - gdt_0) ; code segment selector
SELECTOR_SS      = (gdt_ss - gdt_0) ; stack segment selector
SELECTOR_BIOS    = (gdt_bios - gdt_0) ; BIOS data area selector
SELECTOR_MONOCHR = (gdt_monochr - gdt_0) ; monochrome display videomemory selector
SELECTOR_COLOR   = (gdt_color - gdt_0) ; color display videomemory selector

; ports:
PORT_CMOS       equ 70h ; CMOS-memory access port
PORT_6845       equ 0063h ; BIOS data area address, where 6845 port value is situated
PORT_MONOCHR    equ 03b4h ; monochrome videocontroller port
PORT_COLOR      equ 03d4h ; color videocontroller port
PORT_STATUS     equ 64h ; keyboard status port
PORT_INT_MASKED equ 21h ; interruptions masking port

stack STACK_SIZE ; stack segment

dataseg ; data segment

DATA_SEGMENT_BEGIN = this word ; beginning of data segment

; real mode ss, sp, es registers' addresses:
real_ss dw ?
real_sp dw ?
real_es dw ?

GDT_BEGIN = $

; GDT table:
gdt label word
	gdt_0       descriptor <0, 0, 0, 0, 0>
	gdt_gdt     descriptor <GDT_SIZE-1,,,DATA_SEGMENT,0>
	gdt_ds      descriptor <DATA_SEGMENT_SIZE-1,,,DATA_SEGMENT,0>
	gdt_cs      descriptor <CODE_SEGMENT_SIZE-1,,,CODE_SEGMENT,0>
	gdt_ss      descriptor <STACK_SIZE-1,,,STACK_SEGMENT,0>
	gdt_bios    descriptor <BIOS_DATA_SIZE-1,BIOS_DATA_ADDRESS,0,DATA_SEGMENT,0>
	gdt_monochr descriptor <VIDEOMEM_MONOCHR_SIZE-1,VIDEOMEM_MONOCHR_LOW,VIDEOMEM_HIGH,DATA_SEGMENT,0>
	gdt_color   descriptor <VIDEOMEM_COLOR_SIZE-1,VIDEOMEM_COLOR_LOW,VIDEOMEM_HIGH,DATA_SEGMENT,0>

GDT_SIZE = ($ - GDT_BEGIN)

; videomemory information:
columns              db 80d ; amount of columns on the screen
rows                 db 25d ; amount of rows on the screen
videobuffer_address  dw VIDEOMEM_COLOR ; videobuffer's segment address
videobuffer_selector dw SELECTOR_COLOR ; videobuffer's selector
curr_line            dw 0d ; current line number

; message:
msg db " Hello from protected mode!! $"

DATA_SEGMENT_SIZE = ($ - DATA_SEGMENT_BEGIN); data segment size

codeseg ; code segment

; main procedure
start proc
	; initializing data segment register
	mov ax, DGROUP
	mov ds, ax

	call set_videobuffer_base

	mov bh, 77h
	call clear_screen

	call init_PM

	call set_PM

	call write_message
	call pause

	call set_RM

	mov bh, 77h
	call clear_screen

	mov ah, 4Ch
	int 21h
start endp

; base videobuffer address definition
set_videobuffer_base proc near
	mov bx, word [es:PORT_6845]
	cmp bx, PORT_COLOR
	je videobuffer_exit

	mov [videobuffer_address], VIDEOMEM_MONOCHR
	mov [videobuffer_selector], SELECTOR_MONOCHR

videobuffer_exit:
	ret
set_videobuffer_base endp

; screen clearing procedure
clear_screen proc near
	xor cx, cx
	mov dl, [columns]
	mov dh, [rows]
	mov ax, 0600h
	int 10h
	ret
clear_screen endp

; macrocommand for writing segment base address into descriptor
set_gdt_entry macro
	mov (descriptor.base_l) [bx], ax
	mov (descriptor.base_h) [bx], dl
endm

; preparation before entering protected mode
init_PM proc near
	; filling GDT:

	; computing data segment base address
	mov ax, DGROUP
	mov dl, ah
	shl ax, 4 ; low 2 bytes of address
	shr dl, 4 ; high byte of address

	; saving base address to di:si
	mov si, ax
	mov di, dx

	; creating GDT descriptor
	add ax, offset GDT
	adc dl, 0
	mov bx, offset gdt_gdt
	set_gdt_entry

	; creating data segment descriptor
	mov ax, si
	mov dx, di
	mov bx, offset gdt_ds
	set_gdt_entry

	; creating code segment descriptor
	mov ax, cs
	mov dl, ah
	shl ax, 4
	shr dl, 4
	mov bx, offset gdt_cs
	set_gdt_entry

	; creating stack segment descriptor
	mov ax, ss
	mov dl, ah
	shl ax, 4
	shr dl, 4
	mov bx, offset gdt_ss
	set_gdt_entry

	; saving real mode return address to
	; BIOS data area on address 0040h:0067h
	push ds
	mov ax, 40
	mov ds, ax
	mov word [ds:67], offset shutdown_return
	mov word [ds:69], cs
	pop ds

	; masking all interruptions
	; writing code 5 into CMOS 0Fh cell
	; allows processor transfer control
	; to address 0040h:0067h after reset
	cli ; masking masked interruptions
	mov al, 8fh ; 8fh instead of 0fh because we're at the same time masking unmasked
				; interruptions - setting high bit in CMOS cell
	out PORT_CMOS, al

	mov al, 5
	out PORT_CMOS+1, al ; writing return code

	ret
init_PM endp

; transition to protected mode
set_PM proc near
	; writing in es videomemory segment address
	mov ax, [videobuffer_address]
	mov es, ax

	; saving stack pointer for real mode
	mov [real_ss], ss
	mov [real_es], es

	; loading GDT register
	lgdt qword [gdt_gdt]

	; enabling protected mode
	mov ax, PROTECTED_MODE_BIT
	lmsw ax

	; performing inter-segment transition
	db 0eah
	dw offset flush ; offset in next segment
	dw SELECTOR_CS ; selector of next segment

flush label far
	mov ax, SELECTOR_DS ; loading data segment
	mov ds, ax
	mov ax, SELECTOR_SS ; loading stack segment
	mov ss, ax
	ret
set_PM endp

; write some text on the screen to the x and y coordinates
; x = ax
; y = bx
; length = cx
; attribute = dh
write_on_screen proc near
	push si
	push di

	; computing offset in videobuffer for text writing
	; offset = ((y * columns) + x) * 2
	mov dl, [columns]
	mul dl
	add ax, bx
	shl ax, 1
	mov di, ax
	mov ah, dh ; writing to ah attribute byte

write_xy:
	lodsb ; next symbol in al
	stosw ; writing it into the videomemory
	loop write_xy ; by the end of string

	pop di
	pop si
	ret
write_on_screen endp

; writing text message on the screen
write_message proc near
	; loading videomemory selector to es
	mov ax, [videobuffer_selector]
	mov es, ax

	; setting message coordinates
	mov bx, 0
	mov ax, [curr_line]
	inc [curr_line]

	; loading message address and size
	mov si, offset msg
	mov cx, size msg
	mov dh, 30h ; attribute - black text on blue background

	call write_on_screen

	ret
write_message endp

; small time delay
pause proc near
	push cx
	mov cx, 50

loop0:
	push cx
	xor cx, cx

loop1:
	loop loop1
	pop cx
	loop loop0

	pop cx
	ret
pause endp

set_RM proc near
	; saving stack pointer value
	mov [real_sp], sp

	; reseting processor
	mov al, SHUT_DOWN_COMMAND
	out PORT_STATUS, al

	; waiting for reset
wait_reset:
	hlt
	jmp wait_reset

shutdown_return label far
	; initializing ds with data segment address
	mov ax, DGROUP
	mov ds, ax
	assume ds:DGROUP

	; restoring stack pointer
	mov ss, [real_ss]
	mov sp, [real_sp]

	; restoring es
	mov es, [real_es]

	; permitting all interruptions
	mov ax, 000dh
	out PORT_CMOS, al

	in al, PORT_INT_MASKED
	and al, 0
	out PORT_INT_MASKED, al
	sti
	ret
set_RM endp

CODE_SEGMENT_SIZE = ($ - start)

end start