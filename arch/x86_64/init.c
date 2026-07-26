#include "arch/x86_64/idt.h"
#include "arch/x86_64/pic.h"
#include "kernel/timer.h"
#include "kernel/vga.h"
#include "kernel/log.h"
#include "arch/x86_64/sse.h"

void arch_init() {
    idt_init();
    klog(INFO, "Initialized IDT.\n");

    pic_remap();
    pit_init(TIMER_FREQ);
    pic_set_mask(0);

    pic_clear_mask(0);
    pic_clear_mask(1);
    klog(INFO, "PIC initialization done.\n");
    __asm__ __volatile__("sti");
    klog(INFO, "Interrupts enabled.\n");

    init_sse();
    klog(INFO, "SSE enabled.\n");
}
