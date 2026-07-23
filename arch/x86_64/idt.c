#include "arch/x86_64/idt.h"
#include "kernel/kernel.h"
#include "kernel/vga.h"

volatile struct idt_entry idt[256];

#define KPANIC_DBG(ctx) \
    kprintf("RIP    = 0x%X\n", (ctx)->rip);\
    kprintf("CS     = 0x%X\n", (ctx)->cs);\
    kprintf("RFLAGS = 0x%X\n", (ctx)->rflags)


void interrupt_dispatch(struct idt_ctx* ctx) {
    switch (ctx->vector) {
        case 0x0: { // #DE division by zero 
            KPANIC_DBG(ctx); 
            kernel_panic("Unhandled division by zero.\n");
            break;
        }
        case 0x03: { // #BP debug breakpoint
            kprintf("[DEBUG] Reached breakpoint at RIP: 0x%X\n", ctx->rip);
            break;
        } 
        case 0x06: {
            KPANIC_DBG(ctx);
            kernel_panic("Illegal instruction.");
            break;
        }
        case 0x08: { // #DF double fault 
            kprintf("Double fault! Unrecoverable CPU state.\n"); 
            break;
        }
        case 0x0D: { // #GP general protection fault 
            KPANIC_DBG(ctx);
            kprintf("General Protection fault; GPF err code: 0x%X\n", ctx->errc);
            break;
        }
        case 0x0E: { // #PF page fault
            KPANIC_DBG(ctx);
            unsigned long cr2;
            __asm__ __volatile__("mov %%cr2, %0" : "=r"(cr2));
            kprintf("Page Fault accessing 0x%X\n", cr2);
            kprintf("Error code (flags): 0x%X\n", ctx->errc);
            kernel_panic("Page fault.\n");
            break;
        }
        default: {
            kprintf("Unknown exception %lu at 0x%X\n", ctx->errc, ctx->rip);
            break;
        }
    } 
}

static void idt_set_gate(int vec, void (*handler)(void)) {
    uint64_t addr = (uint64_t)handler;

    idt[vec].offset_low  = addr & 0xFFFF;
    idt[vec].selector    = 0x08;   // kernel code segment
    idt[vec].ist         = 0;
    idt[vec].type_attr   = 0x8E;   // present, DPL=0, interrupt gate
    idt[vec].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[vec].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[vec].zero        = 0;
}

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();

void idt_init(void) {
    idt_set_gate(0, isr0);
    idt_set_gate(1, isr1);
    idt_set_gate(2, isr2);
    idt_set_gate(3, isr3);
    idt_set_gate(4, isr4);
    idt_set_gate(5, isr5);
    idt_set_gate(6, isr6);
    idt_set_gate(7, isr7);
    idt_set_gate(8, isr8);
    idt_set_gate(9, isr9);
    idt_set_gate(10, isr10);
    idt_set_gate(11, isr11);
    idt_set_gate(12, isr12);
    idt_set_gate(13, isr13);
    idt_set_gate(14, isr14);
    idt_set_gate(15, isr15);
    idt_set_gate(16, isr16);
    idt_set_gate(17, isr17);
    idt_set_gate(18, isr18);
    idt_set_gate(19, isr19);
    idt_set_gate(20, isr20);
    idt_set_gate(21, isr21);
    idt_set_gate(22, isr22);
    idt_set_gate(23, isr23);
    idt_set_gate(24, isr24);
    idt_set_gate(25, isr25);

    struct idt_ptr ptr = {
        .limit = sizeof(idt) - 1,
        .base = (uint64_t)idt,
    };

    __asm__ volatile("lidt %0" : : "m"(ptr));
}
