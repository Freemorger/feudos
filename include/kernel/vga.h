#pragma once

#include <stdint.h>
void vga_clear_screen(uint8_t color);
void vga_putc(char c, uint8_t color);
void vga_print(const char* str, uint8_t color);
/// Print fmt to VGA, supported specifiers: %s %d %ld %lu %X
void kprintf(const char* msg, ...);
