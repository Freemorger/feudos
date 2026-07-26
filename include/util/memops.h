#pragma once

#include <stddef.h>

/// Copy n bytes from `src` to `dst`, `src` and `dst` mustn't overlap.   
/// Returns dst 
void* kmemcpy(void *restrict dst, const void *restrict src, size_t n);
