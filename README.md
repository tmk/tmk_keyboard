TMK Keyboard Firmware Collection
================================
This repository includes keyboard and converter firmware projects built with [`tmk_core`][tmk_core] keyboard library.

The latest source code is available here: <http://github.com/tmk/tmk_keyboard>


Updates
-------
#### 2017/01/11
Changed action code for `ACTION_LAYER_MODS` and this may cause incompatibility with existent shared URL and downloaded firmwware of keymap editor. If you are using the action you just have to redefine it on keymap editor. Existent keymap code should not suffer.

#### 2016/06/26
Keymap framework was updated. `fn_actions[]` should be defined as `action_t` instead of `uint16_t`. And default code for keymap handling is now included in core you just need define `uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS]` and `action_t fn_actions[]`.


#### 2016/06/22
Some projects were moved from `converter` and `keyboard` to `orphan` directory. Those might be removed in some future but you will be able to access them with `orphans` tag. See <https://github.com/tmk/tmk_keyboard/issues/173>

#### 2016/02/10
core: flabbergast's Chibios protocol was merged from <https://github.com/flabbergast/tmk_keyboard/tree/chibios> (@72b1668). See [tmk_core/protocol/chibios/README.md](tmk_core/protocol/chibios/README.md). Chibios protocol supports Cortex-M such as STM32 and Kinetis.

#### 2015/04/22
Core library was separated to other branch `core`. <https://github.com/tmk/tmk_keyboard/tree/core>

In `Makefile` you need to set `TMK_DIR` to indicate core library location now.

    TMK_DIR = ../../tmk_core



Projects
--------
You can find some keyboard specific projects under `converter` and `keyboard` directory.

