#pragma once

#define KERNEL_NAME "FeudOS kernel"
#define KERNEL_VER  "v0.0.4"

#if defined(__x86_64__)
    #define KERNEL_ARCH "x86_64" 
#endif // currently the only arch supported and idk whether i would add more 
       // or not, but will suppose its somewhat possible

#include "util/vga_colors.h"
#define PANIC_COLOR  VGA_RED
#define DEF_TEXT_COL VGA_WHITE

[[noreturn]]
void kernel_panic(const char* msg, ...);

