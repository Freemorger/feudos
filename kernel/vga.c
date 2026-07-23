#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "kernel/vga.h"
#include "util/ascii_util.h"

static volatile uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
static const int VGA_WIDTH  = 80;
static const int VGA_HEIGHT = 25;

static inline uint16_t vga_entry(uint8_t c, uint8_t color) {
    return (uint16_t)c | (uint16_t)(color << 8);
}

void vga_clear_screen(uint8_t color) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = vga_entry(' ', color);
    }
}

static size_t cursor_row = 0;
static size_t cursor_column = 0;

static void vga_up() {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(y-1) * VGA_WIDTH + x] =
                VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = 
                vga_entry(' ', 0x07);
    }
}

void vga_putc(const char c, uint8_t color) {
    if (c <= 31) {
        switch (c) {
            case '\n': {
                cursor_column = 0;
                cursor_row    += 1;

                if (cursor_row >= VGA_HEIGHT) {
                    vga_up();
                    cursor_row = VGA_HEIGHT - 1;
                }
                return;
            }
            case '\t': {
                cursor_column += 2 - (cursor_column % 2);
                if (cursor_column >= VGA_WIDTH) {
                    cursor_row    += 1;
                    cursor_column = 0;

                    if (cursor_row >= VGA_HEIGHT) {
                        vga_up();
                        cursor_row = VGA_HEIGHT - 1;
                    }
                }
                return;
            }
            default:
                return;
        }
        return;
    }

    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_column] = 
        vga_entry(c, color);

    cursor_column += 1;
    
    if (cursor_column >= VGA_WIDTH) {
        cursor_row += 1;
        cursor_column = 0;

        if (cursor_row >= VGA_HEIGHT) {
            vga_up();
            cursor_row = VGA_HEIGHT - 1;
        }
    }
}

void vga_print(const char* text, uint8_t color) {
    for (size_t i = 0; text[i] != '\0'; i++) {
        char cur_ch = text[i];
        vga_putc(cur_ch, color);
    }
}

/// Prints unsigned int with specified base; please don't use bases over 16!
static void kprint_uint(uintmax_t n, uint8_t color, uint8_t base) {
    if (n == 0) {
        vga_putc('0', color);
        return;
    } 

    char buf[20]; // enough for 64 bit unsigned int in decimal 

    int i = 0; 
    while (n != 0) {
        int digit = n % base;
        n /= base;
        const char d = dtoa((char)digit);
        buf[i] = d;
        i += 1;
    }

    for (int j = i - 1; j >= 0; j--) {
        vga_putc(buf[j], color);
    }
}

static void kprint_int(intmax_t num, uint8_t color, uint8_t base) {
    if (num < 0) {
        vga_putc('-', color);
        kprint_uint(-(uintmax_t)num, color, base);
    } else {
        kprint_uint((uintmax_t)num, color, base);
    }
}

void kprintf(const char* msg, ...) {
    va_list args;
    const uint8_t color = 0x0F;

    va_start(args, msg);

    for (size_t i = 0; msg[i] != '\0'; i++) {
        if (msg[i] != '%') {
            vga_putc(msg[i], color);
            continue;
        }

        i++;                    // consume '%'
        if (msg[i] == '\0')
            break;

        switch (msg[i]) {
            case 'd': {
                kprint_int(va_arg(args, int), color, 10);
                break; 
            }
            case 's': {
                vga_print(va_arg(args, const char*), color);
                break;
            }
            case 'l': {
                char ahead = msg[++i];
                switch (ahead) {
                    case 'd': {
                        kprint_int(va_arg(args, long), color, 10);
                        break;
                    }
                    case 'u': {
                        kprint_uint(va_arg(args, unsigned long), color, 10);
                        break;
                    }
                    default: {
                        vga_print("%l", color);
                        vga_putc(ahead, color);
                        break;
                    }
                }
                break;
            }
            case 'X': {
                kprint_uint(va_arg(args, unsigned long), color, 16);
                break;
            }
            default: {
                vga_putc('%', color);
                vga_putc(msg[i], color);
                break;
            }
        }
    }

    va_end(args);
}
