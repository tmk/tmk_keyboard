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



HP-HIL Commands
---------------

    DHR[0xFE]   Device Hard Reset
                no response(time out)
    IFC[0x00]   Interface Clear
                response: c00
    ACF[0x08]   Auto Configure
                response: c0A-F, c0A when there is 1 device on the link
    IDD[0x03]   Identify and Describe
                response: describe record and c03
                void device_describe(void)
    EXD[0x32]   Extended Describe Record(optional)
                response: extended describe record and c32
                void device_extended_describe(void)
    EPT[0x01]   Enter Pass-Thru mode
                no response(time out)
    ELB[0x02]   Enter Loop-Back mode
                response: c02
    RPL[0x20]   RePoll
                response: c20 or data with c2x, where x indicates number of byte
    POL[0x10]   Poll info from input devices
                response: c10(nodata) or data with c1x, where x indicates number of byte


HP98203C
--------
https://www.hpmuseum.net/display_item.php?hw=682

    Describe Record: 0xE0, 0x0A, 0x61, 0x0F
        Device ID is 0xE0, it has two 8-bits axes and resolution is 3937(0x0F61)counts/meter which means exactly 100 cpi.

    Extended Describe Record: 0x40 0x00
        localization is supported and its code 0x00(Reserved).

### Replace Encoder
During careless investigation I broke original HP encoder(QEDS-8831) on the keyboard with wrong power connection.
And replaced it with COPAL optical encoder RESW20D50, the new encoder requires decoupling capacitor(22uF), without it the keyboard doesn't read signal correctly.



Device ID
---------
```
00..1Fh Keyboard, Vectra 46030A
2Ch     Audio Interface Unit 45262A
30h     Function Box 46086A
34h     HP-HIL ID 46084A
5Ch     Bar-Code Reader 92916A
60h     Rotary Control Knob 46083A
61h     HP-HIL Quadrature Port 46094A
61h     Control Dials 46085A
68h     HP Mouse 46060A
8Ch     HP-HIL/Touch Bezel 35123A
93h     A-Size Digitizer 46081A
94h     B-Size Digitizer 46088A
C0..DFh Keyboard, ITF 46020A, 46021A
A0..BFh Keyboard, Integral
E0h     Keyboard with Dial 98203C
```
