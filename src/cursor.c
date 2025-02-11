#include "cursor.h"
#include "kernel.h"
#include "io.h"

void resize_cursor(uint8_t start, uint8_t end)
{
	out8(0x3D4, 0x0A);
	out8(0x3D5, (in8(0x3D5) & 0xC0) | start);
	out8(0x3D4, 0x0B);
	out8(0x3D5, (in8(0x3D5) & 0xE0) | end);
}

void set_cursor_coord(uint8_t row, uint8_t col)
{
	uint16_t idx;
	
	idx = row * VGA_WIDTH + col;

	out16(0x3D4, (uint16_t)((idx & 0xFF) << 8 | 0x0F));
	out16(0x3D4, (uint16_t)(((idx >> 8) & 0xFF) << 8 | 0x0E));

	// uint16_t pos = row * VGA_WIDTH + col;

	// out8(0x3D4, 0x0F);
	// out8(0x3D5, (uint8_t) (pos & 0xFF));
	// out8(0x3D4, 0x0E);
	// out8(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
