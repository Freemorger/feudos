section .text
bits 64

%macro ISR_NOERRC 1
global isr%1
isr%1:
    push 0
    push %1
    jmp isr_common
%endmacro

%macro ISR_ERR 1 
global isr%1 
isr%1:
    push %1 
    jmp isr_common 
%endmacro 

ISR_NOERRC 0
ISR_NOERRC 1
ISR_NOERRC 2
ISR_NOERRC 3
ISR_NOERRC 4
ISR_NOERRC 5
ISR_NOERRC 6
ISR_NOERRC 7
ISR_ERR 8
ISR_NOERRC 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NOERRC 15
ISR_NOERRC 16
ISR_ERR 17
ISR_NOERRC 18
ISR_NOERRC 19
ISR_NOERRC 20
ISR_ERR 21
ISR_NOERRC 22
ISR_NOERRC 23
ISR_NOERRC 24
ISR_NOERRC 25
ISR_NOERRC 26
ISR_NOERRC 27
ISR_NOERRC 28
ISR_NOERRC 29
ISR_NOERRC 30
ISR_NOERRC 31
; IRQs (PIC remapped to 0x20-0x2F)
ISR_NOERRC 32   ; IRQ0 - timer
ISR_NOERRC 33   ; IRQ1 - keyboard
ISR_NOERRC 34
ISR_NOERRC 35
ISR_NOERRC 36
ISR_NOERRC 37
ISR_NOERRC 38
ISR_NOERRC 39
ISR_NOERRC 40
ISR_NOERRC 41
ISR_NOERRC 42
ISR_NOERRC 43
ISR_NOERRC 44
ISR_NOERRC 45
ISR_NOERRC 46
ISR_NOERRC 47

%macro PUSH_REGS 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro POP_REGS 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

extern interrupt_dispatch
isr_common:
    PUSH_REGS 

    mov rdi, rsp 
    call interrupt_dispatch

    POP_REGS
    add rsp, 16      ; discard vector + errc 

    iretq
