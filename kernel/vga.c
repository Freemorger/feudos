#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "kernel/vga.h"
#include "arch/x86_64/io.h"
#include "util/ascii_util.h"
#include "limine.h"
#include "font8x8_basic.h"

extern struct limine_framebuffer *g_framebuffer;

#define GLYPH_W 8
#define GLYPH_H 8
#define MAX_ROWS 256

static volatile uint32_t* fb;
static uint64_t pitch_px; // pitch in pixels (fb->pitch is in bytes)
static int VGA_WIDTH;     // columns of 8px glyphs, computed at init
static int VGA_HEIGHT;    // rows of 8px glyphs, computed at init

static const uint32_t vga_palette[16] = {
    0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
    0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
    0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
    0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF,
};

static inline uint32_t fg_rgb(uint8_t color) { 
    return vga_palette[color & 0x0F]; 
}
static inline uint32_t bg_rgb(uint8_t color) { 
    return vga_palette[(color >> 4) & 0x0F]; 
}

void vga_init(void) {
    fb = (volatile uint32_t*)g_framebuffer->address;
    pitch_px  = g_framebuffer->pitch / 4;
    VGA_WIDTH  = g_framebuffer->width  / GLYPH_W;
    VGA_HEIGHT = g_framebuffer->height / GLYPH_H;
    if (VGA_HEIGHT > MAX_ROWS) VGA_HEIGHT = MAX_ROWS;
}

// should be changed with `vga_move_cursor` when possible.
static uint16_t cursor_row = 0;
static uint16_t cursor_column = 0;
static uint8_t line_len[MAX_ROWS]; // chars actually written on each row, for correct '\b' across lines

static uint8_t text_color = 0x0F; // white on black by default
void vga_change_text_color(uint8_t col) {
    text_color = col;
}

static void draw_cell(int col, int row, char c, uint8_t color) {
    uint32_t fg = fg_rgb(color);
    uint32_t bg = bg_rgb(color);
    const unsigned char *glyph = 
        (const unsigned char *)font8x8_basic[(unsigned char)c];

    int px0 = col * GLYPH_W;
    int py0 = row * GLYPH_H;

    for (int y = 0; y < GLYPH_H; y++) {
        unsigned char bits = glyph[y];
        for (int x = 0; x < GLYPH_W; x++) {
            fb[(py0 + y) * pitch_px + (px0 + x)] = (bits & (1 << x)) ? fg : bg;
        }
    }
}

static void clear_cell(int col, int row, uint8_t color) {
    draw_cell(col, row, ' ', color);
}

void vga_clear_screen(uint8_t color) {
    uint32_t bg = bg_rgb(color);
    for (uint64_t y = 0; y < g_framebuffer->height; y++) {
        for (uint64_t x = 0; x < g_framebuffer->width; x++) {
            fb[y * pitch_px + x] = bg;
        }
    }
    for (int i = 0; i < VGA_HEIGHT; i++) line_len[i] = 0;
    vga_move_cursor(0, 0);
}


void vga_set_cursor(uint16_t x, uint16_t y) {
    // TODO
    (void)x;
    (void)y;
}

void vga_move_cursor(uint16_t x, uint16_t y) {
    cursor_row    = y;
    cursor_column = x;

    vga_set_cursor(cursor_column, cursor_row);
}

