#include <stdint.h>
#include <stddef.h>
#include "kernel/kernel.h"
#include "kernel/vga.h"

#define PANIC_COLOR 0x04

[[noreturn]]
void kernel_panic(const char *const msg) {
    vga_print("Kernel panic!\n", PANIC_COLOR);
    vga_putc('\t', 0);
    vga_print(msg, PANIC_COLOR);

    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}

void kernel_main() {
    const uint8_t white_on_black = 0x0F;
    vga_clear_screen(white_on_black);

    vga_print("Booting up " KERNEL_NAME " " KERNEL_VER " ...\n", white_on_black);

    for (;;) {
        __asm__ __volatile__ ("hlt");
    }
}
