#pragma once

#define KERNEL_NAME "FeudOS kernel"
#define KERNEL_VER  "v0.0.1"

[[noreturn]]
void kernel_panic(const char *const msg);

void kprintf(const char* msg, ...);
