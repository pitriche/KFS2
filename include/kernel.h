#pragma once

#include "types.h"

#define VGA_WIDTH	80
#define VGA_HEIGHT	25
#define DEBUG_ADDR	0x2000
#define VGA_BUF_ADDR 0xb8000
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60


enum vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

struct term {
	uint32_t row;
	uint32_t col;
	uint8_t	 color;
	uint16_t buf[VGA_WIDTH * VGA_HEIGHT];
};

void			terminal_putchar(char c);
void			terminal_write(const char* str);

/* code is in gdt.s */
extern uint32_t	get_eflags(void);
