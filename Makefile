ASM := nasm
CC  := x86_64-elf-gcc
LD  := x86_64-elf-gcc

CFLAGS := -ffreestanding  -O2 -Wall -Wextra -mno-red-zone -mcmodel=large -m64 -c -std=gnu23 -Iinclude/
LDFLAGS := -ffreestanding -O2 -nostdlib

# Source files
C_SRCS := $(wildcard kernel/*.c) \
          $(wildcard arch/x86_64/*.c) \
          $(wildcard util/*.c)

ASM_SRCS := $(wildcard arch/x86_64/*.s)

# Object files
C_OBJS   := $(C_SRCS:%.c=obj/%.o)
ASM_OBJS := $(ASM_SRCS:%.s=obj/%.o)

OBJS := $(C_OBJS) $(ASM_OBJS)

all: feudos.bin

# C compilation
obj/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# Assembly compilation
obj/%.o: %.s
	mkdir -p $(dir $@)
	$(ASM) -f elf64 $< -o $@

# Link kernel
feudos.bin: $(OBJS) linker.ld
	$(LD) -T linker.ld -o $@ $(LDFLAGS) $(OBJS) -lgcc

# ISO
iso: feudos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp feudos.bin isodir/boot/feudos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o feudos.iso isodir

run-iso: iso
	qemu-system-x86_64 -cdrom feudos.iso

clean:
	rm -rf obj feudos.bin feudos.iso isodir