static void vga_up() {
    for (int y = GLYPH_H; y < VGA_HEIGHT * GLYPH_H; y++) {
        for (int x = 0; x < VGA_WIDTH * GLYPH_W; x++) {
            fb[(y - GLYPH_H) * pitch_px + x] = fb[y * pitch_px + x];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++) {
        clear_cell(x, VGA_HEIGHT - 1, 0x07);
    }

    for (int r = 1; r < VGA_HEIGHT; r++) line_len[r - 1] = line_len[r];
    line_len[VGA_HEIGHT - 1] = 0;
}

void vga_putc(const char c, uint8_t color) {
    uint16_t new_cursor_column = cursor_column;
    uint16_t new_cursor_row    = cursor_row;

    if (c <= 31) {
        switch (c) {
            case '\n': {
                new_cursor_column =  0;
                new_cursor_row    += 1;

                if (new_cursor_row >= VGA_HEIGHT) {
                    vga_up();
                    new_cursor_row = VGA_HEIGHT - 1;
                }
                vga_move_cursor(new_cursor_column, new_cursor_row);
                return;
            }
            case '\t': {
                new_cursor_column += 2 - (cursor_column % 2);
                if (new_cursor_column >= VGA_WIDTH) {
                    new_cursor_row    += 1;
                    new_cursor_column =  0;

                    if (new_cursor_row >= VGA_HEIGHT) {
                        vga_up();
                        new_cursor_row = VGA_HEIGHT - 1;
                    }
                }
                vga_move_cursor(new_cursor_column, new_cursor_row);
                return;
            }
            case '\b': {
                if (cursor_column == 0) {
                    if (cursor_row == 0) {
                        return;
                    }
                    new_cursor_row    -= 1;
                    new_cursor_column = line_len[new_cursor_row];
                } else {
                    new_cursor_column -= 1;
                }
                clear_cell(new_cursor_column, new_cursor_row, 0);
                if (new_cursor_column < line_len[new_cursor_row]) {
                    line_len[new_cursor_row] = new_cursor_column;
                }
                vga_move_cursor(new_cursor_column, new_cursor_row);
                return;
            }
            default:
                return;
        }
    }

    draw_cell(cursor_column, cursor_row, c, color);
    if (cursor_column + 1 > line_len[cursor_row]) {
        line_len[cursor_row] = cursor_column + 1;
    }

    new_cursor_column += 1;

    if (new_cursor_column >= VGA_WIDTH) {
        new_cursor_row    += 1;
        new_cursor_column = 0;

        if (new_cursor_row >= VGA_HEIGHT) {
            vga_up();
            new_cursor_row = VGA_HEIGHT - 1;
        }
    }
    vga_move_cursor(new_cursor_column, new_cursor_row);
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

    char buf[20]; // enough for 64 bit unsigned int in decimal or hex 

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

void kvprintf(const char *msg, va_list args) {
    for (size_t i = 0; msg[i] != '\0'; i++) {
        if (msg[i] != '%') {
            vga_putc(msg[i], text_color);
            continue;
        }

        i += 1;                    // consume '%'
        if (msg[i] == '\0')
            break;

        switch (msg[i]) {
            case 'd': {
                kprint_int(va_arg(args, int), text_color, 10);
                break; 
            }
            case 'u': {
                kprint_uint(va_arg(args, unsigned int), text_color, 10);
                break;
            }
            case 'c': {
                char c = (char)va_arg(args, int);
                vga_putc(c, text_color);
                break;
            }
            case 's': {
                vga_print(va_arg(args, const char*), text_color);
                break;
            }
            case 'l': {
                char ahead = msg[++i];
                switch (ahead) {
                    case 'd': {
                        kprint_int(va_arg(args, long), text_color, 10);
                        break;
                    }
                    case 'u': {
                        kprint_uint(va_arg(args, unsigned long), text_color, 10);
                        break;
                    }
                    default: {
                        vga_print("%l", text_color);
                        vga_putc(ahead, text_color);
                        break;
                    }
                }
                break;
            }
            case 'X': {
                kprint_uint(va_arg(args, unsigned long), text_color, 16);
                break;
            }
            case 'k': { // custom fmt specifier for color
                vga_change_text_color((uint8_t)va_arg(args, int));
                break;
            }
            default: {
                vga_putc('%', text_color);
                vga_putc(msg[i], text_color);
                break;
            }
        }
    }
}

void kprintf(const char* msg, ...) {
    va_list args;

    va_start(args, msg);

    kvprintf(msg, args);

    va_end(args);
}