### converter
* [ps2_usb](converter/ps2_usb/)             - [PS/2 keyboard to USB][GH_ps2]
* [adb_usb](converter/adb_usb/)             - [ADB keyboard to USB][GH_adb]
* [m0110_usb](converter/m0110_usb)          - [Macintosh 128K/512K/Plus keyboard to USB][GH_m0110]
* [terminal_usb](converter/terminal_usb/)   - [IBM Model M terminal keyboard(PS/2 scancode set3) to USB][GH_terminal]
* [news_usb](converter/news_usb/)           - [Sony NEWS keyboard to USB][GH_news]
* [x68k_usb](converter/x68k_usb/)           - [Sharp X68000 keyboard to USB][GH_x68k]
* [sun_usb](converter/sun_usb/)             - [Sun] to USB(type4, 5 and 3?)
* [pc98_usb](converter/pc98_usb/)           - [PC98] to USB
* [usb_usb](converter/usb_usb/)             - [USB to USB][GH_usb]
* [ibm4704_usb](converter/ibm4704_usb)      - [IBM 4704 keyboard to USB][GH_ibm4704]
* [next_usb](converter/next_usb)            - NeXT(Non-ADB) to USB, contributed by [BCG](https://github.com/bgould) and based on [Adafruit's work](https://learn.adafruit.com/usb-next-keyboard-with-arduino-micro/overview)

### keyboard
* [hhkb](keyboard/hhkb/)                    - [Happy Hacking Keyboard pro][GH_hhkb] **my main board**
* [alps64](keyboard/alps64/)                - [Alps64 PCB](https://geekhack.org/index.php?topic=69740.0)
* [hbkb](keyboard/hbkb/)                    - [Happy Buckling spring keyboard][GH_hbkb](IBM Model M 60% mod)
* [Infinity](keyboard/infinity/)            - Massdrop [Infinity keyboard][Infinity]
* [gh60](keyboard/gh60/)                    - [GH60] DIY 60% keyboard [prototype][GH60_proto] **my second board**
* [onekey](keyboard/onekey/)                - Simple one key keyboard example


### Projects based tmk_keyboard or tmk_core
https://github.com/tmk/tmk_keyboard/wiki/TMK-Based-Projects


[GH_hhkb]:      http://geekhack.org/showwiki.php?title=Island:12047
[GH_ps2]:       http://geekhack.org/showwiki.php?title=Island:14618
[GH_adb]:       http://geekhack.org/showwiki.php?title=Island:14290
[GH_hhkb_bt]:   http://geekhack.org/showwiki.php?title=Island:20851
[GH_m0110]:     http://geekhack.org/showwiki.php?title=Island:24965
[GH_news]:      http://geekhack.org/showwiki.php?title=Island:25759
[GH_terminal]:  http://geekhack.org/showwiki.php?title=Island:27272
[GH_x68k]:      http://geekhack.org/showwiki.php?title=Island:29060
[GH_hbkb]:      http://geekhack.org/showwiki.php?title=Island:29483
[GH_ibm4704]:   http://geekhack.org/index.php?topic=54706.0
[GH60]:         http://geekhack.org/index.php?topic=34959
[GH60_proto]:   http://geekhack.org/index.php?topic=37570.0
[PC98]:         http://en.wikipedia.org/wiki/NEC_PC-9801
[Sun]:          http://en.wikipedia.org/wiki/Sun-3
[Infinity]:     https://www.massdrop.com/buy/infinity-keyboard-kit
[tmk_core]:     https://github.com/tmk/tmk_core



License
-------
**GPLv2** or later. Some protocol files are under **Modified BSD License**.

Third party libraries like LUFA, PJRC and V-USB have their own license respectively.



Build Firmware and Program Controller
-------------------------------------
See [tmk_core/doc/build.md](tmk_core/doc/build.md).



Change your keymap
------------------
See [tmk_core/doc/keymap.md](tmk_core/doc/keymap.md).



Magic Commands
--------------
To see help press `Magic` + `H`.

`Magic` key combination is `LShift` + `RShift` in many projects, but `Power` key on ADB converter.
`Magic` keybind can be vary on each project, check `config.h` in project directory.

Following commands can be also executed with `Magic` + key. In console mode `Magic` keybind is not needed.

    ----- Command Help -----
    c:      enter console mode
    d:      toggle debug enable
    x:      toggle matrix debug
    k:      toggle keyboard debug
    m:      toggle mouse debug
    v:      print device version & info
    t:      print timer count
    s:      print status
    e:	    print eeprom config
    n:	    toggle NKRO
    0/F10:  switch to Layer0
    1/F1:   switch to Layer1
    2/F2:   switch to Layer2
    3/F3:   switch to Layer3
    4/F4:   switch to Layer4
    PScr:   power down/remote wake-up
    Caps:   Lock Keyboard(Child Proof)
    Paus:   jump to bootloader



Boot Magic Configuration - Virtual DIP Switch
---------------------------------------------
Boot Magic are executed during boot up time. Press Magic key below then plug in keyboard cable.
Note that you must use keys of **Layer 0** as Magic keys. These settings are stored in EEPROM so that retain your configure over power cycles.

To avoid configuring accidentally additive salt key `KC_SPACE` also needs to be pressed along with the following configuration keys. The salt key is configurable in `config.h`. See [tmk_core/common/bootmagic.h](tmk_core/common/bootmagic.h).

#### General
- Skip reading EEPROM to start with default configuration(`ESC`)
- Clear configuration stored in EEPROM to reset configuration(`Backspace`)

#### Bootloader
- Kick up Bootloader(`B`)

#### Debug
- Debug enable(`D`)
- Debug matrix enable(`D`+`X`)
- Debug keyboard enable(`D`+`K`)
- Debug mouse enable(`D`+`M`)

#### Keymap
- Swap Control and CapsLock(`Left Control`)
- Change CapsLock to Control(`Caps Lock`)
- Swap LeftAlt and Gui(`Left Alt`)
- Swap RightAlt and Gui(`Right Alt`)
- Disable Gui(`Left Gui`)
- Swap Grave and Escape(`Grave`)
- Swap BackSlash and BackSpace(`Back Slash`)
- Enable NKRO on boot(`N`)

#### Default Layer
- Set Default Layer to 0(`0`)
- Set Default Layer to 1(`1`)
- Set Default Layer to 2(`2`)
- Set Default Layer to 3(`3`)
- Set Default Layer to 4(`4`)
- Set Default Layer to 5(`5`)
- Set Default Layer to 6(`6`)
- Set Default Layer to 7(`7`)



Mechanical Locking support
--------------------------
This feature makes it possible for you to use mechanical locking switch for `CapsLock`, `NumLock`
or `ScrollLock`. To enable this feature define these macros in `config.h` and use `KC_LCAP`, `KC_LN
UM` or `KC_LSCR` in keymap for locking key instead of normal `KC_CAPS`, `KC_NLCK` or `KC_SLCK`. Res
ync option tries to keep switch state consistent with keyboard LED state.

    #define LOCKING_SUPPORT_ENABLE
    #define LOCKING_RESYNC_ENABLE



Start Your Own Project
-----------------------
1. Add `tmk_core` into your repository using `git submodule` or `git subtree`.
2. Copy files from `tmk_keybaord` or other project similar to yours
3. Edit those files to support your keyboard.

See these as examples.
- https://github.com/tmk/infinity_ergodox
- https://github.com/tmk/whitefox



Debugging
--------
Use PJRC's `hid_listen` to see debug messages. You can use xprintf() to display debug info, see `tmk_core/common/xprintf.h`.

- https://www.pjrc.com/teensy/hid_listen.html



Files and Directories
-------------------
### Top
* keyboard/     - keyboard projects
* converter/    - protocol converter projects
* tmk_core/     - core library
* tmk_core/doc/ - documents



Contribution
------------
- Report bugs in github **[Issues](https://github.com/tmk/tmk_keyboard/issues)**.
- Pull requets are also welcomed.



Coding Style
-------------
- Doesn't use Tab to indent, use 4-spaces instead.



Other Keyboard Firmware Projects
------------------
You can learn a lot about keyboard firmware from these. See [Other Projects](https://github.com/tmk/tmk_keyboard/wiki/Other-Projects) other than TMK.
