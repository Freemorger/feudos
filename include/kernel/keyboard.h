#pragma once

#include <stddef.h>
#include <stdint.h>

#define KBD_BUF_SIZE 2048
// TODO: maybe create a macro for wrapping buffers generic instead
/// Wrapping buffer for keyboard 
struct kbd_buffer {
    char   buf[KBD_BUF_SIZE];
    size_t pos; /// next char idx 
};

void kbd_buffer_push(char c);

void  kbd_handle_scancode(uint8_t sc);
char* kbd_get_buf_ptr();
/// Awaits user to write 1 line, then copies it into `buf`. Reserves last byte for null term.
/// Returns copied amount (with null term counted).
size_t kbd_readline(char* buf, size_t max);
