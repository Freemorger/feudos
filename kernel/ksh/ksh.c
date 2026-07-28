#include "kernel/kernel.h"
#include "kernel/keyboard.h"
#include "kernel/timer.h"
#include "kernel/vga.h"
#include <stddef.h>
#include <stdint.h>
#include "kernel/ksh/ksh.h"
#include "kernel/ksh/handlers.h"
#include "util/str.h"

static struct ksh_cmd ksh_handlers[KSH_CMDS_COUNT];

void ksh_populate_base_cmds() {
    ksh_handlers[0] = (struct ksh_cmd){
        .name = "uptime",
        .func = ksh_hndl_uptime,
        .help = "Prints how long machine been running",
    };

    ksh_handlers[1] = (struct ksh_cmd){
        .name = "cpuid",
        .func = ksh_hndl_cpuid,
        .help = "Prints some info about this machine's CPU",
    };

    ksh_handlers[2] = (struct ksh_cmd){
        .name = "clear",
        .func = ksh_hndl_clear,
        .help = "Clears the screen",
    };

    ksh_handlers[3] = (struct ksh_cmd){
        .name = "uname",
        .func = ksh_hndl_uname,
        .help = "Clears the screen",
    };

    ksh_handlers[4] = (struct ksh_cmd){
        .name = "panic",
        .func = ksh_hndl_panic,
        .help = "Triggers a kernel panic with specified message.",
    };

    ksh_handlers[5] = (struct ksh_cmd){
        .name = "echo",
        .func = ksh_hndl_echo,
        .help = "Repeat the specified text."
    };

    ksh_handlers[6] = (struct ksh_cmd){
        .name = "help",
        .func = ksh_hndl_help,
        .help = "Print help message."
    };
}

struct ksh_cmd* ksh_get_handlers() {
    return ksh_handlers;
}

void ksh_run() {
    char line[128];

    ksh_populate_base_cmds();
    for (;;) {
        kprintf("ksh> ");

        size_t len = kbd_readline(line, sizeof(line));
        line[len - 1] = '\0'; // avoid newline 
        
        ksh_exec(line);
    }
}

bool ksh_exec(char* cmd) {
    char* args_start = strfnl(cmd, ' ');
    if (args_start == NULL) {
        args_start = "";
    }
    char* name_end   = strfle(cmd, ' ');
    *(name_end + 1)  = '\0';

    for (size_t i = 0; i < KSH_CMDS_COUNT; i++) {
        struct ksh_cmd* cur = &ksh_handlers[i];
        if (streq(cmd, cur->name)) {
            cur->func(args_start);
            return true;
        }
    }

    kprintf("Unknown command: %s\n", cmd);
    return false;
}
