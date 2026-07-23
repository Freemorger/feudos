#include "arch/x86_64/idt.h"
#include "arch/x86_64/pic.h"
#include "kernel/timer.h"
#include "kernel/vga.h"

void arch_init() {
    idt_init();
    kprintf("[OK] Initialized IDT.\n");

    pic_remap();
    pit_init(TIMER_FREQ);
    pic_set_mask(0);

    pic_clear_mask(0);
    pic_clear_mask(1);
    kprintf("[OK] PIC initialization done.\n");
    __asm__ __volatile__("sti");
    kprintf("[OK] Interrupts enabled.\n");
}
