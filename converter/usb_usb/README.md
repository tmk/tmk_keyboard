USB to USB keyboard protocol converter
======================================
See this for detail and discussion here.

- https://geekhack.org/index.php?topic=69169.0

See this for TMK general info.

- https://github.com/tmk/tmk_keyboard/wiki



Hardware requirement
--------------------
There are two options.

### TMK USB-USB Converter
You can buy a fully preassembled converter from me here.

- https://geekhack.org/index.php?topic=72052.0

### Arduino Leonardo + USB Host Shield
Buying Arduino Leonardo and USB Host Shield.

- https://store.arduino.cc/products/arduino-leonardo-with-headers
- https://docs.arduino.cc/retired/shields/arduino-usb-host-shield
- https://www.sparkfun.com/products/9947



Build firmware
--------------
See wiki.

https://github.com/tmk/tmk_keyboard/wiki#build-firmware



Keymap Editor
-------------
You can editor keymap and download firmware with web browser.

- http://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?usb_usb



Limitation
----------
The converter supports 'HID Boot protocol' only. This means it can host only standard USB keyboard(6KRO).

- Media/System control keys are not recognized.
- 'Fn' key itself cannot be remapped.

Many of NKRO keyboards still can work with the converter in 6KRO mode.
