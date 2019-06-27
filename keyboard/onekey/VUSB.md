TMK with Xtal-less V-USB
========================

TMK onekey works on ATtiny85.

    $ make -f Makefile.vusb

    ...

    Size after:
       text    data     bss     dec     hex filename
       5518      14     254    5786    169a onekey_vusb.elf

    -------- end --------


https://i.imgur.com/sNa302J.jpg


Program with AVRISPmkII
-----------------------
D- and D+ lines are need to be disconnected when programed.

    $ avrdude -p attiny85 -c avrispmkii -U flash:w:onekey_vusb.hex


Xtal-less internal RC
---------------------
http://codeandlife.com/2012/02/22/v-usb-with-attiny45-attiny85-without-a-crystal/

Configure fuses for PLL, internal RC and BOD(Brown out detection).

    $ avrdude -c avrispmkii -p attiny85 -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m

Set 16.5MHz to F_CPU in Makefile

    F_CPU = 16500000

Add settings for calibration in usbconfig.h

    diff --git a/keyboard/onekey/usbconfig.h b/keyboard/onekey/usbconfig.h
    index e9b4f98..01e5aa6 100644
    --- a/keyboard/onekey/usbconfig.h
    +++ b/keyboard/onekey/usbconfig.h
    @@ -161,7 +161,11 @@ section at the end of this file).
      * proceed, do a return after doing your things. One possible application
      * (besides debugging) is to flash a status LED on each packet.
      */
    -/* #define USB_RESET_HOOK(resetStarts)     if(!resetStarts){hadUsbReset();} */
    +#define USB_RESET_HOOK(resetStarts)     if(!resetStarts){hadUsbReset();}
    +/* http://codeandlife.com/2012/02/22/v-usb-with-attiny45-attiny85-without-a-crystal/ */
    +#ifndef __ASSEMBLER__
    +extern void hadUsbReset(void); // define the function for usbdrv.c
    +#endif
     /* This macro is a hook if you need to know when an USB RESET occurs. It has
      * one parameter which distinguishes between the start of RESET state and its
      * end.
    @@ -202,7 +206,7 @@ section at the end of this file).
      * usbFunctionWrite(). Use the global usbCurrentDataToken and a static variable
      * for each control- and out-endpoint to check for duplicate packets.
      */
    -#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   0
    +#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   1


Another calibration code
========================
https://github.com/obdev/v-usb/tree/master/libs-device
