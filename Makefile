ASM := nasm
CC  := x86_64-elf-gcc
LD  := x86_64-elf-gcc

CFLAGS := -ffreestanding -O2 -Wall -Wextra -mno-red-zone -mcmodel=large \
          -fno-pic -fno-pie -m64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 \
          -c -std=gnu23 -Iinclude/
LDFLAGS := -ffreestanding -O2 -nostdlib -static -no-pie -z max-page-size=0x1000

LIMINE_BRANCH := v11.x-binary

# Source files
C_SRCS := $(wildcard kernel/*.c) \
		  $(wildcard kernel/ksh/*.c) \
          $(wildcard arch/x86_64/*.c) \
          $(wildcard util/*.c)

ASM_SRCS := $(wildcard arch/x86_64/*.s)

# Object files
C_OBJS   := $(C_SRCS:%.c=obj/%.o)
ASM_OBJS := $(ASM_SRCS:%.s=obj/%.o)

OBJS := $(C_OBJS) $(ASM_OBJS)

all: feudos.bin

obj/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

obj/%.o: %.s
	mkdir -p $(dir $@)
	$(ASM) -f elf64 $< -o $@

feudos.bin: $(OBJS) linker.ld
	$(LD) -T linker.ld -o $@ $(LDFLAGS) $(OBJS) -lgcc

limine:
	git clone https://github.com/Limine-Bootloader/Limine.git --branch=$(LIMINE_BRANCH) --depth=1 $@

limine/limine: limine
	$(MAKE) -C limine

iso: feudos.bin limine.conf limine/limine
	rm -rf isodir
	mkdir -p isodir/boot/limine
	mkdir -p isodir/EFI/BOOT
	cp feudos.bin isodir/boot/feudos.bin
	cp limine.conf isodir/boot/limine/
	cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isodir/boot/limine/
	cp limine/BOOTX64.EFI isodir/EFI/BOOT/
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		isodir -o feudos.iso
	./limine/limine bios-install feudos.iso

run-iso: iso
	qemu-system-x86_64 -cdrom feudos.iso

clean:
	rm -rf obj feudos.bin feudos.iso isodir limine
