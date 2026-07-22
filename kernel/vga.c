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

static void kprint_int(int num, uint8_t color) {
    bool minus = num < 0;
    if (minus) {
        num = -num;
    }
    
    char buf[11];

    int i = 0; 
    while (num != 0) {
        int digit = num % 10;
        num /= 10;
        const char d = dtoa((char)digit);
        buf[i] = d;
        i += 1;
    }

    if (minus) {
        vga_putc('-', color);
    }

    for (int j = i - 1; j >= 0; j--) {
        vga_putc(buf[j], color);
    }
}

void kprintf(const char* msg, ...) {
    va_list args;
    const uint8_t white_on_black = 0x0F;

    va_start(args, msg);

    bool skip = false;
    for (size_t i = 0; msg[i] != '\0'; i++) {
        if (skip) {
            skip = false;
            continue;
        }

        char prev = i == 0 ? 0 : msg[i-1];
        char cur  = msg[i];

        if (cur == '%' && prev != '\\') {
            char next = msg[i+1];
            if (next == '\0') {
                break;
            } 
            
            switch (next) {
                case 'd': {
                    int num = va_arg(args, int);
                    kprint_int(num, white_on_black);
                    break;
                }
                case 's': {
                    const char* str = va_arg(args, const char*);
                    vga_print(str, white_on_black);
                    break;
                }
                default: 
                    break;
            }
            skip = true;
        } else {
            vga_putc(cur, white_on_black);
        }
    }

    va_end(args);
}
