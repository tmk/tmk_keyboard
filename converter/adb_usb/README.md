ADB to USB keyboard converter
=============================
This firmware converts Apple ADB keyboard/mouse protocol to USB and you can use old ADB keyboard/mouse on modern computer. It works on TMK ADB-USB converter or AVR microcontroller(ATMega32U4/2).

Discussion and info: https://geekhack.org/index.php?topic=14290.0

TMK ADB-USB converter: https://geekhack.org/index.php?topic=72052.0



Infos
-----
Wiki: https://github.com/tmk/tmk_keyboard/wiki

ADB protocol: https://github.com/tmk/tmk_keyboard/wiki/Apple-Desktop-Bus

Issues: https://github.com/tmk/tmk_keyboard/issues?q=is%3Aissue+ADB

Firmware Code: https://github.com/tmk/tmk_keyboard/tree/master/converter/adb_usb



Wiring
------
Connect DATA, VCC and GND to microcontroller. Use port **PD0** for DATA. PSW is not used.

You can change the port with `ADB_PORT`, `ADB_PIN`, `ADB_DDR`, `ADB_DATA_BIT` in `config.h`.

ADB female socket from the front:

      ,--_--.
     / o4 3o \      1: DATA
    | o2   1o |     2: PSW(Power SW)
     -  ===  -      3: VCC
      `-___-'       4: GND


### Pull-up resister:
The external **1k Ohm** pull-up resistor on DATA is **required**.

AVR microcontroller's internal pull-up is too weak for ADB in particular when you want to use a long coiled cable or daisy-chain devices. 

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
    R: 1k Ohm resistor

https://github.com/tmk/tmk_keyboard/wiki/Apple-Desktop-Bus#pull-up-resistor



Build firmware and Program microcontroller
------------------------------------------
To build firmware and program TMK ADB-USB Converter run these commands:

    $ make -f Makefile clean
    $ make -f Makefile [KEYMAP=yourname]
    $ make -f Makefile [KEYMAP=yourname] dfu

You can select keymap name with optional `KEYMAP=` ('plain' is default name). Push button on the converter before running `dfu` target.

Use **Makefile.rev1** for old TMK Converter rev.1 and Teensy2.0 instead of **Makefile**.

https://github.com/tmk/tmk_keyboard/wiki#build-firmware



Debug
-----
Use hid_listen command line tool to see debug outputs.

https://github.com/tmk/tmk_keyboard/wiki#debug



Notes for keyboard
------------------
Apple Standard keyboard(M0116) can't discriminate between right and left modifiers
while Apple Extended keyboard(M0115/M3501) can discriminate them except for Command key.



Notes for mouse
---------------
All one-button mouses should be supported and others will work as one-button mouse even if not supported.

Mouse protocols(handler ID) below are curretnly supported.

- Apple Classic Mouse protocol (1, 2)
- Apple Extended Mouse protocol (4)
- Kensington Turbo Mouse 5 #64210 and Thinking Mouse (0x32)
- Macally 2-button Mouse (0x42)
- Logitech MouseMan/TrackMan Proprietary protocol (0x4C*)
- Logitech MouseMan/TrackMan Extended protocol (0x4D*)
- Micrspeed MacTrac (0x2F, 0x5F) - Not confirmed
- Contour Design Countour Mouse (0x66) - Not confirmed
- Mouse Systems A3 Mouse/Trackball (0x03) - Not confirmed
- CH Products Tracball Pro/DT225 (0x42) - Not confirmed

https://github.com/tmk/tmk_keyboard/wiki/Apple-Desktop-Bus#mouse



Locking CapsLock
----------------
Many of old ADB keyboards have mechanical push-lock switch for Capslock and some like Apple Adujstable keyboard use firmware-base locking with momentary switch for Capslock.

The converter supports the locking Capslock key. Use keycode `LCAP` instead of `CAPS` in your keymap.

https://github.com/tmk/tmk_keyboard/wiki/FAQ-Keymap#mechanical-lock-switch-support
