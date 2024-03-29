# Target file name (without extension).
TARGET ?= next_usb

# Directory common source filess exist
TMK_DIR ?= ../../tmk_core

# Directory keyboard dependent files exist
TARGET_DIR ?= .

# keyboard dependent files
SRC ?=  matrix.c \
	led.c

CONFIG_H ?= config.h

#
# Keymap file
#
ifeq (yes,$(strip $(UNIMAP_ENABLE)))
    KEYMAP_FILE = unimap
else
    ifeq (yes,$(strip $(ACTIONMAP_ENABLE)))
        KEYMAP_FILE = actionmap
    else
        KEYMAP_FILE = keymap
    endif
endif
ifdef KEYMAP
    SRC := $(KEYMAP_FILE)_$(KEYMAP).c $(SRC)
else
    SRC := $(KEYMAP_FILE).c $(SRC)
endif


# MCU name, you MUST set this to match the board you are using
# type "make clean" after changing this, so all files will be rebuilt
#MCU = at90usb162       # Teensy 1.0
#MCU = atmega32u4       # Teensy 2.0
#MCU = at90usb646       # Teensy++ 1.0
#MCU = at90usb1286      # Teensy++ 2.0
MCU ?= atmega32u2       # TMK converter


# Processor frequency.
#   Normally the first thing your program should do is set the clock prescaler,
#   so your program will run at the correct speed.  You should also set this
#   variable to same clock speed.  The _delay_ms() macro uses this, and many
#   examples use this variable to calculate timings.  Do not add a "UL" here.
F_CPU ?= 16000000


#
# LUFA specific
#
# Target architecture (see library "Board Types" documentation).
ARCH ?= AVR8

# Input clock frequency.
#     This will define a symbol, F_USB, in all source code files equal to the
#     input clock frequency (before any prescaling is performed) in Hz. This value may
#     differ from F_CPU if prescaling is used on the latter, and is required as the
#     raw input clock is fed directly to the PLL sections of the AVR for high speed
#     clock generation for the USB and other AVR subsections. Do NOT tack on a 'UL'
#     at the end, this will be done automatically to create a 32-bit value in your
#     source code.
#
#     If no clock division is performed on the input clock inside the AVR (via the
#     CPU clock adjust registers or the clock division fuses), this will be equal to F_CPU.
F_USB ?= $(F_CPU)


# Boot Section Size in *bytes*
#   Teensy halfKay   512
#   Teensy++ halfKay 1024
#   Atmel DFU loader 4096
#   LUFA bootloader  4096
#   USBaspLoader     2048
OPT_DEFS += -DBOOTLOADER_SIZE=4096


# Build Options
#   *Comment out* to disable the options.
#
#BOOTMAGIC_ENABLE = yes	# Virtual DIP switch configuration(+1000)
MOUSEKEY_ENABLE ?= yes	# Mouse keys
EXTRAKEY_ENABLE ?= yes	# Audio control and System control
CONSOLE_ENABLE ?= yes	# Console for debug(+400)
COMMAND_ENABLE ?= yes	# Commands for debug and configuration
#NKRO_ENABLE ?= yes	# USB Nkey Rollover

SRC += next_kbd.c


# Search Path
VPATH += $(TARGET_DIR)
VPATH += $(TMK_DIR)

# Suspend mode:
#    Mode              Power consumption       RemoteWakeup        Replug during suspend
#    -----------------------------------------------------------------------------------
#    STANDBY           10mA                    NG                  OK
#    POWER_DOWN        10mA                    OK                  OK
#    IDLE              23mA                    OK                  OK
#    NOPOWERSAVE       23mA                    OK                  OK
#    NO_POWER_DOWN     23mA                    OK                  OK
# RemoteWakeup does not work with STANDBY due to NeXT query&response protocol.
# Use Power Down mode(default).
#OPT_DEFS += -DSUSPEND_MODE_STANDBY

include $(TMK_DIR)/protocol.mk
include $(TMK_DIR)/protocol/lufa.mk
include $(TMK_DIR)/common.mk
include $(TMK_DIR)/rules.mk
