# HHKB Pro 2 - Teensy 2.0 controller mod

This TMK keyboard variant is for a destructive modification to the HHKB Pro 2,
replacing the stock controller with a Teensy 2.0. See the docs directory for
further reference images and links. The relevant geekhack thread is here:

https://geekhack.org/index.php?topic=57008.0

## How To Compile

To compile only:

    KEYMAP=keymapname make

To compile and send to the teensy, use the following command:

    KEYMAP=keymapname make program

for example:

    KEYMAP=spacefn make program

Note that you should *first* check the Makefile for the PROGRAM value and ensure it matches the command on your own system (the command that will be used to program the teensy). On an Arch Linux system, the command is teensy-loader-cli (dashes) though it may be teensy_loader_cli (underscores) on other systems.

To clean up afterwards:

    make clean
