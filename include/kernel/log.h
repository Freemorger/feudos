#pragma once

enum LogLevel {
    NONE,

    INFO,
    WARN,
    ERROR,
    CRITICAL
};

/// Return string name of loglevel (or empty string for unknown) 
const char* klogl_str(enum LogLevel l);
/// Log message with kprintf fmt.
void klog(enum LogLevel l, const char* fmt, ...);
