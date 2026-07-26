#include "kernel/kernel.h"
#include "kernel/ksh/ksh.h"
#include "kernel/timer.h"
#include "kernel/vga.h"
#include "util/str.h"
#include <stddef.h>

bool ksh_hndl_uptime(const char* args) {
    uint64_t ms_up = timer_uptime_ms();
            
    uint64_t total_seconds = ms_up / 1000;

    uint64_t h_up = total_seconds / 3600;
    uint64_t m_up = (total_seconds / 60) % 60;
    uint64_t s_up = total_seconds % 60;

    kprintf("up %lu:%lu:%lu\n", h_up, m_up, s_up);
    return true;
}

bool ksh_hndl_cpuid(const char* args) {
    unsigned int eax, ebx, ecx, edx;
    __asm__ __volatile__ (
        "cpuid" :
        "=a" (eax),
        "=b" (ebx),
        "=c" (ecx), 
        "=d" (edx)
        : "a" (0)
    );

    char vendor[13];
    *(unsigned int*)&vendor[0] = ebx;
    *(unsigned int*)&vendor[4] = edx;
    *(unsigned int*)&vendor[8] = ecx;
    vendor[12] = '\0';

    kprintf("CPU vendor: %s\n", vendor);

    char brand[48] = {0};

    for (unsigned int leaf = 0x80000002; leaf <= 0x80000004; leaf++) {
        __asm__ __volatile__ (
            "cpuid" :
            "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) :
            "a" (leaf)
        );

        unsigned int base = (leaf - 0x80000002) * 16;
        *(unsigned int*)&brand[base + 0]  = eax;
        *(unsigned int*)&brand[base + 4]  = ebx;
        *(unsigned int*)&brand[base + 8]  = ecx;
        *(unsigned int*)&brand[base + 12] = edx;
    }

    kprintf("CPU model: %s\n", brand);
    kprintf("Features: ");
    // In future, might be better to use cpufeatures structs or smth

    __asm__ __volatile__ (
        "cpuid" :
        "=a" (eax),
        "=b" (ebx),
        "=c" (ecx), 
        "=d" (edx)
        : "a" (1)
    );

    const size_t FEATURES_COUNT = 6;
    bool features[FEATURES_COUNT];
    const size_t FEATURE_NAME_LEN = 7;
    char feature_names[FEATURES_COUNT][FEATURE_NAME_LEN];

    strncpy_f(feature_names[0], "SSE3",   FEATURE_NAME_LEN);
    strncpy_f(feature_names[1], "SSE4.1", FEATURE_NAME_LEN);
    strncpy_f(feature_names[2], "SSE4.2", FEATURE_NAME_LEN);
    strncpy_f(feature_names[3], "APIC",   FEATURE_NAME_LEN);
    strncpy_f(feature_names[4], "AVX",    FEATURE_NAME_LEN);
    strncpy_f(feature_names[5], "AVX2",   FEATURE_NAME_LEN);

    features[0] = (ecx & (1u << 0)) ; // sse3 
    features[1] = (ecx & (1u << 19)); // sse4.1 
    features[2] = (ecx & (1u << 20)); // sse4.2 
    features[3] = (edx & (1u << 9)) ; // apic 
    features[4] = (ecx & (1u << 28)); // avx
                                     
    __asm__ __volatile__ (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(7), "c"(0)
    );

    features[5] = ebx & (1u << 5); // avx2 
   
    for (size_t i = 0; i < FEATURES_COUNT; i++) {
        kprintf(" %s%s ",
            features[i] ? "+" : "-",
            feature_names[i]
        );
    } 
    kprintf("\n");

    return true;
}

bool ksh_hndl_clear(const char* args) {
    vga_move_cursor(0, 0);
    vga_clear_screen(DEF_TEXT_COL);
    return true;
}

bool ksh_hndl_uname(const char* args) {
    kprintf("%s %s %s\n", KERNEL_NAME, KERNEL_VER, KERNEL_ARCH);
    return true;
}

bool ksh_hndl_panic(const char* args) {
    kernel_panic("%s\n", args);
    return true;
}

bool ksh_hndl_help(const char* args) {
    struct ksh_cmd* hndlrs = ksh_get_handlers();
    
    for (size_t i = 0; i < KSH_CMDS_COUNT; i++) {
        struct ksh_cmd hndl = hndlrs[i];
        kprintf("%s: %s\n", hndl.name, hndl.help);
    }

    return true;
}

bool ksh_hndl_echo(const char* args) {
    kprintf("%s\n", args);
    return true;
}
