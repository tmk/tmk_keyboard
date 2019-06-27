#!/bin/sh
make clean
make
cp fc660c.hex binary/fc660c.hex
make clean
make KEYMAP=emu
cp fc660c.hex binary/fc660c_emu.hex
make clean
