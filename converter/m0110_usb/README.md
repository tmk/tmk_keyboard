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
- 2024/01: Add Keymap Editor support for International layout



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
To compile firmware you need AVR GCC. You can edit *Makefile* and *config.h* to change compile options and pin configuration.

    $ git clone git://github.com/tmk/tmk_keyboard.git (or download source)
    $ cd m0110_usb
    $ make -f Makefile.unimap.rev2 clean
    $ make -f Makefile.unimap.rev2

Use `Maefile.unimap.rev1` for TMK converter rev.1 and Teensy(ATMega32u4), instead.


### Keymap file
To create your own keymap copy `unimap.c` to `unimap_<name>.c` and edit it. You can build firmware like below:

    $ make -f Makefile.unimap.rev2 KEYMAP=<name>



Keymap Editor
-------------
You can edit your keymap on Keymap Editor and download firmware.

http://www.tmk-kbd.com/tmk_keyboard/editor/


### International layout
[Keymap Editor for International layout](https://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?m0110_usb_intl#456C4oOW4Kmg6biL5oKGcOOOiNeA5rWMxoDjkr/suIDrg6DhjIDqsqTgu4PrlIDng6DinI7Isu6zs+KykuyDjOK8nO2kgOuoue2MtuqFmeu1puSxiu6EoeySl+GKvOSskNuK5I2J4aWS64um7r6564O07bK755et7aK34Kuc45226ree65ym5pCK4Yuo5Z6h5pqE7K2V75GB6qGJ5LGo5L2I5K6K7o6G5IeG5JeG5K+M7Iam5LGG5IWI6rGA5JaX6LGt6Z6X6IGf6YGJ5IOM5q2L6K2B7K2F7K2I6ZaL7K2M5K2B5K2B6q2P5K2E7IOL6IOB7IOF7IOL7IOM5IOE4oig7Iag7LWA7L2A7Iuh6oWd5JGY5aGb6Yq865yu5KmC5Jai5JKy66GI5KCs5qii7Jmx7Jim6ruu54uJ5Jqp5o2x542G7I2M5KWG46mx7KWF75eE76W97qO024XuoaTliojotYvojZDng5TlsbvouIruvIjqhqjjkarmqIfhnIfnhrnqp6jmqKbjlZjspYTkpp3sqLTimIDir63kkqHkooHunaPhv4bpvYzmlp3ioKHhhKXgt6/knbjjjrnnvLrmgLLqj4jml7LppKLpubHskYnvkJborbniianlkrHusK/pl4rosrLqlZTrhZzqvZbqrLXjqo3lqqTlnqntlIrkuYbttbbrvZTivLbprK3pi6vkhqrthazmmJvhtrTonZ3qhZbupbTlt4Xpobfnkrntn6nmgIbrt4Hrvajnh57hrrbkn7Xth5Pkvrbtl63nkKfPnuy9iueYtOqjjemuk+imleS5veOameCym+eMnu+ziOeCtuGirOSaq+WGmuyxruucmOa5p+isqe2Lq+anlO27j+ubm+KWmeWlsuu/nuq4juqPhy/pgIA=) is available.

See also [issue #771](https://github.com/tmk/tmk_keyboard/issues/771).



Debug
-----
You can use [PJRC HID listen](http://www.pjrc.com/teensy/hid_listen.html) to see debug output. The converter has some functions for debug, press `<Magic>+H` simultaneously to get help.

- Magic combo: `Shift+Option+⌘` or `Shift+Option+Ctrl`(`Shift+Alt+Gui` or `Shift+Alt+Control`)
