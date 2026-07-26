#include "util/str.h"
#include <stddef.h>

bool   streq(const char* a, const char* b) {
    size_t i = 0;
    for (; a[i] != '\0' && b[i] != '\0'; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return a[i] == '\0' && b[i] == '\0';
}

size_t strl(const char *a) {
    size_t c = 0;
    while (a[c] != '\0') {
        c += 1;
    }
    return c;
}

bool isalnum(unsigned char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

/// Find current lexeme end in a separator-delimited string.  
/// Returns a pointer to the end of current lexeme,
/// or `a` if it starts with `sep`, or end of string
char* strfle(const char* a, char sep) {
    const char* cur = a;
    for (; *cur; cur++) {
        if (*cur == sep) {
            if (cur == a) {
                return a;
            }
            return cur-1;
        }
    }
    return cur-1;
}

/// Find next lexeme in a separator-delimited string.
/// Returns a pointer to the beginning of the next lexeme,
/// or NULL if there is no next lexeme.
char* strfnl(const char* a, char sep) {
    bool ended = false;

    for (const char* cur = a; *cur; cur++) {
        if (*cur == sep) {
            ended = true;
            continue;
        }
        if (ended && *cur != sep) {
            return cur;
        }
    }

    return NULL;
}

char* strncpy_f(char *restrict dst, const char *restrict src, size_t max) {
    size_t i = 0;
    for (; i < max && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }
    while (i < max) {
        dst[i] = '\0';
        i += 1;
    }

    return dst;
}

