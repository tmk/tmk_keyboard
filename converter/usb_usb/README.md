USB to USB keyboard protocol converter
======================================
See for detail and discussion here.

- https://geekhack.org/index.php?topic=69169.0


Hardware requirement
--------------------
There are two options.

### TMK USB-USB Converter
You can buy a fully preassembled converter from me here.

- https://geekhack.org/index.php?topic=72052.0

### Arduino Leonardo + USB Host Shield
Buying Arduino Leonardo and USB Host Shield.

- https://store.arduino.cc/usa/leonardo
- https://store.arduino.cc/usa/arduino-usb-host-shield
- https://www.sparkfun.com/products/9947


Build firmware
--------------
Build.

    $ git clone https://github.com/tmk/tmk_keyboard.git
    $ cd tmk_keyboard
    $ git submodule init
    $ git submodule update
    $ cd converter/usb_usb
    $ make

And Program converter. Push button on TMK converter and just run this.

    $ make dfu


In case of Leonardo push reset button then run command. Serial port name(COM17) depends your system. On Linux I got /dev/ttyACM0.

    $ DEV=COM17 make program
    or
    $ avrdude -patmega32u4 -cavr109 -b57600 -Uflash:w:usb_usb.hex -PCOM17

To build debug firmware use `master_debug` branch of `USB_Host_Shield_2.0-tmk` submodule

    $ cd tmk_keyboard/tmk_core/protocol/usb_hid/USB_Host_Shield_2.0-tmk
    $ git chceckout master_debug
    $ cd tmk_keyboard/converter/usb_usb
    $ make -f Makefile.debug
    $ make -f Makefile.debug dfu



Limitation
----------
Only supports 'HID Boot protocol'.

Note that the converter can host only USB "boot protocol" keyboard(6KRO), not NKRO, it is possible to support NKRO keyboard but you will need to write HID report parser for that. Every NKRO keyboard can have different HID report and it is difficult to support all kind of NKRO keyboards in the market.



Keymap editor
-------------
You can editor keymap and download firmware with web brwoser.

- http://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?usb_usb



Update
------
- 2014/12/11  Added Hub support(confirmed with HHKB pro2)
- 2016/09/10  Unimap editor support
- 2016/10/18  Fix LED state at startup
- 2017/05/xx  Fix startup code for FC660C(3eae475)
- 2018/02/xx  Add Korean and Brazilian specific keys(ab6ddac,b2fa0ffa)
- 2018/07/xx  Fix startup issue(c2ce617)
- 2018/10/xx  Use fixed LUFA stack and update USB_Host_Shield_2.0
- 2019/09/18  Add BTLD(bootlader jump) key to unimap



Resource
--------
### USB Host Sheild 2.0
- https://www.circuitsathome.com/products-page/arduino-shields/usb-host-shield-2-0-for-arduino

### USB Host Sheild 2.0 source
- https://github.com/felis/USB_Host_Shield_2.0

### Arduino USB Host Shield(with bootst converter)
- http://arduino.cc/en/Main/ArduinoUSBHostShield

### Arduino source
- https://github.com/arduino/Arduino/hardware/arduino/{cores,variants}

### Arduino-based hardware keyboard remapper - Colemak forum
- http://forum.colemak.com/viewtopic.php?id=1561

### Teensy + Host Shield
- http://www.pjrc.com/teensy/td_libs_USBHostShield.html
