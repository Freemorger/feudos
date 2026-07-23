#include <stdint.h>
#include <stdbool.h>
#include "kernel/kernel.h"

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

void keyboard_handle_scancode(uint8_t sc) {
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
            // TODO: push char to some buf maybe
            kprintf("%c", c);
        }
    }
}
