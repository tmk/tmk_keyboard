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

## Quick-Start Guide for TMK Alt Controller
1. Build a custom keyboard layout using the online tool
   * Use Chrome if the site does not behave as expected
   * Make sure to generate the file using the right template
      *  Pro2/Pro
         * [Bluetooth](http://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?hhkb_rn42)
         * [USB-only](http://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?hhkb)

      *  JP
         * [Bluetooth](http://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?hhkb_jp_rn42)
         * [USB-only](http://www.tmk-kbd.com/tmk_keyboard/editor/unimap/?hhkb_jp)

1. Save the `.hex` file to your computer; note this is all you need to generate
   a custom layout with most of the available features (including mouse control)
    *  Check-out [the last third of this video](https://www.youtube.com/watch?v=TYmAb8zOPWU&t=561s)
       to get an idea of how to build your own layout; it includes how to load to the firmware using FLIP<P>


1. Download and install the `dfu-programmer` (the alternative to FLIP), open-source available
   for Windows, Linux, and Mac OS.  e.g., for Mac OS: `brew install dfu-programmer` <P>
   _Note: This bootloader is all that is required to load/flash the firmware created
   using the online tool. You do not need the full `avr-gcc` development stack
   because you created the required `.hex` file using the online tool instead._

1. Please read and understand this next step in full before running the
   following three commands

   ```
   dfu-programmer atmega32u4 erase --force
   dfu-programmer atmega32u4 flash <your_downloaded_file>.hex
   dfu-programmer atmega32u4 launch
   ```

   Or if your are on Unix-like shell you can use command sequence concatinated with `&&`

   ```
   sleep 20 && \
   dfu-programmer atmega32u4 erase --force && \
   dfu-programmer atmega32u4 flash <your_downloaded_file>.hex && \
   dfu-programmer atmega32u4 launch
   ```

   * Run commands from the directory where you saved your `.hex` file
   * Put controller into programmable mode(bootloader) by hitting the red button located on your controller
   * Once in this mode, you will no longer have access to your HHKB, you need other keyboard to input the commands or mouse to copy and paste.
   * `erase` clears existent firmware on flash memory
   * `flash` loads new firmware you downloaded
   * `launch` restarts keyboard
   * See `man dfu-programmer` for details
   * Replug controller and redo the process when you are in trouble

   * `sleep 20` gives you time to switch your controller to a programmable mode
     These commands execute automatically once the ~ 20 seconds have passed.


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
