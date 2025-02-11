#pragma once

#include "types.h"

#define GDT_ADDR	0x800

/* P + DPL + S + 4 bits */
#define KERNEL_RING	0b10010000
#define USER_RING	0b11110000

/* 4 bits + E + DC + RW + A */
#define CODE		0b00001010
#define DATA		0b00000010
#define STACK		0b00000110



struct __attribute__((packed)) gdt_segment
{
	uint16_t	limit1;			// bits 0 - 15						: 0xFFFF
	uint16_t	base1;			// bits 16 - 31						: 0x0000
	uint8_t		base2;			// ...								: 0x00
	uint8_t		access;			// ...								: KERNEL | DATA
	uint8_t		flags_limit2;	// flags (0b1100) + limit (0b1111)	: 0xCF
	uint8_t		base3;			// bits 56 - 63						: 0x00
};

struct __attribute__((packed)) gdt_descriptor
{
	uint16_t	limit;
	uint32_t	base;	/* GDTR -> GDT Descriptor */
};

struct gdt
{
	struct gdt_segment		segment[7];
	struct gdt_descriptor	descriptor;
};

void		init_gdt(void);
extern void	get_gdtd(struct gdt_descriptor *gdtd);
extern void	load_gdt(void *gdtd);
extern void	enable_protected_mode(void);

