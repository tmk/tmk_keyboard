Keymap Editor for TMK firmware
==============================

Limitation
----------


TODO & MEMO
------------
### Edit action for FN key
### Better source output
- KEYMAP macro
### Actionmap support
### Macro editor
### Layer operation
- copy, swap, set all, clear...
### Compress share URL
- lz-string.js(Done)

### Bootloader jump doesn't work
### Better Keyobard graphic
### Keyobard layout screenshot
- HTML5 canvas
- png image file output


ldscript and Flash Memory
-------------------------
[`ldscript_keymap_avr5.x`](https://github.com/tmk/tmk_keyboard/blob/master/ldscript_keymap_avr5.x) is custom ldscript to place keymap at fixed address(0x6800 in this case).

Flash Memroy Map of ATMega32U4(32KB):

    +--------------------+ 0x0000
    | .vectors           | 0xac (43vectors * 4bytes)
    | .progmem           | PROGMEM variables and PSTR
    | .init0-9           |
    | .text              | code
    | .fini9-0           |
    |                    | > text region
    |--------------------| _etext
    | .data              |
    | .bss               |
    | .noinit            |
    |                    | > data region
    |--------------------| 0x6800
    | .keymap.fn_actions |
    | .keymap.keymaps    | > keymap region(2KB)
    |--------------------| 0x7000
    | bootloader         | 4KB
    +--------------------+ 0x7FFF


Keymap(8bit code)
-----------------
This is a format of keymap used in TMK Keymap Editor. This is comprised of two arrays. `fn_actions` is a 64 byte fixed size array of 32 action codes(16bit). `keymaps` is a three dimensional array of key code(8bit) and contains 8 layers of keymap whose size(ROW x COL) is vary on each keyboard matrices.

Signature of the arrays:

- `const uint16_t fn_actions[] __attribute__ ((section (".keymap.fn_actions")))`
- `const uint8_t keymaps[8][ROW][COL] __attribute__ ((section (".keymap.keymaps")))`

These arrays are placed at fixed address in .keymap section:


    actions[] -> +-------------+
                 | action x 32 |
    keymaps[] -> |-------------|
                 | keymap    0 |
                 |-------------|
                 | keymap    1 |
                 |-------------|
                 | keymap    2 |
                 |-------------|
                 | keymap    3 |
                 |-------------|
                 |   :    :    |



Actionmap(16bit code)
---------------------
`actionmaps` is a three dimensional array of actionmap which is matrix(ROW x COL) of action codes(16bit), but this is not actually used in firmware yet at this time. All key action can be represented in 16bit code. Flash Memory usage will be about doubled size compared to keymap(8bit code).
 
Signature of the arrays:

- `const uint16_t actionmaps[LAYER][ROW][COL] __attribute__ ((section (".keymap.actionmaps")))`


    actionmaps[] -> +-------------+
                    | actionmap 0 |
                    |-------------|
                    | actionmap 1 |
                    |-------------|
                    | actionmap 2 |
                    |-------------|
                    | actionmap 3 |
                    |-------------|
                    |   :    :    |


Flash usage cosidaration
------------------------
As for example of biggest ever keymap, PS/2 and ADB converter have 256 keys in a layer, so use 256 bytes each layer in keymap and 512 bytes in actionmap.
In most cases map size of keyboard will be far smaller than that.

### ATmega32U4

    Firmware    -24KB(with full option)
    Keymapping    4KB-
    Bootloader    4KB(Atmel stock) or 512B(Teensy)

### ATmega16U4(Not suppoeted yet)

    Firmware    -10KB
    Keymapping    2KB-
    Bootloader    4KB(Atmel stock) or 512B(Teensy)



Hex file format
---------------
[Intel Hex(wikipedia)](http://en.wikipedia.org/wiki/Intel_HEX)

### 1.Start code
    ':'

### 2.Byte Count
    2 x hex(0-FF)

### 3.Address
    4 x hex(0-FFFF)

### 4.Record Type
    2 x hex(00-05)

#### Record Type code
    00: Data record. which contains 16bit address and data.
    01: End of File record. It must appear in the last line.(usually :00000001FF)
    02: Extended Segment Address Record.
    03: Start Segment Address Record.
    04: Extended Linear Address Record. Upper two bytes of 32 bit address.
    05: Start Linear Address Record.

### 5.Data
    byte_count * hex(00-FF)

a sequence of data bytes

### 6.Checksum
    2 x hex(00-FF)

LSB of 2's complement of the sum of fields[ 'Byte Count', 'Address', 'Record Type' and 'Data' ]

`checksum = ~(the sum of bytes)&0xff + 1 = (the sum)&0xff^0xff + 1`

