TARGET ?= pana_m8
TARGET_DIR ?= .
TMK_DIR ?= ../../tmk_core

MCU ?= atmega32u4
F_CPU ?= 16000000
# Boot Section Size in *bytes*
OPT_DEFS += -DBOOTLOADER_SIZE=4096

# LUFA specific
ARCH ?= AVR8
F_USB ?= $(F_CPU)
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT


# Build Options
#BOOTMAGIC_ENABLE ?= yes	# Virtual DIP switch configuration
#MOUSEKEY_ENABLE ?= yes		# Mouse keys
#EXTRAKEY_ENABLE ?= yes		# Audio control and System control
CONSOLE_ENABLE ?= yes		# Console for debug
COMMAND_ENABLE ?= yes    	# Commands for debug and configuration
#NKRO_ENABLE ?= yes		# USB Nkey Rollover
#ACTIONMAP_ENABLE ?= yes		# Use 16bit actionmap instead of 8bit keymap
UNIMAP_ENABLE ?= yes		# Universal keymap
KEYMAP_SECTION_ENABLE ?= yes	# fixed address keymap for keymap editor


# List C source files
SRC ?=	pana_m8.c

CONFIG_H ?= config.h


# Keymap file
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
