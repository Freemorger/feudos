#pragma once

#include <stdint.h>
void vga_clear_screen(uint8_t color);
void vga_putc(char c, uint8_t color);
void vga_print(const char* str, uint8_t color);
void kprintf(const char* msg, ...);
