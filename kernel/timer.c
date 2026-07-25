#include <stdint.h>
#include "kernel/timer.h"

static volatile uint64_t ticks = 0;
static uint32_t timer_freq = TIMER_FREQ; 

void timer_tick(void) {
    ticks++;
}

uint64_t timer_get_ticks(void) {
    return ticks;
}

uint64_t timer_uptime_ms(void) {
    return (ticks * 1000) / TIMER_FREQ;
}

#if defined(__x86_64__) | defined (__i386__)
// Hlt is x86 thingy, so this implementation is x86 only
void sleep_ticks(uint64_t n) {
    uint64_t target = ticks + n;
    while (ticks < target) {
        __asm__ __volatile__("hlt");
    }
}

#endif
