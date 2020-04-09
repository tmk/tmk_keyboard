Sun to USB keyboard protocol converter
======================================
Target MCU is ATMega32u2/4 but other USB capable AVR will also work.

This converter will work with Sun Type 2-5 Keyboards.


Check wiki pages for other information about TMK keyobard firware.

https://github.com/tmk/tmk_keyboard/wiki


Update
------
2020-04-08 Added unimap support


Connector
---------
Modern Type 4 and 5 keyboards uses 8Pin mini DIN.

       ___ ___
      /  |_|  \
     / 8  7  6 \
    | 5    4  3 |
     \_ 2   1 _/
       \_____/
     (receptacle)

    Wiring:
    Pin mini DIN        MCU
    ----------------------------------
    1   GND             GND
    2   GND             GND
    3   5V
    4   RX/TX(Mouse)
    5   RX              PD3
    6   TX              PD2
    7   GND             GND
    8   5V              VCC


Protocol
--------
Signal: Asynchronous, Negative logic, 1200baud, No Flow control

Frame format: 1-Start bit, 8-Data bits, No-Parity, 1-Stop bit

AVR USART engine expects positive logic while Sun keyboard signal is negative.
To use AVR UART engine you need external inverter in front of RX and TX pin.
Otherwise you can use software serial to communicate the keyboard.

This firmware uses software serial by default, so you don't need any inverter.
It can be still built with 'make HARDWARE_SERIAL=y' to enable hardware serial if you have inverter. You can use 74LS04 for example.


### Commands From System To Keyboard

    0x01 Reset
            Keyboard responds with following byte sequence:
            Success: 0xFF 0x04 0x7F
            Fail:    0x7E 0x01 0x7F
    0x02 Bell On
    0x03 Bell Off
    0x0A Click On
    0x0B Click Off
    0x0E LED
            followed by LED status byte:
            bit: 3       2       1       0
            LED: CapsLk  ScrLk   Compose NumLk
    0x0F Layout
            Keyboard responds with 'Layout Response' 0xFE 0xXX

### Commands From Keyboard To System
    0x7F Idle
            means no keys pressed.
    0xFE Layout Response
    0xFF Reset Response(followed by 0x04)

### Reference
- http://kentie.net/article/sunkbd/page2.htm
- http://kentie.net/article/sunkbd/KBD.pdf


Build Firmware
--------------
For TMK converter with ATmega32U2 just run `make` to build firmware hex file.
For other DIY converters with ATmega32U4 like Teensy2 or Pro Micro use `make -f Makefile.atmega32u4` instead of `make`.

    $ cd sun_usb
    $ make

Then, load the hex file into MCU with your favorite programmer. If you have `dfu-programmer` installed you can use `make dfu`.

    $ make dfu



Keyboard Control
----------------
You can send Sun protocol commands with TMK `Magic` key combo. By default `Magic` key is `LShift` + `RShift`, `LAlt` + `RAlt' or `LMeta` + `RMeta`.
https://github.com/tmk/tmk_keyboard#magic-commands

Following Sun specific commands are available. For example, to send 'Bell On' you can press `LShift` + `RShift` + `Up` keys simultaneously.

```
----- Sun converter Help -----
Up:     Bell On
Down:   Bell Off
Left:   Click On
Right:  Click Off
PgUp:   LED all On
PgDown: LED all On
Insert: Layout
Delete: Reset
```


Tested on
---------
### Type 3
http://blog.daveastels.com.s3-website-us-west-2.amazonaws.com/2014/12/27/type-3-keyboard.html

### CTCSP SHORT TYPE KEYBOARD(Type 5)
http://imgur.com/a/QIv6p
