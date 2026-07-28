# FeudOS
My attempt to write a x86_64 OS kernel :D     
Still Very early stages   
Planned idea: something unix-like, partially posix, monolithic/hybrid kernel.   
Currently written in C, a bit of nasm. Uses `gnu23` C standard in kernel.
## Table of contents
- [Build prerequisites](#build-prerequisites)
  - [Getting prerequisites on different OS'es](#getting-prerequisites-on-different-oses)
    - [Debian-based GNU/Linux (Debian, Ubuntu, Mint, Pop!_OS, etc.)](#debian-based-gnulinux-debian-ubuntu-mint-pop_os-etc)
    - [Fedora GNU/Linux](#fedora-gnulinux)
    - [Arch-based GNU/Linux (Arch, Manjaro, EndeavourOS, CachyOS, Artix, etc.)](#arch-based-gnulinux-arch-manjaro-endeavouros-cachyos-artix-etc)
    - [FreeBSD](#freebsd)
    - [macOS](#macos)
- [Get & build it](#get--build-it)

## Build prerequisites

To build and run FeudOS, you'll need:

* `make`
* `x86_64-elf-gcc` cross compiler (version 15 or newer)
* `nasm`
* `limine`
* `xorriso`

Optional:

* `qemu-system-x86_64` (to run the ISO in a VM)
* `x86_64-elf-binutils` (for debugging)
* `tigervnc` (if you want to run qemu with VNC, would be useful if you're on 
wayland - see `run_qemu_wayland.sh` comments)

### Getting prerequisites on different operating systems

#### Debian-based GNU/Linux (Debian, Ubuntu, Mint, Pop!_OS, etc.)

```sh
sudo apt update
sudo apt install \
    build-essential \
    nasm \
    xorriso \
    qemu-system-x86 \
    git
```

Build `x86_64-elf-gcc` yourself or get a binary from a third-party source.

Build Limine:

```sh
git clone https://github.com/limine-bootloader/limine.git
cd limine
make
# Optionally install limine somewhere in your PATH.
```

#### Fedora GNU/Linux

```sh
sudo dnf install \
    make \
    gcc \
    nasm \
    xorriso \
    qemu-system-x86 \
    git
```

Again, build the cross compiler yourself or get it from a third-party source.

Build Limine the same way as above.

#### Arch-based GNU/Linux (Arch, Manjaro, EndeavourOS, CachyOS, Artix, etc.)

```sh
sudo pacman -S --needed \
    base-devel \
    nasm \
    xorriso \
    qemu-desktop \
    git
```

You can build the cross compiler using the AUR package `x86_64-elf-gcc` or from source.

Example with `yay`:

```sh
yay -S x86_64-elf-gcc
# yay -S x86_64-elf-binutils # if needed
```

Limine is also available in the AUR:

```sh
yay -S limine
```

#### FreeBSD

```sh
pkg install \
    gmake \
    nasm \
    xorriso \
    qemu \
    git
```

Build the cross compiler and Limine yourself (or get them from a third-party source).

Run the build with `gmake` instead of `make`.

#### macOS

```sh
brew install \
    make \
    nasm \
    xorriso \
    qemu \
    git
```

Build `x86_64-elf-binutils`, `x86_64-elf-gcc`, and Limine manually, or install them from a tap that provides an ELF cross toolchain.

## Get & build it

```sh
git clone https://github.com/Freemorger/feudos
cd feudos

make      # Build the ISO
make run-iso # Build and run the ISO in QEMU
```
> [!NOTE]
> Wayland users might encounter rendering issues with default QEMU screen. 
> See `run_qemu_wayland.sh` script comments to learn more

