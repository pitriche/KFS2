global start

extern kernel_main

; ##############################################################################

section .text

start:
	mov esp, stack_top
	call kernel_main

	; disable interrupts and halt execution
	cli
.hang:
	hlt
	jmp .hang

; ##############################################################################

section .bss

align 16
stack_bottom:
	resb 16384	; Reserve 16 KiB for stack
stack_top:
