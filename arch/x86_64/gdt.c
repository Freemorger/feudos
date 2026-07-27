#include <stdint.h>

struct [[gnu::packed]] gdt_entry {
    uint64_t raw;
};

struct [[gnu::packed]] gdt_ptr {
    uint16_t limit;
    uint64_t base;
};

static struct gdt_entry gdt[3];
static struct gdt_ptr gdtp;

extern void gdt_flush(uint64_t gdtp_addr); // arch/x86_64/gdt_flush.s

void gdt_init() {
    gdt[0].raw = 0;                                              // null
    gdt[1].raw = (1ULL<<43) | (1ULL<<44) | (1ULL<<47) | (1ULL<<53); // code, present, long mode
    gdt[2].raw = (1ULL<<41) | (1ULL<<44) | (1ULL<<47);              // data, present, writable

    gdtp.limit = sizeof(gdt) - 1;
    gdtp.base  = (uint64_t)&gdt;

    gdt_flush((uint64_t)&gdtp);
}
