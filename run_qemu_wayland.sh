#!/bin/sh
# This script will build iso and run it in QEMU with VNC 
# It exists because of some rendering issues with qemu on wayland, and you 
# might use it if you stumble upon those.
# Prerequsites: other feudos prereqs + tigervnc
set -e

make iso
qemu-system-x86_64 \
    -cdrom feudos.iso \
    -serial file:qemu.log \
    -display none \
    -vnc :0 \
    >/dev/null 2>&1 &

QEMU_PID=$!

sleep 1

vncviewer localhost:5900

kill "$QEMU_PID"
