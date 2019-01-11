make -f Makefile.unimap.zzh KEYMAP=hhkb_zzh TARGET=hhkb_unimap_zzh

mv hhkb_unimap_zzh.hex ./binary/

make -f Makefile.unimap.zzh KEYMAP=hhkb_zzh TARGET=hhkb_unimap_zzh clean
