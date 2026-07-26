#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "kernel/kernel.h"
#include "kernel/keyboard.h"
#include "kernel/ksh/ksh.h"
#include "kernel/ksh/handlers.h"
#include "kernel/log.h"
#include "kernel/timer.h"
#include "kernel/vga.h"
#include "arch/init_arch.h"
#include "util/vga_colors.h"

[[noreturn]]
void kernel_panic(const char* msg, ...) {
    vga_print("Kernel panic!\n", PANIC_COLOR);
    kprintf("\t%k", PANIC_COLOR);

    va_list args;
    va_start(args, msg);
    
    kvprintf(msg, args);

    va_end(args);

    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}

void kernel_main() {
    vga_clear_screen(DEF_TEXT_COL);

    klog(NONE, "Booting up %s %s for %s ...\n", 
        KERNEL_NAME, KERNEL_VER, KERNEL_ARCH
    );

    arch_init();

    ksh_run();

    for (;;) {
        __asm__ __volatile__ ("hlt");
    }
}
