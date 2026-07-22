; multiboot1 header

MBALIGN  equ  1<<0
MEMINFO  equ  1<<1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; some stack 
section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

; page tables for long mode 
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096

; initial gdt
section .rodata
gdt64:
    dq 0                                      ; null descriptor 
.code_segment: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)  ; executable, code, present, long-mode(L bit)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64


section .text
bits 32
global _start
extern kernel_main
extern long_mode_start

_start:
    mov esp, stack_top
   
    call check_multiboot
    call check_cpuid
    call check_long_mode

    call set_up_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:long_mode_start   

    hlt

check_multiboot:
    cmp eax, 0x2BADB002       ; GRUB leaves this magic value in EAX
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp error

check_cpuid:
    ; Toggle the ID bit (bit 21) in EFLAGS. If it stays flipped, CPUID exists.
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode          ; extended function not supported - no long mode

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29         ; LM bit
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error

set_up_page_tables:
    ; PML4[0] -> PDPT
    mov eax, p3_table
    or eax, 0b11              ; present + writable
    mov [p4_table], eax

    ; PDPT[0] -> PD
    mov eax, p2_table
    or eax, 0b11
    mov [p3_table], eax

    ; Fill the 512 entries of the PD with 2 MiB pages, identity mapped:
    ; entry N maps virtual [N*2MiB, (N+1)*2MiB) -> physical [N*2MiB, ...)
    mov ecx, 0
.map_p2_table:
    mov eax, 0x200000         ; 2 MiB
    mul ecx
    or eax, 0b10000011        ; present + writable + huge page (2MiB)
    mov [p2_table + ecx * 8], eax
    inc ecx
    cmp ecx, 512
    jne .map_p2_table
    ret

enable_paging:
    ; Load CR3 with the address of the top-level page table (PML4)
    mov eax, p4_table
    mov cr3, eax

    ; Enable PAE (required for long mode)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set the Long Mode Enable (LME) bit in the EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging (activates long mode now that LME + PAE are set)
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret

; Prints "ERR: <code>" in white-on-red directly to VGA text memory, then halts.
error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    hlt

; --- 64-bit entry --------------------------------------------------------------
bits 64
global long_mode_start
long_mode_start:
    mov ax, 0 
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kernel_main
    hlt
