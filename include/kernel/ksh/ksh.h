#pragma once
// KSh - kernel shell

#define KSH_CMDS_COUNT 7

struct ksh_cmd {
    const char* name;
    bool (*func)(const char* args);
    const char* help;
};

/// Run handle loop for KSh (kernel shell)
void ksh_run();
/// Executes command; mutates `cmd` by changing the first whitespace after cmd name to null term for parsing
bool ksh_exec(char* cmd);
struct ksh_cmd* ksh_get_handlers();
