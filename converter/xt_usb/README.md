XT to USB keyboard converter
==============================
This firmware converts XT keyboard protocol to USB.(It supports Scan Code Set 1.)


Connect Wires
-------------
In case of Teensy2.0(ATMega32U4):

1. Connect **Vcc** and **GND**.
2. Connect **Clock** and **Data** line. 
    - **Interrupt**:   **Clock** is on `PD1` and **Data** on `PD0`.(Recommended. Soarer's converter compatible)
3. Optionally you need pull-up resistor. 1K-10K Ohm is OK.

To change pin configuration edit **config.h** and **Makefile**.


Build Firmware
--------------
For **PJRC Teensy** just run `make`:

    $ make clean
    $ make

To select keymap:

    $ make clean
    $ make KEYMAP=[plain|jis|spacefn|...]

After that you will find HEX file `xt_usb_lufa.hex` in current directory.


Keymap
------
Several version of keymap are available in advance but you are recommended to define your favorite layout yourself. To define your own keymap create file named `keymap_<name>.c` and see keymap document(you can find in README.md of top directory) and existent keymap files.

