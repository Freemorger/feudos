#pragma once
#include <stddef.h>

bool   streq(const char* a, const char* b);
/// Str length
size_t strl (const char* a);
bool isalnum(unsigned char c);
/// Find current lexeme end in a separator-delimited string.  
/// Returns a pointer to the end of current lexeme,
/// or `a` if it starts with `sep`, or end of string
char* strfle(const char* a, char sep);
/// Find next lexeme in a separator-delimited string.
/// Returns a pointer to the beginning of the next lexeme,
/// or NULL if there is no next lexeme.
char*  strfnl(const char* a, char sep);
/// Copies <= `max` bytes from `src` to `dst`, `src` and `dst` shouldn't overlap.  
/// Pretty much like libc strncpy.. Though to avoid lsp/whatever naming conflict issues, it has suffix _f
/// Returns `dst`
char* strncpy_f(char *restrict dst, const char *restrict src, size_t max);
