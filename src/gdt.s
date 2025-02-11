global get_gdtd
global load_gdt
global enable_protected_mode
global get_eflags

; ##############################################################################

section .text


get_gdtd:
	mov eax, [esp + 4]
	sgdt [eax]
	ret

load_gdt:
	mov eax, [esp + 4]
	cli
	lgdt [eax]
	ret

enable_protected_mode:
	jmp 08h:.update_cs
.update_cs:

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, 0x18
	mov ss, ax

	mov eax, cr0
	or ah, 1
	mov cr0, eax

	ret

get_eflags:
	mov eax, cr0
	ret
