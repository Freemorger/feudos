#include <stdint.h>

extern uint64_t kernel_phys_base;
extern uint64_t kernel_virt_base;

static uint64_t pt_scratch[4][512] __attribute__((aligned(4096)));
static int pt_scratch_used = 0;

static inline uint64_t v2p(uint64_t vaddr) {
    return vaddr - kernel_virt_base + kernel_phys_base;
}

static uint64_t* alloc_table(void) {
    uint64_t *t = pt_scratch[pt_scratch_used++];
    for (int i = 0; i < 512; i++) t[i] = 0;
    return t;
}

void map_page(uint64_t virt, uint64_t phys, uint64_t hhdm_offset) {
    uint64_t pml4_i = (virt >> 39) & 0x1FF;
    uint64_t pdpt_i = (virt >> 30) & 0x1FF;
    uint64_t pd_i   = (virt >> 21) & 0x1FF;
    uint64_t pt_i   = (virt >> 12) & 0x1FF;

    uint64_t cr3;
    __asm__ __volatile__("mov %%cr3, %0" : "=r"(cr3));
    uint64_t *pml4 = (uint64_t*)(cr3 + hhdm_offset);

    if (!(pml4[pml4_i] & 1)) pml4[pml4_i] = v2p((uint64_t)alloc_table()) | 0x3;
    uint64_t *pdpt = (uint64_t*)((pml4[pml4_i] & ~0xFFFULL) + hhdm_offset);

    if (!(pdpt[pdpt_i] & 1)) pdpt[pdpt_i] = v2p((uint64_t)alloc_table()) | 0x3;
    uint64_t *pd = (uint64_t*)((pdpt[pdpt_i] & ~0xFFFULL) + hhdm_offset);

    if (!(pd[pd_i] & 1)) pd[pd_i] = v2p((uint64_t)alloc_table()) | 0x3;
    uint64_t *pt = (uint64_t*)((pd[pd_i] & ~0xFFFULL) + hhdm_offset);

    pt[pt_i] = (phys & ~0xFFFULL) | 0x3;
    __asm__ __volatile__("invlpg (%0)" :: "r"(virt) : "memory");
}
