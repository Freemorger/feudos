ENTRY(_start)

SECTIONS
{
    /* GRUB loads kernels at or above 1 MiB by convention -- addresses
     * below that are reserved for real-mode stuff (BIOS, bootloaders). */
    . = 1M;

    .boot :
    {
        *(.multiboot)
    }

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}
