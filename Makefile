src_path :=		src/
obj_path :=		obj/
kern_path :=	iso/boot/

c_obj_path :=	$(obj_path)c/
c_src :=		$(wildcard $(src_path)*.c)
c_obj :=		$(c_src:$(src_path)%.c=$(c_obj_path)%.o)

asm_obj_path :=	$(obj_path)asm/
asm_src :=		$(wildcard $(src_path)*.s)
asm_obj :=		$(asm_src:$(src_path)%.s=$(asm_obj_path)%.o)

# -fno-ident : Do not generate special symbols for debug information (remove the comment section)
CFLAGS =		-m32 -c -ffreestanding -fno-builtin -fno-exceptions \
				-fno-stack-protector -nostdlib -nodefaultlibs -fno-ident \
				-I include -Wall -Wextra

LDFLAGS :=		-m elf_i386 -n -T src/linker.ld

kernel_bin :=	$(addprefix $(kern_path), kernel.bin)
kernel_iso :=	kernel.iso

CC :=			x86_64-elf-gcc
LD :=			x86_64-elf-ld
NASM :=			nasm

DOCKER_RUN :=	docker run --rm -v "$(PWD):/root/cc" cross-compiler

# ##############################################################################

all : $(kernel_iso)

all_linux :
	$(DOCKER_RUN) make $(kernel_iso)

# create the bootable iso
$(kernel_iso): $(kernel_bin)
	grub-mkrescue -o $(kernel_iso) --compress=xz iso

# compile the kernel
$(kernel_bin): $(obj_path) $(c_obj) $(asm_obj) 
	$(LD) $(LDFLAGS) -o $(kernel_bin) $(asm_obj) $(c_obj)

# create build directory
$(obj_path) :
	mkdir -p $(asm_obj_path)
	mkdir -p $(c_obj_path)

# build asm object files in elf 32 (i386 32 bits compatible)
$(asm_obj_path)%.o : $(src_path)%.s
	$(NASM) -f elf32 $< -o $@

# build c object files
$(c_obj_path)%.o : $(src_path)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(obj_path)

fclean: clean
	rm -rf $(kernel_iso) $(kernel_bin)

re: fclean all


.PHONY: all clean fclean re
