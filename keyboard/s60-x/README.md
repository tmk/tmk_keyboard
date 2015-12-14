S60-X keyboard firmware
=======================
DIY compact keyboard designed by VinnyCordeiro for Sentraq. Based on GH60 code.

## Build
Move to this directory then just run `make` like:

    $ make

Use `make -f Makefile.pjrc` if you want to use PJRC stack but I find no reason to do so now.


## Keymap
Several version of keymap are available in advance but you are recommended to define your favorite layout yourself. To define your own keymap create file named `keymap_<name>.c` and see keymap document(you can find in top README.md) and existent keymap files.

To build firmware binary hex file with a certain keymap just do `make` with `KEYMAP` option like:

    $ make KEYMAP=[custom|poker|poker_set|poker_bit|plain|hasu|spacefn|hhkb|<name>]

File S60-X_lufa.hex.old is a testing firmware to confirm that all possible switch positions are working on the PCB. It has the layout shown on [keymap_custom.c](keymap_custom.c).


### 0  Initial explanations
The █████ blocks on the layouts hides the switch positions that do not exist physically on the PCB. If you feel like hacking the keyboard and adding new keys, those are the positions that can be used. You'll have to modify the [keymap_common.h](keymap_common.h) file for that.

The ▒▒▒▒▒ blocks hides switch positions not used on this particular layout, but they do exist on the PCB.

There is no LED support on the PCB at the moment, but I'll let the code for that untouched.


### 1  Custom
The custom keymap - [keymap_custom.c](keymap_custom.c) - is where I tested all the switches, not being concerned with a specific layout or layers. It's a plain layout option with the extra keys used on ISO & HHKB layouts being assigned some other keys.

#### 1.0 Default layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │  `  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │  =  │PgUp │BkSpc│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │ Tab │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │  \  │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Caps │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │PgDwn│Enter│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Shift│Home │  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │ End │Shift│  Up │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Ctrl │ Gui │ Alt │█████│█████│█████│Space│█████│█████│█████│ Alt │ Gui │ App │Ctrl │█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘


### 2  Poker
[keymap_poker.c](keymap_poker.c) emulates original Poker layers
while both [keymap_poker_bit.c](keymap_poker_bit.c) and [keymap_poker_set.c](keymap_poker_set.c) implements same layout in different way and they fix a minor issue of original Poker and enhance arrow keys.

    Fn + Esc = `
    Fn + {left, down, up, right}  = {home, pgdown, pgup, end}

#### 2.0 Default layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │  `  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │  =  │▒▒▒▒▒│BkSpc│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │ Tab │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │  \  │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Caps │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │▒▒▒▒▒│Enter│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Shift│▒▒▒▒▒│  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │▒▒▒▒▒│Shift│▒▒▒▒▒│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Ctrl │ Gui │ Alt │█████│█████│█████│Space│█████│█████│█████│ Fn  │ Gui │ App │Ctrl │█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
#### 2.1 Poker Fn layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │ Esc │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │▒▒▒▒▒│     │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │ FnQ │ Up  │     │     │     │     │     │     │ Cal │     │Home │ Ins │     │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │Left │Down │Right│     │     │ Psc │ Slk │Pause│     │ Tsk │ End │▒▒▒▒▒│     │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │▒▒▒▒▒│ Del │     │ Web │Mute │ VoU │ VoD │     │PgUp │PgDwn│ Del │▒▒▒▒▒│ Up  │▒▒▒▒▒│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │     │     │█████│█████│█████│ FnS │█████│█████│█████│ Fn  │Left │Down │Right│█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘


### 3. Plain
Without any Fn layer this will be useful if you want to use key remapping tool like AHK on host.
See [keymap_plain.c](keymap_plain.c) for detail.

#### 3.0 Plain Default layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │ Esc │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │  =  │▒▒▒▒▒│BkSpc│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │ Tab │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │  \  │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Caps │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │▒▒▒▒▒│Enter│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Shift│▒▒▒▒▒│  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │▒▒▒▒▒│Shift│▒▒▒▒▒│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Ctrl │ Gui │ Alt │█████│█████│█████│Space│█████│█████│█████│ Alt │ Gui │ App │Ctrl │█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘


### 4. Hasu
This is Hasu's favorite keymap with HHKB Fn, Vi cursor and Mousekey layer. See [keymap_hasu.c](keymap_hasu.c) for detail.

(Hasu is the creator of the TMK firmware, for those who do not know that.)


### 5. SpaceFN
This layout proposed by spiceBar uses space bar to change layer with using Dual role key technique. See [keymap_spacefn.c](keymap_spacefn.c) and [SpaceFN discussion](http://geekhack.org/index.php?topic=51069.0).

#### 5.0 Default layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │ Esc │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │  =  │▒▒▒▒▒│BkSpc│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │ Tab │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │  \  │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Caps │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │▒▒▒▒▒│Enter│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Shift│▒▒▒▒▒│  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │▒▒▒▒▒│Shift│▒▒▒▒▒│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Ctrl │ Gui │ Alt │█████│█████│████ Space/Fn ███│█████│█████│ Alt │ Gui │ App │Ctrl │█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
#### 5.1 SpaceFN layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │  `  │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │▒▒▒▒▒│ Del │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │     │     │     │     │     │     │Home │ Up  │ End │ Psc │ Slk │Pause│ Ins │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │     │     │     │     │     │PgUp │Left │Down │Right│     │     │▒▒▒▒▒│     │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │▒▒▒▒▒│     │     │     │     │Space│PgDwn│  `  │  ~  │     │     │▒▒▒▒▒│     │▒▒▒▒▒│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │     │     │█████│█████│█████│ Fn  │█████│█████│█████│ Alt │ Gui │ App │Ctrl │█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘


### 6. HHKB
[keymap_hhkb.c](keymap_hhkb.c) emulates original HHKB layers.
#### 6.0: Default layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │ Esc │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │  =  │  \  │  `  │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │ Tab │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │BkSpc│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Ctrl │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │ Fn3 │  '  │▒▒▒▒▒│Enter│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Shift│▒▒▒▒▒│  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │▒▒▒▒▒│Shift│ Fn  │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │▒▒▒▒▒│ Gui │ Alt │█████│█████│█████│Space│█████│█████│█████│▒▒▒▒▒│ Alt │ Gui │▒▒▒▒▒│█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
#### 6.1: HHKB Fn layer
    ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    │ Pwr │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │ Ins │ Del │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │Caps │     │     │     │     │     │     │     │ Psc │ Slk │ Pus │ Up  │     │     │█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │ VoD │ VoU │ Mut │ Ejc │     │  *  │  /  │Home │PgUp │Left │Right│▒▒▒▒▒│Enter│█████│
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │     │▒▒▒▒▒│     │     │     │     │     │  +  │  -  │ End │PgDwn│Down │▒▒▒▒▒│     │     │
    ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    │▒▒▒▒▒│     │     │█████│█████│█████│     │█████│█████│█████│▒▒▒▒▒│     │     │▒▒▒▒▒│█████│
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘

    
