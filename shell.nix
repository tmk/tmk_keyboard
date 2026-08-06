# A Nix shell for tmk_keyboard.
# Based on the shell for qmk/qmk_firmware:
# https://github.com/qmk/qmk_firmware/blob/master/shell.nix

with import <nixpkgs> {
  config = {
    allowUnsupportedSystem = true;
  };
}; 

let
  avrbinutils = pkgsCross.avr.buildPackages.binutils;
  avrlibc = pkgsCross.avr.libcCross;

  avr_incflags = [
    "-isystem ${avrlibc}/avr/include"
    "-B${avrlibc}/avr/lib/avr5"
    "-L${avrlibc}/avr/lib/avr5"
    "-B${avrlibc}/avr/lib/avr35"
    "-L${avrlibc}/avr/lib/avr35"
    "-B${avrlibc}/avr/lib/avr51"
    "-L${avrlibc}/avr/lib/avr51"
  ];

  avrgcc = pkgsCross.avr.buildPackages.gcc.overrideAttrs (oldAttrs: rec {
    name = "avr-gcc-8.1.0";
    src = fetchurl {
      url = "mirror://gcc/releases/gcc-8.1.0/gcc-8.1.0.tar.xz";
      sha256 = "0lxil8x0jjx7zbf90cy1rli650akaa6hpk8wk8s62vk2jbwnc60x";
    };
  });
in 

stdenv.mkDerivation {
  name = "tmk_keyboard";

  buildInputs = [
    diffutils
    dfu-programmer
    dfu-util
    avrbinutils
    avrgcc
    avrlibc
    avrdude
  ];

  CFLAGS = avr_incflags;
  ASFLAGS = avr_incflags;
}
