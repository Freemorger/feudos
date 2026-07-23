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
To build and run FeudOS, you would need:
- `make` 
- `x86_64-elf-gcc` cross compiler (ver. 15 or newer) 
- `nasm` assembler
- `grub-mkrescue`, for ISO generation
- `xorriso` 
Optional:
- `qemu-system-x86_64` (to run ISO in VM)
- `x86_64-elf-binutils` (for debugging)
### Getting prerequisites on different OS'es
#### Debian-based GNU/Linux (Debian, Ubuntu, Mint, PopOS, etc.)
```sh
sudo apt update
sudo apt install \
    build-essential \
    nasm \
    grub-pc-bin \
    xorriso \
    qemu-system-x86
```
Then build the cross compiler (`x86_64-elf-gcc`) by yourself of get a binary from third-party source.
#### Fedora GNU/Linux
```sh
sudo dnf install \
    make \
    gcc \
    nasm \
    grub2-tools \
    xorriso \
    qemu-system-x86
```
Again, build the cross compiler by yourself or get it somewhere.. 
#### Arch-based GNU/Linux (Arch, Manjaro, EndeavourOS, CachyOS, Artix, etc)
```sh
sudo pacman -S --needed \
    base-devel \
    nasm \
    grub \
    xorriso \
    qemu-desktop
```
You can build cross compiler with AUR recipe `x86_64-elf-gcc` or from source.    
Example for yay:
```sh
yay -S x86_64-elf-gcc 
# yay -S x86_64-elf-binutils # if needed 
```
#### FreeBSD 
```sh 
pkg install \
    gmake \
    nasm \
    grub2 \
    xorriso \
    qemu
```
Build cross compiler or get it from 3rd party.   
Run build with `gmake` instead of `make`.
#### macOS 
```sh 
brew install \
    make \
    nasm \
    xorriso \
    qemu
```
GRUB:
```sh 
brew install --cask grub2
```
Build x86_64-elf-binutils and x86_64-elf-gcc manually, or install from a tap that provides an ELF cross toolchain.
## Get & build it 
```sh
git clone https://github.com/Freemorger/feudos
make # build ISO
make run-iso # build and run ISO in qemu VM
```
