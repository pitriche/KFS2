#ifndef KERNEL_H
# define KERNEL_H

/*
 * NOTES:
 *
 * Type sizes are changing and creating weird bugs if x86_64 c is executed in
 * 32 bits i386
 *
 * The + operator have priority over the bitshift operators, always use brackets
 * for multiple operators in the same line
 *
 * The __attribute__((packed)) attribute is absolutely vital for any structure
 * that is to be mapped in memory, due to padding ruining everything otherwise
 * (fuck padding)
*/

/* 32 bits unambiguous types */
typedef unsigned long long	u64;
typedef unsigned int		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;
typedef long long			i64;
typedef int					i32;
typedef short				i16;
typedef char				i8;

unsigned int *VGA_MEM_ADD;

#endif