Sun to USB keyboard protocol converter
======================================
This converter translates Sun keyboard protocol into USB HID to use vintage Sun keyboards on modern computer. The converter should work with Sun Type 2-5 Keyboards. Target MCU is ATmega32U2/4. Mouse support is available only to ATmega32U2 at this time.


Prebuilt TMK Sun converter is available here:

https://geekhack.org/index.php?topic=72052.0

Refer to this page for Sun keyboard protocol details.

https://github.com/tmk/tmk_keyboard/wiki/Sun-Keyboard-Protocol

Check wiki for general information about TMK keyboard firmware.

https://github.com/tmk/tmk_keyboard/wiki



Update
------
2024-09-28 Add mouse support and rewrite code to use timer for software uart
2020-04-08 Added unimap support



Wiriing
-------
For Type 4 and 5 keyboards use 8Pin mini DIN.

       ___ ___
      /  |_|  \
     / 8  7  6 \
    | 5    4  3 |
     \_ 2   1 _/
       \_____/
     (receptacle)

    Pin mini DIN        MCU
    ----------------------------------
    1   GND             GND
    2   GND             GND
    3   5V              VCC
    4   Mouse RX/TX     PD4
    5   Keyboard RX     PD2
    6   Keyboard TX     PD3
    7   GND             GND
    8   5V              VCC


Just wire connecotr pins to MCU pins respectively, no extra component is needed. Pull-up resistor on signal lines are optional.



Keymap Editor
-------------
You can download prebuilt firmware on TMK keymap editor.

https://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?sun_usb

For ATmega32U4 use this link, instead.

https://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?sun_usb_32u4



Build Firmware
--------------
For TMK converter with ATmega32U2 just run `make` to build firmware hex file. For ATmega32U4 use `make -f Makefile.atmega32u4` instead of `make`.

    $ cd sun_usb
    $ make

Then, load the hex file into MCU with your favorite programmer. If you have `dfu-programmer` installed you can use `make dfu`.

    $ make dfu



Keyboard Commands
-----------------
You can send Sun keyboard commands with TMK Magic key combo. Magic key is `LShift + RShift` on the converter.

For example, to send 'Reset' command press `LShift + RShift + Delete` key combo.

Following Sun specific commands are available.

    --- Sun keyboard commands ---
    Home:        Toggle Bell
    End:         Toggle Click
    PgUp:        LED All On
    PgDown:      LED All Off
    Insert:      Layout
    Delete:      Reset
