%define MAGIC 0x1BADB002
%assign ALIGN 1
%assign MEMINFO 2
%assign FLAGS (ALIGN | MEMINFO)
%assign CHECKSUM (0x100000000 - (MAGIC + FLAGS))

section .mb_header
dd MAGIC
dd FLAGS
dd CHECKSUM
