#include "arch/x86_64/idt.h"
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"
#include "kernel/kernel.h"
#include "kernel/timer.h"
#include "kernel/vga.h"
#include "kernel/keyboard.h"
#include <stdint.h>

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
        case 0x20: { // IRQ0 PIT Timer
            timer_tick(); 
            pic_send_eoi(0);
            break;
        }
        case 0x21: { // IRQ1 keyboard 
            uint8_t scancode = inb(0x60);
            keyboard_handle_scancode(scancode);
            pic_send_eoi(1);
            break;
        }
        default: {
            if (ctx->vector >= 0x20 && ctx->vector <= 0x2F) {
                pic_send_eoi(ctx->vector - 0x20);
            } else {
                kprintf("Unknown exception %lu at 0x%X\n", ctx->errc, ctx->rip);
            }
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
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();

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
    idt_set_gate(26, isr26);
    idt_set_gate(27, isr27);
    idt_set_gate(28, isr28);
    idt_set_gate(29, isr29);
    idt_set_gate(30, isr30);
    idt_set_gate(31, isr31);
    idt_set_gate(32, isr32);
    idt_set_gate(33, isr33);
    idt_set_gate(34, isr34);
    idt_set_gate(35, isr35);
    idt_set_gate(36, isr36);
    idt_set_gate(37, isr37);
    idt_set_gate(38, isr38);
    idt_set_gate(39, isr39);
    idt_set_gate(40, isr40);
    idt_set_gate(41, isr41);
    idt_set_gate(42, isr42);
    idt_set_gate(43, isr43);
    idt_set_gate(44, isr44);
    idt_set_gate(45, isr45);
    idt_set_gate(46, isr46);
    idt_set_gate(47, isr47);

    struct idt_ptr ptr = {
        .limit = sizeof(idt) - 1,
        .base = (uint64_t)idt,
    };

    __asm__ volatile("lidt %0" : : "m"(ptr));
}
