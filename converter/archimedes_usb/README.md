Acorn Archimedes Keyboard Converter
===================================
This covnerter makes Acorn Archimedes keyboard usable on modern USB computer.

Check these for technical information and resources.

- https://github.com/tmk/tmk_keyboard/wiki/ACORN-ARCHIMEDES-Keyboard
- https://geekhack.org/index.php?topic=121747.0


Preassembled converter is available [here](https://geekhack.org/index.php?topic=72052.0).


Pinouts and Wirings
-------------------
Mini-DIN-6P female socket from the front:

                    Connector                   AVR pin
                    ------------------------------------
      ,--_--.       1: Reset                    PD0
     / o6 5o \      2: NC
    | o4   3o |     3: GND                      GND
     - 2o o1 -      4: 5V                       VCC
      `-___-'       5: In(from keyboard)        PD1
                    6: Out(to keyboard)         PD3

Target microcontroller is Atmel ATMega32U2 by default but porting this project to other 8-bit AVR controllers would be easy.


Mouse support
-------------
Not tested with real mouse but it should work somehow.

I don't have Archimedes mouse to test, try and make a report if you have one.
