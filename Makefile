header_path :=	header/
src_path :=		src/
obj_path :=		obj/
kern_path :=	iso/boot/


asm_src_files :=	header.asm \
					main.asm
asm_obj_path :=		obj/asm/
asm_src :=			$(addprefix $(src_path), $(asm_src_files))
asm_obj :=			$(addprefix $(asm_obj_path), $(asm_src_files:.asm=.o))

c_src_files :=	main.c
c_obj_path :=	obj/c/
c_src :=		$(addprefix $(src_path), $(c_src_files))
c_obj :=		$(addprefix $(c_obj_path), $(c_src_files:.c=.o))

# -m32 to compile to i386 32 bits, -c to compile without linking
cflags :=	-m32 -c -ffreestanding -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs

kernel_bin :=	$(addprefix $(kern_path), kernel.bin)
kernel_iso :=	kernel.iso

# ##############################################################################

all : $(kernel_iso)

# create the bootable iso
$(kernel_iso): $(kernel_bin)
	grub-mkrescue -o $(kernel_iso) --compress=xz iso
# grub-mkrescue /usr/lib/grub/i386-pc -o $(kernel_iso) iso

# compile the kernel in i386 32 bits
$(kernel_bin): $(obj_path) $(asm_obj) $(c_obj)
	x86_64-elf-ld -m elf_i386 -n -o $(kernel_bin) -T linker.ld $(asm_obj) $(c_obj)

# create build directory
$(obj_path) :
	mkdir -p $(asm_obj_path)
	mkdir -p $(c_obj_path)

# build asm object files in elf 32 (i386 32 bits compatible)
$(asm_obj_path)%.o : $(src_path)%.asm
	nasm -f elf32 $< -o $@

# build c object files
$(c_obj_path)%.o : $(src_path)%.c
	x86_64-elf-gcc $(cflags) -I $(header_path) $< -o $@

clean:
	rm -rf $(obj_path)

fclean: clean
	rm -rf $(kernel_iso) $(kernel_bin)

re: fclean all


.PHONY: all clean fclean re
