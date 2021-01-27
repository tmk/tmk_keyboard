Build Firmware and Program Controller
=====================================


Download and Install
--------------------
### 1. Install Tools

1. **Toolchain** On Windows install [Atmel AVR Toolchain][atmelgcc] for AVR GCC compiler and [Cygwin][cygwin](or [MinGW][mingw]) for shell terminal. On Mac you can use [CrossPack][crosspack] or your favorite package manager. On Linux you can install AVR GCC with your favorite package manager.

2. **Programmer** Install [dfu-programmer][dfu-prog]. GUI tool [Atmel FLIP][flip] also can be used on Windows.

3. **Driver** On Windows when you start DFU bootloader on the chip for the first time you will see **Found New Hardware Wizard** to install driver. If you install the device driver properly you will find a chip name like **ATmega32U4** under the **LibUSB-Win32 Devices** tree in **Device Manager**. If not you will need to update its driver in **Device Manager**. You will find the driver in the `FLIP` install directory, something like this: `C:\Program Files (x86)\Atmel\Flip 3.4.5\usb\`. For `dfu-programmer` install the driver distributed with it.

If you use a PJRC Teensy you don't need step 2 and 3 above, just get the [Teensy loader][teensy-loader].


### 2. Download Source
You can find the firmware source on github:

- <https://github.com/tmk/tmk_keyboard>

If you are familiar with [`git`][git] tools you can use that; otherwise you can download the zip archive from:

- <https://github.com/tmk/tmk_keyboard/archive/master.zip>


Build Firmware
--------------
### 1. Open Terminal
Open a terminal window. Use the Cygwin (or MingGW) shell terminal on Windows, the `Terminal.app` on Mac OSX, and whatever terminal you like on Linux.

### 2. Change Directory
Move to the project directory in the firmware source.

    cd tmk_keyboard/{'keyboard' or 'converter'}/<project>

### 3. Make
Build the firmware using the GNU `make` command. You'll see a `<project>_<variant>.hex` file in that directory unless something unexpected occurs in build process.


    make -f Makefile.<variant> clean
    make -f Makefile.<variant>




Program Controller
------------------
Now you have a **hex** file in the current directory. This **hex** file is only needed to program your controller, other files are used for development and you may leave and forget them.

### 1. Start Bootloader
How you program the controller depends on the controller chip and its board design. To program an AVR USB chip you'll need to start it up in bootloader mode. Most boards have a push-button to load the bootloader. Consult with your controller board's manual.

<!--PROGRESS-->
### 2. Program with DFU bootloader
Stock AVR USB chips have the DFU bootloader by factory default. `FLIP` is a DFU programmer on Windows offered by Atmel. `FLIP` has two version of the tool, a GUI app and a command line program. If you want the GUI see the tutorial below. An open source alternative, `dfu-programmer`, also supports AVR chips; it is a command line tool and runs on Linux, Mac OSX and even Windows.


To program with the command line version of `FLIP` run this. Note that you need to set the PATH variable properly.

    $ make -f Makefile.<variant> flip

With `dfu-programmer` run this.

    $ make -f Makefile.<variant> dfu

Or you can execute the command directly as the following.

    $ dfu-programmer <controller> erase --force
    $ dfu-programmer <controller> flash <your_firmware.hex>
    $ dfu-programmer <controller> reset

The `<controller>` part will be `atmega32u4` or `atmega32u2` in most cases. See the manual of the command for details. On Linux and Mac OSX you will need proper permissions to program a controller and you can probably use `sudo` command for this purpose. On Linux you also can configure `udev` rules to set permissions.


#### FLIP GUI Tutorial
1. In the menu bar click **Device** -> **Select**, then choose your chip name. (In most cases **ATmega32U2** or **ATmega32U4**)
2. In the menu bar click **Settings** -> **Communication** -> **USB**, then click the **Open** button in the **USB Port Connection** dialog. At this point you'll have to plug into USB and start the bootloader.

3. In the menu bar click **File** -> **Load HEX File**, then select your firmware hex file in the File Selector dialog.
4. In the **Operations Flow** panel click the **Run** button to load the firmware binary to the chip. Note that you should keep the **Erase**, **Blank Check**, **Program** and **Verify** check boxes selected.
5. Re-connect the USB cord or click the **Start Application** button to restart your controller.
Done.

See also these instructions if needed.

- <http://code.google.com/p/micropendous/wiki/LoadingFirmwareWithFLIP>
- <http://www.atmel.com/Images/doc7769.pdf>


##### Troubleshooting
* **FLIP: AtLibUsbDfu.dll not found**
Remove the current driver and re-install the one FLIP provides from DeviceManager of Windows. See <http://imgur.com/a/bnwzy>


### 3. Program with Another Programmer
If you are using a PJRC Teensy consult with the instructions for [Teensy Loader][teensy-loader]. Or run this target with `make` after you install the command line version of it.

    $ make -f Makefile.<variant> teensy

You may want to use another programmer like [`avrdude`][avrdude]. In that case you can still use make target `program` for build with configuring `PROGRAM_CMD` in the Makefile. See below.

    $ make -f Makefile.<variant> program


[atmelgcc]:     http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORWINDOWS.aspx
[cygwin]:       https://www.cygwin.com/
[mingw]:        http://www.mingw.org/
[crosspack]:    http://www.obdev.at/products/crosspack/index.html
[flip]:         http://www.atmel.com/tools/FLIP.aspx
[dfu-prog]:     http://dfu-programmer.sourceforge.net/
[teensy-loader]:http://www.pjrc.com/teensy/loader.html
[avrdude]:      http://savannah.nongnu.org/projects/avrdude/
[git]:          https://git-scm.com/



Makefile Options
----------------
### 1. MCU and Frequency.

    MCU = atmega32u4       # Teensy 2.0
    #MCU = at90usb1286      # Teensy++ 2.0
    F_CPU = 16000000

Set your MCU and its clock in Hz.

    # Boot Section Size in *bytes*
    #   Teensy halfKay   512
    #   Teensy++ halfKay 2048
    #   Atmel DFU loader 4096
    #   LUFA bootloader  4096
    OPT_DEFS += -DBOOTLOADER_SIZE=4096

If you are using a PJRC Teensy use `512` for `BOOTLOADER_SIZE`, otherwise use `4096` unless you are sure.

### 2. Features
Optional. Note that you can ***comment out*** lines with `#` to disable them.

    BOOTMAGIC_ENABLE = yes      # Virtual DIP switch configuration(+1000)
    MOUSEKEY_ENABLE = yes       # Mouse keys(+4700)
    EXTRAKEY_ENABLE = yes       # Audio control and System control(+450)
    CONSOLE_ENABLE = yes        # Console for debug(+400)
    COMMAND_ENABLE = yes        # Commands for debug and configuration
    SLEEP_LED_ENABLE = yes      # Breathing sleep LED during USB suspend
    #NKRO_ENABLE = yes          # USB Nkey Rollover - not yet supported in LUFA
    #BACKLIGHT_ENABLE = yes     # Enable keyboard backlight functionality

