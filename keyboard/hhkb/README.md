Alternative Controller for HHKB Pro
===================================
I wanted to add some features like vi cursor and mouse keys to my [HHKB][HHKB] but its controller is not programmable and firmware source code is not open, of course. This means customizing this keyboard needs to replace original controller with programmable one.

This controller can work with HHKB **Professional**, **Professional 2**, **JP** and **Type-S**.

See [this thread][AltController] in geekhack.org for details.

[HHKB]: http://www.pfu.fujitsu.com/hhkeyboard/
[AltController]: http://geekhack.org/index.php?topic=12047.0


## Update
* Bluetooth module RN-42 is supported.(2015/01)


## Features
* Customizable keymap
* More keymap layers(more Fn keys)
* Mouse keys
* USB NKRO
* Bluetooth(RN-42)

See README of [tmk_keyboard] for more.

[tmk_keyboard]: http://github.com/tmk/tmk_keyboard
 
### Pros
* No risks: Everything is all reversible
* No need for PCB trace patching, case cutting or any other destructive mod
* Can keep original controller intact
* Can change all HHKB behavior as you like

### Cons
* Void your warranty
* Lose USB hub function of Pro2

## DISCLAIMER
I'm not a professional of electronics nor MCU programming. This may damage your HHKB.
And my English writing is poor, I'm not sure I can convey my notions accurately.


## HHKB Internals
See [doc/HHKB.txt](doc/HHKB.txt) and files under [doc/](doc/) for internal of HHKB and this controller.


## Build Firmware & Program
See [this document](../../tmk_core/doc/build.md) and [this](https://github.com/tmk/tmk_keyboard/wiki#build-firmware) first.

### Configuration
Build options and firmware settings are available in `Makefile` and `config.h` or `config_rn42.h`.


### Keymap
To define your own keymap create file named `keymap_<name>.c` and see [keymap document](../../tmk_core/doc/keymap.md) and existent keymap files.


### Build 
Several version of keymap are available in advance but you are recommended to define your favorite layout yourself. Just `make` with `KEYMAP` option like:

    $ make -f Makefile.<variant> KEYMAP=<name> clean
    $ make -f Makefile.<variant> KEYMAP=<name>

`Makefile` can be used for **Pro2 and Pro** USB controller, `Makefile.jp` for **JP**, `Makefile.rn42` for **Pro2** Bluetooth and `Makefile.rn42.jp` for **JP** Bluetooth.


### Program
First, push reset button on board to start bootloader.

This command programs the controller with [dfu-programmer] if the tool is installed and configured properly.

    $ make -f Makefile.<variant> KEYMAP=<name> dfu

Or you can also use [FLIP] command to program. Also the tool should be installed and configured properly. FLIP GUI application is also available.

    $ make -f Makefile.<variant> KEYMAP=<name> flip



## Hardware

### TMK Alt Controller Board
Preassmbled controller is available from [here](https://geekhack.org/index.php?topic=71517.0).

You can find design files of the controller at [Keyboard Controller Board for HHKB(KiCad project)](https://github.com/tmk/HHKB_controller).


[dfu-programmer]: http://dfu-programmer.sourceforge.net/
[FLIP]: http://www.atmel.com/tools/FLIP.aspx
