ADB to USB keyboard converter
=============================
This firmware converts Apple ADB keyboard/mouse protocol to USB, you can use it to plug old ADB keyboard/mouse into modern computer. It works on prebuilt TMK ADB-USB Converter or generic dev board with USB AVR MCU(ATMega32U4/2) like Teensy2.0.

Discuss about this here: http://geekhack.org/showwiki.php?title=Island:14290

Prebuilt TMK ADB-USB converter is available here: https://geekhack.org/index.php?topic=72052.0



README FIRST
------------
https://github.com/tmk/tmk_keyboard
https://github.com/tmk/tmk_keyboard/tree/master/converter/adb_usb

Also check these when you are in trouble.

https://github.com/tmk/tmk_keyboard/wiki
https://github.com/tmk/tmk_keyboard/issues


Wiring
------
If you build this yourself you have to solder some wires.
Connect ADB pins to controller just by 3 lines(Vcc, GND, Data) at least. By default Data line uses port PD0.
This is not needed but you can connect PSW to PD1 optionally.

ADB female socket from the front:

      ,--_--.
     / o4 3o \      1: DATA
    | o2   1o |     2: PSW(Power SW)
     -  ===  -      3: VCC
      `-___-'       4: GND

This converter uses AVR's internal pull-up, but it seems to be too weak, in particular when you want to use a long or coiled cable. The external pull-up resistor(1K-10K Ohm) on Data is strongly recommended.(It is almost must!)
https://github.com/tmk/tmk_keyboard/wiki/FAQ#pull-up-resistor

Pull-up resister:

    Keyboard       AVR MCU
                   ,------.
    5V------+------|VCC   |
            |      |      |
           [R]     |      |
            |      |      |
    DATA----+------|PD0   |
                   |      |
    GND------------|GND   |
                   `------'
    R: 1K Ohm resistor


Define following macros for ADB connection in config.h if you use other than port PD0.

    ADB_PORT, ADB_PIN, ADB_DDR, ADB_DATA_BIT


Build firmware and Program microcontroller
------------------------------------------
See [doc/build.md](../../tmk_core/doc/build.md).

To build firmware and program TMK ADB-USB Converter run these commands:

    $ make -f Makefile clean
    $ make -f Makefile [KEYMAP=yourname]
    $ make -f Makefile [KEYMAP=yourname] dfu

You can select keymap name with optional `KEYMAP=` ('plain' is default name). Push button on the converter before running `dfu` target.

Use **Makefile.rev1** for old TMK Converter rev.1 and Teensy2.0 instead of **Makefile**.



Keymap
------
You can change keymap by editing code of unimap_plain.c directly, or copy it to your own keymap file like unimap_yourname.c and edit the file.
How to define the keymap is probably obvious. You can find key symbols in common/keycode.h. And see [doc/keymap.md](../../tmk_core/doc/keymap.md) for more detail.


Magic command
-------------
To get help message in hid_listen press `h` holding Magic key. Magic key is `Power key`.

https://github.com/tmk/tmk_keyboard/wiki#debug


Locking CapsLock
----------------
Many of old ADB keyboards have mechanical push-lock switch for Capslock key and this converter supports the locking Capslock key by default. Use keycode `LCAP` instead of `CAPS` in your keymap in short. See README in top directory for more detail.
https://github.com/tmk/tmk_keyboard/blob/master/README.md#mechanical-locking-support

If you want to remap Capslock key you will have to remove locking pin or just replace with normal momentary switch. Some keyboards like Apple Adujstable keyboard use firmware-base locking with momentary switch for Capslock and remapping it won't be useful in most cases.


Notes for keyboard
------------------
Not-extended ADB keyboards have no discrimination between right modifier and left one,
you will always see left control even if you press right control key.
Apple Extended Keyboard and Apple Extended Keyboard II can discriminate both side
modifiers except for GUI key(Windows/Command).

And most of ADB keyboards have no diodes in its matrix so they are not NKRO unfortunately,
though ADB protocol itself supports it. See tmk_core/protocol/adb.c for more info.


Notes for mouse
---------------
ADB mouse support was added by @mek-apelsin on Apr,2015.
https://github.com/tmk/tmk_keyboard/pull/207

All one-button mouses should be supported.

As of 2019 June, the converter can handle multi-button mice and trackball up to eight buttons if the pointing device supports Apple Extended Mouse protocol. But some devices use their own specific protocol unfortunately and they will work as one-button mouse unless device specific code is added.

Kensington Turbo Mouse 5(#64210) is supported now.
https://github.com/tmk/tmk_keyboard/issues/274#issuecomment-504726633

EOF
