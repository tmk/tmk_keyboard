PS/2 to USB keyboard converter
==============================
This firmware converts PS/2 keyboard protocol to USB.(It supports Scan Code Set 2.)

You can discuss about this converter here.

https://geekhack.org/index.php?topic=14618.0


Preassembled TMK PS/2-USB converter is available here.

https://geekhack.org/index.php?topic=72052.0


Build Firmware
--------------
For **TMK converter Rev.2**:

    $ make -f Makefile.rev2 clean
    $ make -f Makefile.rev2 KEYMAP=plain

To program firmware push the button on converter and run:

    $ make -f Makefile.rev2 KEYMAP=plain dfu


For **TMK converter Rev.1** use `Makefile.rev1` instead.

If your use DIY converter with ATMega32u4 board you can use `Makefile.32u4`, but note that progarmming with 'dfu' may not work for your converter.


Build your own converter
------------------------
Use ATMega32u4 as controller and Makefile.32u4 to build firmware.

1. Wire **Vcc** and **GND** properly.
2. Connect **Clock** to `PD1` and **Data** to `PD0` line. (Compatible to Soarer's converter pin configuration)
3. You need pull-up resistors on both signal lines. 1K-10K Ohm would be fine.

You can configure controller and pin configurations in Makefile and config.h.


Keymap
------
To define your own keymap copy `unimap_plain.c` to `unimap_<name>.c` and edit it. Or just edit `unimap_plain.c` directly.

See wiki pages and documents.

https://github.com/tmk/tmk_keyboard/wiki


PS/2 signal handling implementations
------------------------------------
Following three methods are available to implement PS/2 signal handling.

- **Interrupt** Uses pin interrupt to detect falling edge of clock line. **Recommended.** (ps2_interrupt.c)
- **Busywait** Implementation with portable C code for reference. (ps2_busywait.c)
- **USART** Uses AVR USART hardware engine to receive PS/2 signal. You must use this fothis for V-USB. (ps2_usart.c)

TMK converter rev1 uses **USART** and rev2 uses **Interrupt** for reference.

You can change method by editing `Makefile` but not needed in most case.


V-USB Support
-------------
With V-USB you can use this converter on ATmega(168/328). Use Makefile.vusb to build firmeware. Not supported actively anymore.

Circuit:

                    +---+   +---------------+
    USB            GND  |   |   ATmega168   |
    ===                 C3  |               |
    5V <-------+--------+---|Vcc,AVCC       |        PS/2
               R1           |               |        ====
    D- <----+--+-----R2-----|INT1        RXD|------->DATA
    D+ <----|---+----R3-----|INT0        XCK|------->CLOCK
            Z1  Z2          |               |      ->5V
    GND<----+---+--+--+-----|GND            |      ->GND
                   |  |     |               |
                   |  C2-+--|XTAL1          |
                   |     X1 |               |
                   +--C3-+--|XTAL2          |
                            +---------------+
    R1:     1.5K Ohm
    R2,R3:  68 Ohm
    Z1,Z2:  Zenner 3.6V
    C1,C2:  22pF
    C3:     0.1uF
    X1:     Crystal 20MHz(16MHz/12MHz)
