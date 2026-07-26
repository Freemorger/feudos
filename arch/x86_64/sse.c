#include <stdint.h>
void init_sse() {
    uint64_t cr0, cr4;

    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 &= ~(1UL << 2);   // EM = 0
    cr0 |=  (1UL << 1);   // MP = 1
    asm volatile("mov %0, %%cr0" :: "r"(cr0));

    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    cr4 |= (1UL << 9);    // OSFXSR
    cr4 |= (1UL << 10);   // OSXMMEXCPT
    asm volatile("mov %0, %%cr4" :: "r"(cr4));

    asm volatile("fninit");
}
