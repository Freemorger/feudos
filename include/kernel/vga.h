#pragma once

#include <stdint.h>
void vga_clear_screen(uint8_t color);
void vga_putc(char c, uint8_t color);
void vga_print(const char* str, uint8_t color);
/// Set cursor pos to (X;Y)
void vga_move_cursor(uint16_t x, uint16_t y);
/// Print fmt to VGA, supported specifiers: %s %d %ld %lu %X
void kprintf(const char* msg, ...);
