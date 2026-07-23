#pragma once
#include <stdint.h>

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

#define PIC1_OFFSET 0x20  // IRQ0-7  -> vectors 0x20-0x27
#define PIC2_OFFSET 0x28  // IRQ8-15 -> vectors 0x28-0x2F

void pic_remap(void);
void pic_send_eoi(uint8_t irq);
void pic_set_mask(uint8_t irq_line);
void pic_clear_mask(uint8_t irq_line);
void pit_init(uint32_t freq);
