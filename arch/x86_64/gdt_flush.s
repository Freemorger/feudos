bits 64
section .text
global gdt_flush
gdt_flush:
    lgdt [rdi]

    push 0x08
    lea rax, [rel .reload_cs]
    push rax
    o64 retf              ; explicit 64-bit-operand far return
.reload_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
