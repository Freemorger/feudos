#include "kernel/keyboard.h"
#include <stdint.h>
#include <stdbool.h>
#include "kernel/vga.h"
#include "kernel/keyboard.h"
#include "util/memops.h"

static struct kbd_buffer kbd_buf = {0};

static bool shift_held = false;

static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ', 0,
    // 0x3B onward todo later
};

static const char scancode_ascii_shift[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' ', 0,
};

#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36

static char kbd_buf_pop() {
    if (kbd_buf.pos == 0) {
        return '\0';
    }

    kbd_buf.pos = kbd_buf.pos - 1;

    char c = kbd_buf.buf[kbd_buf.pos];
    kbd_buf.buf[kbd_buf.pos] = '\0';

    return c;
}

void kbd_buffer_push(char c) {
    if (c == '\b') {
        kbd_buf_pop();
        return;
    }

    if (kbd_buf.pos == (sizeof(kbd_buf.buf) - 1)) {
        kbd_buf.pos = 0;
    }

    kbd_buf.buf[kbd_buf.pos] = c;
    kbd_buf.pos += 1;
}

void kbd_handle_scancode(uint8_t sc) {
    bool released = sc & 0x80;
    uint8_t code  = sc & 0x7F;

    if (code == SC_LSHIFT || code == SC_RSHIFT) {
        shift_held = !released;
        return;
    }

    if (released) {
        return;
    }

    if (code < 128) {
        char c = shift_held ? scancode_ascii_shift[code] : scancode_ascii[code];
        if (c) {
            kbd_buffer_push(c); 
            kprintf("%c", c);
        }
    }
}

char* kbd_get_buf_ptr() {
    return kbd_buf.buf;
}

size_t kbd_readline(char* buf, size_t max) {
    size_t start_pos = kbd_buf.pos;

    for (;;) {
        __asm__ __volatile__("hlt");

        if (
            kbd_buf.pos != start_pos &&
            kbd_buf.pos != 0 &&
            kbd_buf.buf[kbd_buf.pos - 1] == '\n'
        ) {
            break;
        }
    }

    size_t line_len = kbd_buf.pos - start_pos;

    if (line_len >= max) {
        line_len = max - 1;
    }

    kmemcpy(buf, &kbd_buf.buf[start_pos], line_len);
    buf[line_len] = '\0';

    return line_len;
}
