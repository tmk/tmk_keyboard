MA42 keyboard firmware
======================
DIY compact keyboard designed for Dvorak and run by Michael (Joey Quinn) and Adam (DaSchwa).

## Build
Move to this directory then just run `make` like:

    $ make

## Flash
Compile and flashes firmware with `dfu-programmer`.

	$ make dfu

## Keymap
Several version of keymap are available in advance but you are recommended to define your favorite layout yourself. To define your own keymap create file named `keymap_<name>.c` and see keymap document(you can find in top README.md) and existent keymap files.

To build firmware binary hex file with a certain keymap just do `make` with `KEYMAP` option like:

    $ make KEYMAP=[ma42|michael|adam|<name>]

## Default Layout

### Layer 0
	,---------------------------------------------------.
	|Esc |  '|  ,|  .|  P|  Y|  F|  G|  C|  R|  L| Bksp |
	|---------------------------------------------------|
	|Tab  |  A|  O|  E|  U|  I|  D|  H|  T|  N|  S|  -  |
	|---------------------------------------------------|
	|LSFT |  ;|  Q|  J|  K|  X|  B|  M|  W|  V|  Z| RSFT|
	|---------------------------------------------------|
	|LCTL|Alt| GUI |   RET   |   SPC  | Fn |GUI|ALT|RCTL|
	`---------------------------------------------------'
	
### Fn Layer 1
	,---------------------------------------------------.
	|  ` |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|   \  |
	|---------------------------------------------------|
	|Tab  |   |   |   |   |   |   |   |   |  [|  ]|  /  |
	|---------------------------------------------------|
	|LSFT |   |   |   |   |   |   |   |  -|  =| UP| RSFT|
	|---------------------------------------------------|
	|LCTL|Alt| GUI |   RET   |   SPC  | Fn |LFT|DWN|RGHT|
	`---------------------------------------------------'

