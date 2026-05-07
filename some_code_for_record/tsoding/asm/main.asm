format ELF64 executable 3
entry _start

_start:
	mov eax, 1
	mov edi, 1
	mov rsi, msg
	mov edx, msg_len
	syscall

	mov eax, 60
	xor edi, edi
	syscall

segment readable writeable

msg db "Hello, FASM!", 10
msg_len = $ - msg
