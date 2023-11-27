TARGET ?= nokeyboard
TMK_DIR ?= ../../tmk_core
TARGET_DIR ?= .

SRC ?= $(TARGET).c
CONFIG_H ?= config.h

# MCU name
MCU ?= atmega32u4
#MCU ?= atmega32u2

# Processor frequency.
F_CPU ?= 16000000

#
# LUFA specific
#
ARCH ?= AVR8
F_USB ?= $(F_CPU)

# Interrupt driven control endpoint task
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT

#
# Build Options
#
MOUSEKEY_ENABLE ?= no	# Mouse keys
EXTRAKEY_ENABLE ?= no	# Media control and System control
CONSOLE_ENABLE ?= yes	# Console for debug
COMMAND_ENABLE ?= no    # Commands for debug and configuration
NKRO_ENABLE ?= no	# USB Nkey Rollover
NO_KEYBOARD ?= yes 	# No keyboard interface
NO_DEBUG ?= yes

# Boot Section Size in bytes
OPT_DEFS += -DBOOTLOADER_SIZE=4096

#OPT_DEFS += -DNO_DEBUG


# Search Path
VPATH += $(TARGET_DIR)
VPATH += $(TMK_DIR)

include $(TMK_DIR)/protocol/lufa.mk
include $(TMK_DIR)/common.mk
include $(TMK_DIR)/rules.mk
