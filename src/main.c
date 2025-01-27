#include "kernel.h"

#define SC_WIDTH	80
#define SC_HEIGHT	25

/* Write here to inspect values */
#define DEBUG_ADDR	0x2000

struct u64_exp
{
	u8	p1;
	u32	p2;
	u16	p3;
	u8	p4;
}__attribute__((packed));


void	print_char(u64 to_write)
{
	u8			*vga;
	struct u64_exp	to_write_struct;

	vga = (void *)VGA_MEM_ADD + 0xb8000;

	to_write_struct = *(struct u64_exp *)(&to_write);

	*(u8 *)(vga + 0) = to_write_struct.p1;
	*(u32 *)(vga + 1) = to_write_struct.p2;
	*(u16 *)(vga + 5) = to_write_struct.p3;
	*(u8 *)(vga + 7) = to_write_struct.p4;

	// vga[1] = to_write_struct.p2;
	// vga[2] = to_write_struct.p2 >> 8;
	// vga[3] = to_write_struct.p2 >> 16;
	// vga[4] = to_write_struct.p2 >> 24;

	// vga[5] = to_write_struct.p3;

	// vga[6] = to_write_struct.p4;
	// vga[7] = to_write_struct.p4 >> 8;
}

void	main()
{
	// u64 test = 0x1f321f341f391f36;
	u64 test = 0x1f321f34;
	test = (test << 32) + 0x1f391f36;
	print_char(test);
	// print_char(0x32);
	// *((unsigned int *)0xb8000) = 0x1f321f34;
	// *((unsigned char *)0xb8001) = 0x1f;
	// *((unsigned char *)0xb8002) = 0x32;
	// *((unsigned char *)0xb8003) = 0x1f;
}