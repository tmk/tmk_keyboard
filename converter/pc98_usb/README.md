PC98 to USB keyboard protocol converter
=======================================
Main target is TMK prebuilt converter(ATMega32u2) but it will work on dev boards with ATMega32u4.

To initialize keyboard correctly you have to connect keyboard with converter first and then plug them into USB port.
Otherwise, keyboard may not work.


You can buy [TMK prebuilt converter](https://geekhack.org/index.php?topic=72052.0) here to support this project.

Discuss or report issue on github or [this thread](https://geekhack.org/index.php?topic=110094.0).



Connector
---------
On host/converter:

    8Pin mini DIN
       ___ ___
      /  |_|  \
     / 8  7  6 \
    | 5    4  3 |
     \_ 2   1 _/
       \_____/
     (receptacle)


Wiring: You can change this with editing config.h.

    Pin mini DIN        MCU
    ----------------------------------
    1  ~RST(TXD)        PD3
    2   GND             GND
    3  ~RDY             PD4
    4   RXD             PD2
    5  ~RTY             PD1
    6   NC
    7   NC
    8   5V              VCC



Protocol
--------
Signal: Asynchronous, Positive logic, 19200baud, Least bit first
Frame format: 1-Start bit(Lo), 8-Data bits, Odd-Parity, 1-Stop bit



Prebilt Firmware
----------------
Prebuilt firmware HEX files are available under 'binary' directory.



Build Firmware
--------------
Just use 'make' to bild firmware for TMK PC98 converter(ATMega32u2).

    $ make clean
    $ make

To flash firmware run this command

    $ make dfu

then push button on the converter to flash firmware.

If you are using ATMega32u4 based board use following commands to get firmware HEX file.

    $ make -f Makefile.32u4 clean
    $ make -f Makefile.32u4

And flash it on your board using specific program method.



PC98 Scan Codes
---------------
    ,---. ,---. ,------------------------. ,------------------------. ,------------------------.
    |Stp| |Cpy| | F1 | F2 | F3 | F4 | F5 | | F6 | F7 | F8 | F9 | F10| | F11| F12| F13| F14| F15|
    `---' `---' `------------------------' `------------------------' `------------------------'
    ,-------------------------------------------------------------. ,-------.  ,---------------.
    | Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bspc| |Ins|Del|  |Hom|Hlp|  -|  /|
    |-------------------------------------------------------------| |-------|  |---------------|
    | Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|      | |RUp|RDn|  |  7|  8|  9|  *|
    |-------------------------------------------------------`Enter| |-------|  |---------------|
    |Ctl|Cap|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  \|     | |  Up   |  |  4|  5|  6|  +|
    |-------------------------------------------------------------| |-------|  |---------------|
    | Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO| Shift | |Lef|Rig|  |  1|  2|  3|  =|
    `-------------------------------------------------------------' |-------|  |---------------|
       |Kana|Win|GRP|NFER|       Space        |XFER| ^2|Win| ^3|    | Down  |  |  0|  ,|  .| ^1|
       `-------------------------------------------------------'    `-------'  `---------------'
    ^1: Enter on keypad is identical to Enter on alphanumeric portion
    ^2: ALT on DIGITAL WAVE Dboard while Menu on PC-9821 keyboard
    ^3: HOME on PC98XL keyboard
    ^4: CAPS and Kana are locking keys, either mechanically or by firmware emulation


    ,---. ,---. ,------------------------. ,------------------------. ,------------------------.
    | 60| | 61| | 62 | 63 | 64 | 65 | 66 | | 67 | 68 | 69 | 6A | 6B | | 52 | 53 | 54 | 55 | 56 |
    `---' `---' `------------------------' `------------------------' `------------------------'
    ,-------------------------------------------------------------. ,-------.  ,---------------.
    | 00 | 01| 02| 03| 04| 05| 06| 07| 08| 09| 0A| 0B| 0C| 0D| 0E | | 38| 39|  | 3E| 3F| 4D| 41|
    |-------------------------------------------------------------| |-------|  |---------------|
    |  0F  | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 1A| 1B|      | | 36| 37|  | 42| 43| 44| 45|
    |-------------------------------------------------------`  1C | |-------|  |---------------|
    | 74| 71| 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 28|     | |   3A  |  | 46| 47| 48| 49|
    |-------------------------------------------------------------| |-------|  |---------------|
    |   70    | 29| 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33| 70/7D | | 3B| 3C|  | 4A| 4B| 4C| 4D|
    `-------------------------------------------------------------' |-------|  |---------------|
       | 72 | 77| 73| 51 |        34          | 35 | 79| 78| 5E|    |   3D  |  | 4E| 4F| 50| 1C|
       `-------------------------------------------------------'    `-------'  `---------------'
    5E: HOME on PC98XL keyboard
    79: ALT on DIGITAL WAVE Dboard
    77,78,79: Left Win, right Win and Application on PC-9821 keyboard



PC98 keyboard commands
----------------------
http://www.webtech.co.jp/company/doc/undocumented_mem/io_kb.txt

response from keyboard:
    FAh: ACK
    FCh: NACK

Inhibit repeating key:
    9Ch, 70h

LED control:
    9Dh 7xh

    second byte:
    0111 xxxx
         |||`- NumLock
         ||`-- ?
         |`--- CapsLock
         `---- Kana

Enabling Win and App key on PC-9821 keyboard:
    95h, 03h



Raku Raku keyboard support
--------------------------
NEC PC-9801-98-S02 - raku raku keyboard:
https://deskthority.net/photos-f62/nec-pc-9801-98-s02-t5212.html

To build firmware for the keyboard

    $ make KEYMAP=rakuraku clean
    $ make KEYMAP=rakuraku
    $ make KEYMAP=rakuraku dfu

Its scan code map is very different from standard types. This is not tested.



Other PC98 converter projects and resource
------------------------------------------
### TMK wiki
- https://github.com/tmk/tmk_keyboard/wiki/PC-9801-Keyboard

### PC-9800シリーズ テクニカルデータブック HARDWARE 編 1993年 p139, p343
- https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n151
- https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n355

### PC98 to USB
- http://davy.nyacom.net/kbd98usb/

### PC98 to PS/2
- http://www.tsp.ne.jp/~sawada/mago/c_gka98at.htm
- http://www.tsp.ne.jp/~sawada/mago/src/gka98at.asm

### Scan code
- http://ixsvr.dyndns.org/usb2pc98



NOTES
-----
### Tested on
- PC-9801V
- DIGITAL WAVE Dboard


### RDY
PC98 host keeps RDY line high to prevent keyboard from sending data while processing.

- https://archive.org/stream/PC9800TechnicalDataBookHARDWARE1993/PC-9800TechnicalDataBook_HARDWARE1993#page/n359

PC-9801V keyboard requires RDY pulse as acknowledgement from host, it doesn't next data without this. Dboard doesn't need this.


### Inhibit key repeating
The command(9Ch, 70h) works with Raku Raku keybaord but not with Dboard.

### LED indicater
Dboard has LEDs but replys with FA to 9D command but ignore it. The LED indicates just its internal states. Dboard replays with FA to 9C command but it doesn't seem to understand repeat setting.

PC-9801V has no LEDs and doesn't accept LED command. It replys with 9D to 9D command. PC-9801V doesn't accept repeat setting command. It replys with 9C to 9C command.


### PC-9801V
Note that you have to connect this keyboard with converter before plug in USB port. It seems this keyboard requires for host to send any command before starting to send scan code.


### Connection order
Converter doesn't support hotplugging due to limitation of PC-9801 keyboard.
You will have to plug keyboard into converter during without power first, and then plug them into USB port.
