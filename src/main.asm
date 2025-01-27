global	start
global	VGA_MEM_ADDR
extern	main

KERNEL_STACK_ADDR equ	kernel_stack_bottom
USER_STACK_ADDR equ 	user_stack_bottom
VGA_MEM_ADDR equ 		0xb8000

; ##############################################################################

section .text

bits 32

start:
	mov esp, kernel_stack_top

	call main

	;mov dword [0xb8000], 0x1f321f34
	;lgdt [gdt_descriptor]
	hlt

; ##############################################################################

; bss section, contains the stack memories
section .bss

kernel_stack_bottom:
	resb 16384	; 16kb stack
kernel_stack_top:

user_stack_bottom:
	resb 16384	; 16kb stack
user_stack_top: