#include <stdint.h>
#include "kernel/timer.h"

static volatile uint64_t ticks = 0;
static uint32_t timer_freq = TIMER_FREQ; 

void timer_tick() {
    ticks += 1;
}

uint64_t timer_get_ticks() {
    return (ticks * 1000) / timer_freq;
}

void sleep_ticks(uint64_t n) {
    uint64_t target = ticks + n;
    while (ticks < target) {
        __asm__ __volatile__("hlt");
    }
} 
