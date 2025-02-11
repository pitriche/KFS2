#include "kernel.h"
#include "printk.h"
#include "io.h"
#include "cursor.h"
#include "keycodes.h"
#include "gdt.h"

# define NTERM 8

static struct term		terms[NTERM];
static struct term *	term = &terms[0];
static uint16_t * const	term_buffer = (uint16_t *)VGA_BUF_ADDR;
static char				kp_char[128];

/* ########################################################################## */

static uint16_t	vga_entry(char c)
{
	return ((uint16_t)c | (uint16_t)term->color << 8);
}

void cpy_term(void)
{
	for (uint16_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; ++i)
		term->buf[i] = term_buffer[i];
}

void paste_term()
{
	for (uint16_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; ++i)
		term_buffer[i] = term->buf[i];
}

void switch_term(uint8_t term_idx)
{
	cpy_term();
	term = &terms[term_idx];
	paste_term();
	set_cursor_coord(term->row, term->col);
}


/* 0:UP, 1:DOWN, 2:LEFT, 3:RIGHT */
void	terminal_cursor_move(uint8_t dir)
{
	switch (dir)
	{
		/* UP */
		case 0:
			if (term->row > 0)
				--term->row;
			break;

		/* DOWN */
		case 1:
			if (term->row < VGA_HEIGHT - 1)
				++term->row;
			break;

		/* LEFT */
		case 2:
			if (term->col <= 0 && term->row != 0)
			{
				term->col = VGA_WIDTH - 1;
				--term->row;
			}
			else if (term->col > 0)
				--term->col;
			break;

		/* RIGHT */
		case 3:
			if (term->col >= VGA_WIDTH - 1 && term->row != VGA_HEIGHT - 1)
			{
				term->col = 0;
				++term->row;
			}
			else if (term->col < VGA_WIDTH - 1)
				++term->col;
			break;
	}
	set_cursor_coord(term->row, term->col);
}

/* Scrolls the screen up by 1 line */
static void terminal_scroll(void)
{
	uint32_t i = 0;

	/* displace all screen data backwards by VGA_WIDTH * 2 bytes */
	for (; i < VGA_HEIGHT * VGA_WIDTH - VGA_WIDTH; ++i)
		term_buffer[i] = term_buffer[i + VGA_WIDTH];

	/* Clears the last line */
	for (; i < VGA_HEIGHT * VGA_WIDTH; i++)
		term_buffer[i] = vga_entry(' ');
}

void	terminal_newline()
{
	term->col = 0;
	if (term->row + 1 >= VGA_HEIGHT)
		terminal_scroll();
	else
		term->row++;
}

