M0110/M0110A to USB keyboard converter
======================================
This firmware converts the protocol of Apple Macintosh keyboard **M0110**, **M0110A** and **M0120** into USB. Target of this project is USB AVR controller like **ATmega32U2** and **ATmega32U4**. Using this converter you can revive these retro keyboards with modern computer.

Read README of top directory too.

Pictures of **M0110 + M0120** and **M0110A** with [TMK converter].

![M0110+M0120](http://i.imgur.com/dyvXb2Tm.jpg)
![M0110A](http://i.imgur.com/HuHOEoHm.jpg)


- M0110A support was contributed by [skagon@github](https://github.com/skagon).
- M0120 also is supported. keys(+ * / and ,) on M0120 are recognized as cursor keys.



Update
------
- 2013/08: Change port for signals `PF` to `PD`
- 2013/09: Change port again, it uses inversely `PD0` for data and `PD1` for clock line now.
- 2014/06: Change keymaps
- 2015/03: Add support for "International"(ISO) keyboard(keymap_intl.c)
- 2016/09: Unimap support - keymap editor on web browser



Hardware
--------
You can buy preassembled [TMK converter] or make yourown with AVR dev board like PJRC [Teensy].

Port of the MCU `PD1` is assigned to `CLOCK` line and `PD0` to `DATA` by default, you can change pin configuration with editing `config.h`.

[![M0110 Converter](http://i.imgur.com/yEp2eRim.jpg)](http://i.imgur.com/yEp2eRi.jpg)

### 4P4C phone handset cable
Note that original cable used with Mac is **straight** while phone handset cable is **crossover**.

<http://en.wikipedia.org/wiki/Modular_connector#4P4C>

[Teensy]: http://www.pjrc.com/teensy/
[TMK converter]: https://geekhack.org/index.php?topic=72052.0


### Socket Pinout
- <http://pinouts.ru/Inputs/MacKeyboard_pinout.shtml>

![Jack fig](http://www.kbdbabel.org/conn/kbd_connector_macplus.png)


### Pull-up Resistor
You may need pull-up resistors on signal lines(`CLOCK`, `DATA`) in particular when you have long or coiled cable. **1k-10k Ohm** will be OK for this purpose. In that case the converter may not read signal from keyboard correctly without pull-up resistors.



Building Firmware
-----------------
To compile firmware you need AVR GCC. You can edit *Makefile* and *config.h* to change compile options and pin configuration. Also `KEYMAP` option can be used to select keymap.

    $ git clone git://github.com/tmk/tmk_keyboard.git (or download source)
    $ cd m0110_usb
    $ make -f Makefile.rev2 clean
    $ make -f Makefile.rev2 [KEYMAP={intl|spacefn}]

Use `Maefile.rev1` for TMK converter rev.1 and Teensy(ATMega32u4), instead.



Keymap
------
To create your own keymap copy existent keymap file to `keymap_<name>.c` and edit it. You can build with `make -f Makefile.rev2 KEYMAP=<name>`.

Or you can edit keymap on web browser and download firmware.

http://www.tmk-kbd.com/tmk_keyboard/editor/


Debug
-----
You can use [PJRC HID listen](http://www.pjrc.com/teensy/hid_listen.html) to see debug output. The converter has some functions for debug, press `<Magic>+H` simultaneously to get help.

- Magic combo: `Shift+Option+⌘` or `Shift+Option+Ctrl`(`Shift+Alt+Gui` or `Shift+Alt+Control`)
