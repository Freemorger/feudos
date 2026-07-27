#pragma once

#include <stdint.h>
#include <stdarg.h>

void vga_init();
void vga_clear_screen(uint8_t color);
void vga_putc(char c, uint8_t color);
void vga_print(const char* str, uint8_t color);
/// Update color of text for future chars to be printed via kprintf.
void vga_change_text_color(uint8_t col);
/// Set cursor pos to (X;Y)
void vga_move_cursor(uint16_t x, uint16_t y);
/// Kprintf but for va list 
void kvprintf(const char* msg, va_list args);
/// Print fmt to VGA; supported specifiers: %s %d %ld %lu %X, custom specs: %k (vga color)
void kprintf(const char* msg, ...);

