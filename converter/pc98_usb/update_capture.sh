#!/bin/sh
MCU=atmega32u2  make -f Makefile.capture clean
MCU=atmega32u4  make -f Makefile.capture clean
#MCU=at90usb1286 make -f Makefile.capture clean

MCU=atmega32u2  make -f Makefile.capture all
MCU=atmega32u4  make -f Makefile.capture all
#MCU=at90usb1286 make -f Makefile.capture all

cp pc98_capture_atmega32u2.hex binary
cp pc98_capture_atmega32u4.hex binary
#cp pc98_capture_at90usb1286.hex binary

MCU=atmega32u2  make -f Makefile.capture clean
MCU=atmega32u4  make -f Makefile.capture clean
#MCU=at90usb1286 make -f Makefile.capture clean

#git add -u binary
#git commit -m "pc98_usb: Update prebuilt firmware files for signal capture"
