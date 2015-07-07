# Programming TMK Firmware

So you've decided to build a custom keyboard and now that you've assembled it, what's next? Hasu/TMK's open source keyboard firmware is a great tool to get started with programming your keyboard and adding tons of functionality through the use of layers. 

## Step 1: Setting Up Your Environment

The easiest way to start programming your keyboard is to use a Linux environment. While this is all possible in Windows, my preferred poison is Linux and I'm going to be using Arch for this.

Firstly, install avr-gcc through your favorite package manager. On Arch it'll look like this:

`sudo pacman -S avr-gcc`

Follow the steps to get it installed on your machine.

Next, make sure you have git installed. If you don't, install it through your package manager and read a git tutorial to get startred.

Next, you're going to want to clone or fork TMK's keyboard repository:

`git clone https://github.com/tmk/tmk_keyboard.git`

Now, we can finally get started with creating our custom layout.

## Step 2: Creating a layout

To get started, cd into the directory that corresponds with whatever keyboard you're going to be programming. For me, it's the Alps64 board for the 60% Alps PCB Group Buy that Hasu just ran.

`cd keyboard/alps64`

Inside, we'll see we have a Makefile, a bunch of *.c files, and a few header files. We're going to be creating our own keymap source file.

Create a new keymap file, call it anything you want. Since I'm going to be basing my layout on the KBT Pure, I'm going to be calling it keymap_pure.c.

Inside, first thing we will need to include is the keymap_common.h header file.

`#include "keymap_common.h"`

Next, we're going to get started working on our layers.

We're going to be creating an const array:

`const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {}`

and inside the array we'll be putting the mapping for our layers.

Each layer will be set up as follows:
```
[0] = KEYMAP_AEK( \
    ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC, \
    TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSLS, \
    CAPS,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN, QUOT,ENT, \
    LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH, RSFT, \
    LCTL,LGUI,LALT,          SPC,                          FN0,RALT, RCTL),
```
Where [0] represents the layer itself, and each row of the keyboard is marked off by a '\' at the end. The documentation for the keys can be found in the \doc\keymap.md and \doc\keymap.txt files.

Now, since this is a 60% we'll need at least one more layer to implement the bare essential functionality of the keyboard. We'll be adding the next layer as follows:
```
[1] = KEYMAP_AEK( \
        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, DEL, \
        TRNS ,TRNS, TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,INS,UP,PSCR,VOLD,VOLU,MUTE, \
        TRNS ,TRNS,TRNS,TRNS,TRNS,  TRNS,  TRNS,PGUP,LEFT,DOWN,RGHT,TRNS, TRNS, \
        TRNS,TRNS,  TRNS,  TRNS,  TRNS,  TRNS,  TRNS,PGDN,HOME, END,PAUS,TRNS, \
        TRNS,TRNS,TRNS,          TRNS,                         TRNS,TRNS,TRNS)
```
Here, TRNS indicates that when the FN0 key is pressed, the key is transparent and falls through to the layer underneath.

Now that we have our layers fully programmed, we need to set the behavior for our layer.
```
const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_MOMENTARY(1),                  // Pure Layer
};
```
This indicates that we're going to be setting the Fn key to momentarially switch to the next layer. That is, the layer will only stay active when we have our finger on the key.

Now that that's done, we can get started with compiling and deploying.

## Step 3: Making

Making couldn't be easy enough. Open the Makefile and edit the SRC to pull the source from your new file.
```
ifdef KEYMAP
    SRC := keymap_$(KEYMAP).c $(SRC)
else
    SRC := keymap_pure.c $(SRC)
endif
```
To compile, first run

`make -f Makefile clean`

to ensure no files from previous makes exist.

Then, simply run

`make -f Makefile `

If all goes well, you'll be seeing a bunch of output files in that directory.

And that's it! Deploy to your keyboard following the steps in the build readme and you should be all set.

Full source is available on my [github](https://github.com/thisisshi/tmk_keyboard)