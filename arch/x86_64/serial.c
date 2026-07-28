#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

static int serial_tx_empty(void) { return inb(0x3F8 + 5) & 0x20; }

void serial_print(const char *s) {
    while (*s) {
        while (!serial_tx_empty());
        outb(0x3F8, *s++);
    }
}

void serial_print_hex(uint64_t val) {
    serial_print("0x");
    char buf[17];
    for (int i = 0; i < 16; i++) {
        uint8_t nib = (val >> ((15 - i) * 4)) & 0xF;
        buf[i] = nib < 10 ? '0' + nib : 'A' + nib - 10;
    }
    buf[16] = '\0';
    serial_print(buf);
}