### 3. Programmer
Optional. Set the proper command for your controller, bootloader, and programmer. This command can be used with `make program`.

    # avrdude with AVRISPmkII
    PROGRAM_CMD = avrdude -p $(MCU) -c avrispmkII -P USB -U flash:w:$(TARGET).hex

    # avrdude with USBaspLoader
    PROGRAM_CMD = avrdude -p $(MCU) -c usbasp -U flash:w:$(TARGET).hex

    # avrdude with arduino
    PROGRAM_CMD = avrdude -p $(MCU) -c arduino -P COM1 -b 57600 -U flash:w:$(TARGET).hex



Config.h Options
----------------
### 1. Magic Command Key Combination

    #define IS_COMMAND() (keyboard_report->mods == (MOD_BIT(KB_LSHIFT) | MOD_BIT(KB_RSHIFT)))

### 2. Mechanical Locking Support for CapsLock

    /* Mechanical locking CapsLock support. Use KC_LCAP instead of KC_CAPS in keymap */
    #define CAPSLOCK_LOCKING_ENABLE
    /* Locking CapsLock re-synchronize hack */
    #define CAPSLOCK_LOCKING_RESYNC_ENABLE

### 3. Disable Debug and Print

    /* disable debug print */
    #define NO_DEBUG

    /* disable print */
    #define NO_PRINT

### 4. Disable Action Features

    #define NO_ACTION_LAYER
    #define NO_ACTION_TAPPING
    #define NO_ACTION_ONESHOT
    #define NO_ACTION_MACRO
    #define NO_ACTION_FUNCTION

***TBD***
