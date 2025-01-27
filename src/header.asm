; multiboot header, will be linked before the main section

section .mb_header
	dd 0x1badb002					; multiboot magic number
	dd 0							; architecture + flags, protected mode i386
	dd 0x100000000 - (0x1badb002)	; header checksum
