#include <stdint.h>
#include <stddef.h>
#include "limine.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[3] = LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[4] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[2] = LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request kaddr_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
    .revision = 0
};



uint64_t hhdm_offset;
uint64_t kernel_phys_base;
uint64_t kernel_virt_base;

extern void kernel_main(void);
extern void map_page(uint64_t virt, uint64_t phys, uint64_t hhdm_offset);

static void hcf(void) {
    for (;;) {
        __asm__ __volatile__ ("cli; hlt");
    }
}

void _start(void) {
    if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision)) {
        hcf();
    }

    if (hhdm_request.response == NULL || kaddr_request.response == NULL) {
        hcf();
    }

    hhdm_offset      = hhdm_request.response->offset;
    kernel_phys_base = kaddr_request.response->physical_base;
    kernel_virt_base = kaddr_request.response->virtual_base;

    map_page(hhdm_offset + 0xB8000, 0xB8000, hhdm_offset);

    kernel_main();
    hcf();
}
