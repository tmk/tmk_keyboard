Microsoft Sculpt Ergonomic Desktop keyboard firmware
======================
Ergonomic keyboard by Microsoft modified to work with Teensy 2.0.

- TODO: put original controller PCB version

## Microsoft Sculpt Ergonomic Desktop keyboard resources
- [Microsoft Sculpt Ergonomic Desktop support page]
(https://www.microsoft.com/hardware/en-us/d/sculpt-ergonomic-desktop)

## Build
Move to this directory and run `make`:

    $ make

## Matrix
Only keyboard with US layout was traced.

### Original US layout keyboard matrix
Original US layout keboard matrix is 8 rows and 18 columns:

          A     B     C     D     E     F     G     H     I
       ----------------------------------------------------
    1        PAUS         DEL     0     9           8  BSPC  -->
    2        PGUP         F12  LBRC  MINS        RBRC   INS  -->
    3        HOME        CALC     P     O           I        -->
    4        SLCK         ENT  SCLN     L           K  BSLS  -->
    5                     APP  SLSH  QUOT  RALT        LEFT  -->
    6        END   RSFT  PGDN         DOT        COMM        --> 
    7  LCTL  RGHT          UP  DOWN                     SPC  -->
    8        PSCR         F11   EQL    F9          F8   F10  -->

                          J     K     L     M     N     O     P     Q     R
                          -------------------------------------------------
                  -->     7   TAB     Q     2     1                          1
                  -->     Y    F5    F3     W     4          F6              2
                  -->     U     R     E  CAPS     3           T              3
                  -->     J     F     D           A        LGUI              4
                  -->     H     G    F4     S   ESC              LALT        5
                  -->     M     V     C     X     Z  LSFT                    6
                  -->     N     B   SPC                                RCTL  7
                  -->    F7     5    F2    F1   GRV           6              8

Note that original matrix requires 8+18=26 pins on micro-controller and will
not work verbatim on Teensy 2.0 because it only has 25 pins.

### Modified US layout keyboard matrix 

To make all keys work on Teensy 2.0 it is best to merge keys on column C and G
of orininal matrix (see above). To achieve that simply connect colums C and G
to the same pin on Teensy.

Modified matrix will have 8 rows and 17 columns and will require 25 pins:

          A     B     C     D     E     F     G     H
       ----------------------------------------------
    1        PAUS         DEL     0     9     8  BSPC  -->
    2        PGUP         F12  LBRC  MINS  RBRC   INS  -->
    3        HOME        CALC     P     O     I        -->
    4        SLCK         ENT  SCLN     L     K  BSLS  -->
    5              RALT   APP  SLSH  QUOT        LEFT  -->
    6        END   RSFT  PGDN         DOT  COMM        --> 
    7  LCTL  RGHT          UP  DOWN               SPC  -->
    8        PSCR         F11   EQL    F9    F8   F10  -->

                          I     J     K     L     M     N     O     P     Q
                          -------------------------------------------------
                  -->     7   TAB     Q     2     1                          1
                  -->     Y    F5    F3     W     4          F6              2
                  -->     U     R     E  CAPS     3           T              3
                  -->     J     F     D           A        LGUI              4
                  -->     H     G    F4     S   ESC              LALT        5
                  -->     M     V     C     X     Z  LSFT                    6
                  -->     N     B   SPC                                RCTL  7
                  -->    F7     5    F2    F1   GRV           6              8

Note that RALT and RSFT are now sitting on the same column (C).


## Keymap
Only keyboard with US layout was traced and tested.

### 1  Original Scultp keymap
[keymap.c](keymap.c) represents original Sculpt layout

    Fn + {F1, F2, F3} = {play/pause, mute, volume down, volume up}
    Fn + {left, down, up, right}  = {home, pgdown, pgup, end}

#### 1.0 Default layer
    ,---------------------------.    ,-----------------------------------------------.
    |Esc| F1| F2| F3| F4| F5| F6|    | F7| F8| F9|F10|F11|F12|PrSc|ScLk|Pau|Calc|    |
    |---------------------------|    |-----------------------------------------------|
    |  `|  1|  2|  3|  4|  5|  6|    |    7|  8|  9|  0|  -|  =|      Bcksp| Del|Home|
    |---------------------------|    |-------------------------------------|    |----|
    |  Tab|  Q|  W|  E|  R|    T|    |     Y|  U|  I|  O|  P|  [|  ]|     \|    | End|
    |---------------------------|    |-----------------------------------------------|
    |  Caps|  A|  S|  D|  F|   G|    |      H|  J|  K|  L|  ;|  '|    Enter| Ins|PgUp|
    |---------------------------|    |-----------------------------------------------|
    |  Shift|  Z|  X|  C|  V|  B|    |       N|  M|  ,|  .|  /|       Shift|  Up|PgDn|
    |---------------------------`----'-----------------------------------------------|
    |   Ctrl| Gui| Alt|            |            |  Alt|  Fn|      Ctrl|Left|Down|Righ|
    `--------------------------------------------------------------------------------'
