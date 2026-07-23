// arch/x86_64/pic.c
#include "arch/x86_64/pic.h"
#include "arch/x86_64/io.h"

#define ICW1_ICW4      0x01
#define ICW1_INIT      0x10
#define ICW4_8086      0x01
#define PIC_EOI        0x20

void pic_remap(void) {
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4); io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4); io_wait();

    outb(PIC1_DATA, PIC1_OFFSET); io_wait();
    outb(PIC2_DATA, PIC2_OFFSET); io_wait();

    outb(PIC1_DATA, 0x04); io_wait(); // 0000 0100 = slave on IRQ2
    outb(PIC2_DATA, 0x02); io_wait(); // slave's cascade identity = 2

    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

void pic_set_mask(uint8_t irq_line) {
    uint16_t port = irq_line < 8 ? PIC1_DATA : PIC2_DATA;
    uint8_t line = irq_line < 8 ? irq_line : irq_line - 8;
    uint8_t value = inb(port) | (1 << line);
    outb(port, value);
}

void pic_clear_mask(uint8_t irq_line) {
    uint16_t port = irq_line < 8 ? PIC1_DATA : PIC2_DATA;
    uint8_t line = irq_line < 8 ? irq_line : irq_line - 8;
    uint8_t value = inb(port) & ~(1 << line);
    outb(port, value);
}

void pit_init(uint32_t freq) {
    uint32_t divisor = 1193180 / freq;
    outb(0x43, 0x36); 
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}
