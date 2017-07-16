Sony NEWS keyboard converter
============================
Sony NEWS is a BSD workstation with 68K/MIPS released in 1987 in Japan.

- http://en.wikipedia.org/wiki/Sony_NEWS
- https://www.sony.net/SonyInfo/CorporateInfo/History/SonyHistory/2-12.html#block3

This converter allows NEWS keyboard to be connected to modern PC via USB. It works with NWP-5461 and NWP-411A.

How DIP switches work is unknown and you may need to turn all switches off with this converter.

Limitations:
- Speaker/Buzzer is not supported.
- LEDs on NWP-5461 is not supported.

Pics:
- http://imgur.com/a/JyMzw

Discussion:
- https://geekhack.org/index.php?topic=25759



Protocol
--------
NEWS keyboard protocol is a usual asynchronous serial communication. You can receive data from the keyboard easily with UART of micro controller.

- TTL level
- 9600bps
- 1-start bit
- 8-data bit
- non-parity
- 1-stop bit.

Its scan code is one byte consists of 1-bit release(break) flag at MSB and 7-bit code.
For example 0x29 is sent when 'a' key is pressed and 0xA9 when released.

    MSB         LSB
    7 6 5 4 3 2 1 0   bit
    | | | | | | | |
    | +-+-+-+-+-+-+-- scan code(00-7F)
    +---------------- break flag: sets when released


Scan Codes
----------
### NWP-5461

    ,---.   ,------------------------, ,------------------------. ,---------.                   
    |Pow|   | F1 | F2 | F3 | F4 | F5 | | F6 | F7 | F8 | F9 | F10| | F11| F12| ,-----------.
    `---'   `------------------------' `------------------------' `---------' |  *|  /|  +|    
    ,-------------------------------------------------------------. ,---. ,---------------|
    |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|  BS | |Hlp| |  7|  8|  9|  -|
    |-------------------------------------------------------------| |---| |---------------|
    |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|Del|   | |Ins| |  4|  5|  6|  ,|
    |---------------------------------------------------------'   | |---| |---------------|
    |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  `|Return| |Clr| |  1|  2|  3|   |
    |-------------------------------------------------------------| |---| |-----------|Ent|
    |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /|   |Shift   | |PgU| |  0|  .| Up|   |
    |-------------------------------------------------------------| |---| |---------------|
    |Alt  |Gui |Alt |         Space     |          |Gui|App|Ctrl  | |PgD| |Tab|Lef|Dow|Rig|
    `-------------------------------------------------------------' `---' `---------------'
     
    ,---.   ,------------------------, ,------------------------. ,---------.                   
    | 7A|   | 01 | 02 | 03 | 04 | 05 | | 06 | 07 | 08 | 09 | 0A | | 68 | 69 | ,-----------.
    `---'   `------------------------' `------------------------' `---------' | 64| 65| 52|
    ,-------------------------------------------------------------. ,---. ,---------------|
    | 0B| 0C| 0D| 0E| 0F| 10| 11| 12| 13| 14| 15| 16| 17| 18|  19 | | 6A| | 4B| 4C| 4D| 4E|
    |-------------------------------------------------------------| |---| |---------------|
    |  1A | 1B| 1C| 1D| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27|   | | 6B| | 4F| 50| 51| 56|
    |---------------------------------------------------------'   | |---| |---------------|
    |  28  | 29| 2A| 2B| 2C| 2D| 2E| 2F| 30| 31| 32| 33| 34|  35  | | 6C| | 53| 54| 55|   |
    |-------------------------------------------------------------| |---| |-----------| 5A|
    |  36    | 37| 38| 39| 3A| 3B| 3C| 3D| 3E| 3F| 40| 41|   42   | | 6D| | 57| 59| 58|   |
    |-------------------------------------------------------------| |---| |---------------|
    | 43  | 44 | 45 |       46          |    47    | 48| 49|  4A  | | 6E| | 66| 5B| 5C| 5D|
    `-------------------------------------------------------------' `---' `---------------'



Pinouts
-------
### NWP-5461

     -------------
     \ 1 2 3 x 5 /
      \ 6 7 8 9 /
       ---------
    1 VCC
    2 BZ(Speaker)
    3 Keyboard Data(from keyboard MCU TxD)
    4 NC
    5 GND
    6 Keyboard Command?(to MCU RxD via two schmitt triggers of Z8 74LS14)
    7 Mouse Data(from Mouse Ext connector)
    8 Power switch(directly to the switch via diode)
    9 FG

I have three NWP-5461s and GND and FG is connected in one of them for some reason.
- Serial No.14178(EH0100 5 04 04 0173) GND and FG is not tied.
- Serial No.14580(EH0100 5 09 12 0211) GND and FG is not tied.
- Serial No.17458(EH0100 7 09 08 0104) GND and FG is not tied.


### NWP-411A

    -------------
    \ 1 2 3 x 5 /
     \ x 7 x 9 /
      ---------
    1 VCC
    2 BZ(Speaker)
    3 Keyboard Data(from keyboard MCU TxD)
    4 NC
    5 GND
    6 NC
    7 Mouse Data(from Mouse Ext connector)
    8 NC
    9 FG
    NOTE: These are just from my guess and not confirmed.
