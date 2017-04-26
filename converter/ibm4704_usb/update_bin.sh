#!/bin/sh
make -f Makefile.unimap.rev1 clean
make -f Makefile.unimap.rev2 clean
make -f Makefile.unimap.alps.rev1 clean
make -f Makefile.unimap.alps.rev2 clean

make -f Makefile.unimap.rev1
make -f Makefile.unimap.rev2
make -f Makefile.unimap.alps.rev1
make -f Makefile.unimap.alps.rev2

cp ibm4704_usb_rev1_unimap.hex binary
cp ibm4704_usb_rev2_unimap.hex binary
cp ibm4704_usb_rev1_alps_unimap.hex binary
cp ibm4704_usb_rev2_alps_unimap.hex binary

make -f Makefile.unimap.rev1 clean
make -f Makefile.unimap.rev2 clean
make -f Makefile.unimap.alps.rev1 clean
make -f Makefile.unimap.alps.rev2 clean
