#pragma once

#include <stdint.h>
#define TIMER_FREQ 100

void timer_tick();
uint64_t timer_get_ticks();
uint64_t timer_uptime_ms();
void sleep_ticks(uint64_t n);
