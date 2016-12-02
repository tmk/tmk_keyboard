KL27Z128/256 board
==================
2016/11/30

KL27Z is configured to use internal 48MHz RC oscillator.

The board has push button on PTA4 and LED on PTD7. The button works as 'a' key and the LED as an indicator for capslock.


ELF board
---------
### prototypte pinout
- pcb layout  https://deskthority.net/resources/image/33293
- schematic  https://deskthority.net/resources/image/33252


    39 .....  44 \ conn / 1 ..... 6
    38           |______|    LED  7
     :  Rst                       :
     :                            :
    32  Bl                        13
    31 .........24 23............ 14

    1   PTD6        12  PTC3         23  GND        34  PTE20
    2   PTD5        13  PTC2         24  PTA18      35  PTA20   Reset
    3   PTD4        14  PTC1         25  3.3V       36  PTA3    SWD_DIO
    4   PTD3        15  PTC0         26  NMI        37  PTA0    SWD_CLK
    5   PTD2        16  PTB17        27  PTA2       38  3.3V
    6   PTD1        17  PTB16        28  PTA1       39  GND
    7   PTD0        18  PTB3         29  PTE25      40  VBUS    VREGIN
    8   PTC7        19  PTB2         30  PTE24      41  USB D-
    9   PTC6        20  PTB1         31  PTE30      42  USB D+
    10  PTC5        21  PTB0         32  PTE29      43  GND
    11  PTC4        22  PTA19        33  PTE21      44  USB Shield

### ROM bootloader pins
See Reference Manual Chapter 13.

    PTA2 LPUART0_TX
    PTA1 LPUART0_RX *
    PTB0 I2C0_SCL
    PTB1 I2C0_SDA
    PTC4 SPI0_SS_b *
    PTC7 SPI0_MISO
    PTC6 SPI0_MOSI
    PTC5 SPI0_SCK *

Due to errata e9457, need to pull-up `SPI0_SS_b`(or `SPI0_SCK`) to prevent false detection of SPI.
The errata doesn't refer UART though, `LPUART0_RX` also requires pull-up or down resisitor from experience. 
Without resistor it easily false-detects activity on UART interface with finger touch on the pin.




Resources
---------
### Deskthority thread
https://deskthority.net/workshop-f7/can-we-design-the-teensy-alternative-for-keyboards-t13662-510.html

### Kinetis KL2x
http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/kinetis-cortex-m-mcus/l-series-ultra-low-power-m0-plus/kinetis-kl2x-48-mhz-usb-ultra-low-power-microcontrollers-based-on-arm-cortex-m0-plus:KL2x

### KL27Z128/256 Data Sheet
http://www.nxp.com/assets/documents/data/en/data-sheets/KL27P64M48SF6.pdf

### KL27Z128/256 Reference Manual
http://www.nxp.com/assets/documents/data/en/reference-manuals/KL27P64M48SF6RM.pdf

### Errata
http://www.nxp.com/assets/documents/data/en/errata/KINETIS_L_1N71K.pdf

>>
e9457: Kinetis Flashloader/ ROM Bootloader: The peripheral auto-detect code in
bootloader can falsely detect presence of SPI host causing non-responsive
bootloader
Description: During the active peripheral detection process, the bootloader can interpret spurious data on
the SPI peripheral as valid data. The spurious data causes the bootloader to shutdown all
peripherals except the “falsely detected" SPI and enter the command phase loop using the
SPI. After the bootloader enters the command phase loop using the SPI, the other peripherals
are ignored, so the desired peripheral is no longer active.
The bootloader will not falsely detect activity on the I2C, UART, or USB interfaces, so only the
SPI interface is affected.
Workaround: Ensure that there is an external pull-up on the SPI chip-select pin or that the pin is driven high.
This will prevent the bootloader from seeing spurious data due to activity on the SPI clock pin.

### Kinetis Bootloader and blhost
http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/kinetis-cortex-m-mcus/kinetis-symbols-footprints-and-models/kinetis-bootloader:KBOOT


Build
-----

    make


Program
-------
Flash firmware with ROM bootloader.

    blhost -u -- flash-image  build/kl27z.hex erase


TODO
----
- Fix boards/ELF files
- add macro `USBx_CTL_RESUME` to ChibiOS-Contrib/os/common/ext/CMSIS/KINETIS/kl27zxxx.h
