Fourway Sample Project
======================

This sample project shows how to configure a four button WASD keyboard.

This project implements a much more flexible matrix.c and most projects should not need to modify it.

While matrix.c is very flexible it is not a complete implementation.  The current implementation assumes the diodes in the switch matrix are wired on to the rows.  Attempts to change DIODE_DIRECTION will result in an error message.

The order of the structures in config.h are organized from the end user's point of view to controller configuration.

From the top to bottom we have:

* keyboard layout
* matrix layout
* controller configuration
* usb configuration

Keyboard Layout
---------------
Defines the physical layout of the keys.  See usb_keycodes.h for all KB_* codes.

Matrix Layout
-------------
Defines the pcb layout of the switches and how it maps to the logical layout.  Use KB_NO in the logial layout to mean there is not key at that logical position.

Controller Configuration
------------------------
Defines how the rows and cols in the logical layout are wired on to the Teensy 2.0 controller.  Use NC to mean not connected (see ports.h for all defines). The order of the rows and cols configured on to the Teensy does not matter.  The code will walk the rows and cols in the correct order.

Usb Configuration
-----------------
Defines usb metadata.

Implementation Details and Limitations
--------------------------------------
The rows in the matrix are connected to output pins while the cols are input pins.  The input is configured to be pulled up with a resistor, therefore reading 1 means the switch is open and 0 is the switch is closed.  This also means when the output is row is "turned on" it is shorted to ground to attempt to pull down the resistor.

The current implementation can only go up to 16 rows but it can be easily expanded to 64 rows with a uint64_t.

The controller is assumed to be a Teensy 2.0, but can easily be extended to use a Teensy 2.0+ to support even larger matrices.

Setting DIODE_DIRECTION to COLS is not implemented but this can be worked around by transposing both the keyboard layout and matrix layouts in config.h.

To debug matrix.c uncomment two lines in matrix_init.  This will add an init delay of 1 second and print debug messages which are readable using hid_listen.  Also adding a delay of 300ms (use _delay_ms(300)) in matrix_scan helps slows down the entire program to make the messages readable in real time.
