# KFS2

KFS2 is a simple operating system kernel written in C and assembly for the x86 architecture. This project demonstrates basic kernel development, including setting up a multiboot-compliant kernel, initializing a terminal, and displaying text on the screen.
This part's goal is setting up a Global Descriptor Table and load it into the kernel.

## Prerequisites

To build and run KFS2, you need the following tools installed:

- Docker
- QEMU

## Building KFS2

1. To build the container, run:
```sh
docker build cross_compiler -t cross_compiler_env
```

2. To run the container on windows powershell, run (--rm: kill container if it exists, -i: interactive mode, -t: allocate a terminal to the container):
```sh
docker run --rm -it -v "${pwd}:/root/cc" cross_compiler_env
```

For linux run:
```sh
docker run --rm -it -v "$(pwd):/root/cc" cross_compiler_env
```

Once inside the container, simply run make to compile the project.


## Running KFS2

Don't forget to add qemu folder to the path !

Outside of the container, if the kernel iso has been compiled, to launch it in qemu run:
```sh
qemu-system-i386 -cdrom .\kernel.iso
```
or with the linux syntax:
```sh
qemu-system-i386 -cdrom ./kernel.iso
```

## Project Structure

- [`src`](src/): Contains the source code for the kernel
    - [`boot.s`](src/boot.s): Assembly code for the bootloader
    - [`header.s`](src/header.s): Assembly code for the multiboot header
    - [`io.s`](src/io.s): Assembly code for port I/O functions
    - [`gdt.s`](src/gdt.s): Assembly code for GDT related functions
    - [`kernel.c`](src/kernel.c): C code for the kernel
    - [`printk.c`](src/printk.c): C code for printk function
    - [`cursor.c`](src/cursor.c): C code for cursor resize and repositioning
    - [`gdt.c`](src/gdt.c): C code for GDT initializing
    - [`linker.ld`](src/linker.ld): Linker script for the kernel
- [`Makefile`](Makefile): Build script for the project
- [`grub.cfg`](grub.cfg): GRUB configuration file for booting the kernel