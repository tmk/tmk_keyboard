PC98 to USB keyboard protocol converter
=======================================
TMK Keyboard protocol converter for NEC PC-9801 series that allows you to use PC-9801 keyboard on modern USB PC.

Main target is TMK prebuilt converter(ATMega32u2) but it will work on dev boards with ATMega32u4.


You can buy [TMK prebuilt converter](https://geekhack.org/index.php?topic=72052.0) here to support this project.

Discuss or report issue on github or [this thread](https://geekhack.org/index.php?topic=110094.0).
See [this wiki page](https://github.com/tmk/tmk_keyboard/wiki/PC-9801-Keyboard) for PC98 keyboard technical infos.



Connector
---------
On host/converter:

    8Pin mini DIN
       ___ ___
      /  |_|  \
     / 8  7  6 \
    | 5    4  3 |
     \_ 2   1 _/
       \_____/
     (receptacle)


    Pin mini DIN        AVR Pins
    ----------------------------------
    1  ~RST(TXD)        PD3
    2   GND             GND
    3  ~RDY             PD4
    4   RXD             PD2
    5  ~RTY             PD1
    6   NC
    7   NC
    8   5V              VCC



Prebilt Firmware
----------------
Prebuilt firmware HEX files are available under 'binary' directory.



Build Firmware
--------------
### TMK PC98 Converter
Just use 'make' to bild firmware for TMK PC98 converter(ATMega32u2).

    $ make clean
    $ make

To flash firmware run this command

    $ make dfu

then push button on the converter to flash firmware.


### Ohter boards
If you are using ATMega32u4 based board use following commands to get firmware HEX file.

    $ make -f Makefile.32u4 clean
    $ make -f Makefile.32u4

And flash it on your board using specific program method.



Raku Raku keyboard support
--------------------------
This is not supported actively anymore.

NEC PC-9801-98-S02 - raku raku keyboard:
https://deskthority.net/photos-f62/nec-pc-9801-98-s02-t5212.html

To build firmware for the keyboard

    $ make KEYMAP=rakuraku clean
    $ make KEYMAP=rakuraku
    $ make KEYMAP=rakuraku dfu

Its scan code map is very different from standard types.
See `unimap_trans.h` and `unimap_rakuraku.c`.
