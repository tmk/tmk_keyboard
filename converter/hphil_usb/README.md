HP-HIL Keyboard Converter
=========================
This was ported from MMcM's HP-HIL Converter.

- https://github.com/MMcM/qmk_firmware/tree/3496929812d687c1acfd383237835a787515c933/keyboards/converter/hp_hil

- https://deskthority.net/viewtopic.php?f=7&t=23968



Wireing
-------
HP-HIL device requires +12V on power line according to the spec, while signal line uses 5V logic.
The device works entirely with 5V regulated in fact, it will need 7V at least for voltage regulator input.

The converter uses `PD0` for SI which reads from the device, and `PD1` for SO which sends out.

Pull-up resistors are not needed probably. Strong pull-up(low resistor value) like 1K Ohm causes problem on 'PD0' at least, the device can't drive signal line with it.


Resources
---------
### HP-HIL Technical Reference Manual
http://bitsavers.informatik.uni-stuttgart.de/pdf/hp/hp-hil/45918A-90001_HP-HIL_Technical_Reference_Manual_Jan86.pdf


### TMK wiki page
https://github.com/tmk/tmk_keyboard/wiki/HP-HIL

