#include "gdt.h"

struct gdt * const	gdt = (struct gdt *)GDT_ADDR;

static void	create_segment(struct gdt_segment *seg, uint8_t ring, uint8_t type)
{
	seg->limit1 = 0xFFFF;
	seg->base1 = 0x0000;
	seg->base2 = 0x00;
	seg->access = ring | type;
	seg->flags_limit2 = 0xcf;
	seg->base3 = 0x00;
}

void		ft_bzero(void *ptr, uint32_t size)
{
	uint8_t	*buf;

	buf = (uint8_t *)ptr;
	for (uint32_t i = 0; i < size; ++i)
		buf[i] = 0x00;
}

void		init_gdt(void)
{
	ft_bzero(&(gdt->segment[0]), 8);
	create_segment(&(gdt->segment[1]), KERNEL_RING, CODE);
	create_segment(&(gdt->segment[2]), KERNEL_RING, DATA);
	create_segment(&(gdt->segment[3]), KERNEL_RING, STACK);

	create_segment(&(gdt->segment[4]), USER_RING, CODE);
	create_segment(&(gdt->segment[5]), USER_RING, DATA);
	create_segment(&(gdt->segment[6]), USER_RING, STACK);

	gdt->descriptor.limit = sizeof(struct gdt_segment) * 7 - 1; // minus one because it must points to the least valid byte
	gdt->descriptor.base = (uint32_t) &(gdt->segment[0]);

	load_gdt(&(gdt->descriptor));
	enable_protected_mode();
}
