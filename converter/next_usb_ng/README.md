NeXT non-ADB Keyboard Converter
===============================
With this converter you can use NeXT keyboard non-ADB on modern computer.

https://github.com/tmk/tmk_keyboard/wiki/NeXT

You can support this project by buying prebuilt TMK NeXT keyboard converter.

https://geekhack.org/index.php?topic=72052.0

For NeXT Keyboard ADB see TMK ADB Converter, instead.


Pinouts
-------
Socket on converter from front:

            _____
     GND  / 5   4 \  PSW
    FROM | 3     2 | TO
          |     1 |  VCC
           `-----'
    FROM: keyboard to computer
    TO:   computer to keyboard
    PSW:  Power key



Wiring
------
Connections to ATmega32U2/U4

    FROM(Pin3): PD0
    TO(Pin2):   PD1
    PSW(Pin4):  PD4
    GND(Pin5):  GND
    VCC(Pin1):  VCC



Protocol
--------
See this for NeXT keyboard protocol.

- https://github.com/tmk/tmk_keyboard/wiki/NeXT#next-keyboard-protocol
