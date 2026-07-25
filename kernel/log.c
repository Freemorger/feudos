#include <stdarg.h>
#include "kernel/log.h"
#include "kernel/vga.h"

const char* klogl_str(enum LogLevel l) {
    switch (l) {
        case INFO: 
            return "OK";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case CRITICAL:
            return "CRITICAL";
        default:
            return "";
    }
}

void klog(enum LogLevel l, const char* fmt, ...) {
    const char* l_str = klogl_str(l);
    if (l != NONE) {
        kprintf("[%s] ", l_str);
    }

    va_list args;
    va_start(args, fmt);

    kvprintf(fmt, args);

    va_end(args);
}
