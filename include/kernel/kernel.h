#pragma once

#define KERNEL_NAME "FeudOS kernel"
#define KERNEL_VER  "v0.0.3"

#if defined(__x86_64__)
    #define KERNEL_ARCH "x86_64" 
#endif // currently the only arch supported and idk whether i would add more 
       // or not, but will suppose its somewhat possible

[[noreturn]]
void kernel_panic(const char *const msg);

void kprintf(const char* msg, ...);
