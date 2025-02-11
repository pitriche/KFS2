ENTRY(start)

SECTIONS
{
    . = 1M;

    /* Multiboot header must appear in the first 8KiB. Keep it here at the front. */
    .boot ALIGN(4):
    {
        KEEP(*(.mb_header))
    }

    /* Code (text) section */
    .text ALIGN(4):
    {
        *(.text)       /* Main .text */
    }

    /* Read-only data (rodata). */
    .rodata ALIGN(4):
    {
        *(.rodata)
    }

    /* Data section (initialized data). */
    .data ALIGN(4):
    {
        *(.data)
    }

    /* BSS section (uninitialized data). Include COMMON for uninitialized globals. */
    .bss ALIGN(4):
    {
        *(COMMON)
        *(.bss)
    }

}
