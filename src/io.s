global in8
global in16
global in32
global out8
global out16
global out32

; ##############################################################################

; IO Ports input functions
in8:
	mov dx, word[esp + 4]		; Load first argument, IO Port
	mov eax, 0
	in al, dx
	ret

in16:
	mov dx, word[esp + 4]		; Load first argument, IO Port
	mov eax, 0
	in ax, dx
	ret

in32:
	mov dx, word[esp + 4]		; Load first argument, IO Port
	mov eax, 0
	in eax, dx
	ret

; IO Ports output functions
out8:
	mov dx, word[esp + 4]		; Load first argument, IO Port
	mov al, byte[esp + 8]		; Load 2nd argument, 1 byte value to send
	out dx, al
	ret

out16:
	mov dx, word[esp + 4]		; Load first argument, IO Port
	mov ax, word[esp + 8]		; Load 2nd argument, 2 bytes value to send
	out dx, ax
	ret

out32:
	mov dx, word[esp + 4]		; Load first argument, IO Port
	mov eax, dword[esp + 8]	; Load 2nd argument, 4 bytes value to send
	out dx, eax
	ret
