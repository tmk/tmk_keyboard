TARGET ?= usb_desc_dump
TMK_DIR ?= ../../tmk_core
TARGET_DIR ?= .
SRC ?=  desc_dump.cpp
CONFIG_H ?= config.h

# MCU name
MCU ?= atmega32u4
# Processor frequency.
F_CPU ?= 16000000

#
# LUFA specific
#
ARCH ?= AVR8
F_USB ?= $(F_CPU)
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT

#
# Build Options
#
MOUSEKEY_ENABLE ?= no	# Mouse keys
EXTRAKEY_ENABLE ?= no	# Media control and System control
CONSOLE_ENABLE ?= yes	# Console for debug
COMMAND_ENABLE ?= no    # Commands for debug and configuration
NKRO_ENABLE ?= no	# USB Nkey Rollover

# Boot Section Size in bytes
OPT_DEFS += -DBOOTLOADER_SIZE=4096
OPT_DEFS += -DNO_ACTION_TAPPING
OPT_DEFS += -DNO_ACTION_LAYER
OPT_DEFS += -DNO_ACTION_MACRO
OPT_DEFS += -DNO_DEBUG

#LDFLAGS += -Wl,--relax

# program Leonardo
PROGRAM_CMD = avrdude -p$(MCU) -cavr109 -b57600 -Uflash:w:$(TARGET).hex -P$(DEV)

# Search Path
VPATH += $(TARGET_DIR)
VPATH += $(TMK_DIR)

include $(TMK_DIR)/protocol/usb_hid.mk
include $(TMK_DIR)/protocol/lufa.mk
include $(TMK_DIR)/common.mk
include $(TMK_DIR)/rules.mk
