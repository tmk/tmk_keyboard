# Target name
TARGET ?= fc660c

# Location of tmk_core
TMK_DIR ?= ../../tmk_core

# Location of target dependent files exist
TARGET_DIR ?= .


# List C source files
SRC ?=	fc660c.c

# Configure file
CONFIG_H ?= config.h


# MCU name
MCU ?= atmega32u4

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency in Hz. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#
#     This will be an integer division of F_USB below, as it is sourced by
#     F_USB after it has run through any CPU prescalers. Note that this value
#     does not *change* the processor frequency - it should merely be updated to
#     reflect the processor speed set externally so that the code can use accurate
#     software delays.
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

# Interrupt driven control endpoint task
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT


# Boot Section Size in *bytes*
#   Teensy halfKay   512
#   Teensy++ halfKay 1024
#   Atmel DFU loader 4096	(TMK Alt Controller)
#   LUFA bootloader  4096
#   USBaspLoader     2048
OPT_DEFS += -DBOOTLOADER_SIZE=4096


# Build Options
#   comment out to disable the options.
#
#BOOTMAGIC_ENABLE ?= yes	# Virtual DIP switch configuration
MOUSEKEY_ENABLE ?= yes		# Mouse keys
EXTRAKEY_ENABLE ?= yes		# Audio control and System control
CONSOLE_ENABLE ?= yes		# Console for debug
COMMAND_ENABLE ?= yes    	# Commands for debug and configuration
NKRO_ENABLE ?= yes		# USB Nkey Rollover
#ACTIONMAP_ENABLE ?= yes		# Use 16bit actionmap instead of 8bit keymap
UNIMAP_ENABLE ?= yes		# Universal keymap
KEYMAP_SECTION_ENABLE ?= yes	# fixed address keymap for keymap editor

#OPT_DEFS += -DNO_ACTION_TAPPING
#OPT_DEFS += -DNO_ACTION_LAYER
#OPT_DEFS += -DNO_ACTION_MACRO


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


# Search Path
VPATH += $(TARGET_DIR)
VPATH += $(TMK_DIR)

include $(TMK_DIR)/protocol/lufa.mk
include $(TMK_DIR)/common.mk
include $(TMK_DIR)/rules.mk