/* Clears the entire screen */
void	terminal_clear(void)
{
	for (uint32_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
		term_buffer[i] = vga_entry(' ');
}

/* writes a char at the designated coordinates without changing the cursor */
void	terminal_putchar_at(char c, uint32_t x, uint32_t y)
{
	term_buffer[y * VGA_WIDTH + x] = vga_entry(c);
}

void	terminal_del(void)
{
	terminal_cursor_move(2);
	terminal_putchar_at(' ', term->col, term->row);
	set_cursor_coord(term->row, term->col);
}

void	terminal_putchar(char c)
{
	if (c != '\n')
		terminal_putchar_at(c, term->col++, term->row);
	if (term->col >= VGA_WIDTH || c == '\n')
		terminal_newline();
	set_cursor_coord(term->row, term->col);
}

void	terminal_write(const char* str)
{
	for (uint32_t i = 0; str[i]; ++i)
		terminal_putchar(str[i]);
}

void	terminal_initialize(void)
{
	for (uint8_t i = 0; i < NTERM; ++i)
	{
		terms[i].row	= 0;
		terms[i].col	= 0;
		terms[i].color	= LIGHT_GREY | BLACK << 4;
		for (uint32_t j = 0; j < VGA_HEIGHT * VGA_WIDTH; j++)
			terms[i].buf[j] = vga_entry(' ');
	}

	/* Example of color change in a terminal */
	// terms[1].color = RED | BLACK << 4;

	/* Clear screen and set the cursor */
	terminal_clear();
	resize_cursor(14, 15); // default cursor
	set_cursor_coord(term->row, term->col);
}

void term_rainbow_write(const char* str)
{
	static uint8_t colors[8] = {RED, LIGHT_RED, LIGHT_BROWN, LIGHT_GREEN, CYAN, LIGHT_BLUE, MAGENTA, WHITE};

	for (uint32_t i = 0; str[i]; ++i)
	{
		term->color = colors[term->col / 10];
		terminal_putchar(str[i]);
	}
}

void	keyboard_initialize()
{
	for (int i = 0; i < 128; ++i)
		kp_char[i] = 0;
	kp_char[a] = 'a'; kp_char[b] = 'b'; kp_char[c] = 'c'; kp_char[d] = 'd';
	kp_char[e] = 'e'; kp_char[f] = 'f'; kp_char[g] = 'g'; kp_char[h] = 'h';
	kp_char[i] = 'i'; kp_char[j] = 'j'; kp_char[k] = 'k'; kp_char[l] = 'l';
	kp_char[m] = 'm'; kp_char[n] = 'n'; kp_char[o] = 'o'; kp_char[p] = 'p';
	kp_char[q] = 'q'; kp_char[r] = 'r'; kp_char[s] = 's'; kp_char[t] = 't';
	kp_char[u] = 'u'; kp_char[v] = 'v'; kp_char[w] = 'w'; kp_char[x] = 'x';
	kp_char[y] = 'y'; kp_char[z] = 'z';

	kp_char[zero] = '0'; kp_char[one] = '1'; kp_char[one + 1] = '2';
	kp_char[one + 2] = '3'; kp_char[one + 3] = '4'; kp_char[one + 4] = '5';
	kp_char[one + 5] = '6'; kp_char[one + 6] = '7'; kp_char[one + 7] = '8';
	kp_char[one + 8] = '9';

	kp_char[dash] = '-'; kp_char[equal] = '='; kp_char[enter] = '\n';
	kp_char[space] = ' ';
}

void	welcome_msg()
{
	term_rainbow_write("\n\n\n\n\n");
	term_rainbow_write("               :::     ::::::::        :::    ::: ::::::::: ::::::::: \n");
	term_rainbow_write("             :+:     :+:    :+:       :+:   :+:  :+:       :+:    :+: \n");
	term_rainbow_write("           +:+ +:+        +:+        +:+  +:+   +:+       +:+         \n");
	term_rainbow_write("         +#+  +:+      +#+          +#++:++    :#::+::#  +#++:++#++   \n");
	term_rainbow_write("       +#+#+#+#+#+  +#+            +#+  +#+   +#+              +#+    \n");
	term_rainbow_write("            #+#   #+#             #+#   #+#  #+#       #+#    #+#     \n");
	term_rainbow_write("           ###  ##########       ###    ### ###        ########       \n");
	term_rainbow_write("\n\n\n\n\n");
	term_rainbow_write("                       KERNEL FROM SCRATCH (KFS)                      \n");
	term_rainbow_write("                  Crafted with <3 by  pitriche & alabalet             \n");
	term_rainbow_write("                             School 42                                \n");
}

void	kernel_main(void)
{
	uint16_t				in;
	uint32_t				*gdt_seg;	/* one segment takes 2 elements here */
	struct gdt_descriptor	gdtd;

	init_gdt();
	terminal_initialize();
	keyboard_initialize();
	welcome_msg();

// ----- potentiellement a mettre dans une fonction
	switch_term(1);
	printk("Welcome to the kernel\n");
	get_gdtd(&gdtd);
	printk("GDT address: 0x%x\n", gdtd.base);
	printk("GDT size: %d\n", gdtd.limit);

	gdt_seg = (uint32_t *)gdtd.base;
	for (uint16_t i = 0; i < (gdtd.limit + 1); i += 8) // Une entrée GDT fait 8 octets (2)
		printk("GDT[%d]: 0x%x%x\n", i / 8, gdt_seg[i / 4], gdt_seg[i / 4 + 1]);
	printk("eflags register: 0b%i\n", get_eflags());
	switch_term(0);
// -----

	while (1)
	{

		/*
		 * Serial port 0x64 (status register) polling to wait for bit 1
		 * (output buffer full)
		 */
		if (in8(KEYBOARD_STATUS_PORT) & 1)
		{
			in = in16(KEYBOARD_DATA_PORT);
			// printk("keypress [0x%x]-", in);
			// printk("keypress [0x%x]  ", in & 0xff);

			/* print characters if keypress is mapped */
			if (!(in & 0b10000000) && kp_char[in & 0b01111111])
				terminal_putchar(kp_char[in & 0x7f]);

			/* special function key */
			switch (in & 0xff)
			{
				case del:		terminal_del(); break;

				case f1:		switch_term(0); break;
				case f2:		switch_term(1); break;
				case f3:		switch_term(2); break;
				case f4:		switch_term(3); break;
				case f5:		switch_term(4); break;
				case f6:		switch_term(5); break;
				case f7:		switch_term(6); break;
				case f8:		switch_term(7); break;

				case extended:
					switch ((in >> 8) & 0xff)
					{
						case up:	terminal_cursor_move(0); break;
						case down:	terminal_cursor_move(1); break;
						case left:	terminal_cursor_move(2); break;
						case right:	terminal_cursor_move(3); break;
					}
				break;

				// case esc: asm("hlt"); break;
			}
		}
	}
}
