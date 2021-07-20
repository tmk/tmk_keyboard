TARGET ?= usb_desc_dump
TMK_DIR ?= ../../tmk_core
TARGET_DIR ?= .

SRC ?= ino.cpp
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
NO_KEYBOARD ?= yes 	# No keyboard interface

# Boot Section Size in bytes
OPT_DEFS += -DBOOTLOADER_SIZE=4096
OPT_DEFS += -DNO_DEBUG

EXTRACPPFLAGS = -fpermissive

# program Leonardo
PROGRAM_CMD = avrdude -p$(MCU) -cavr109 -b57600 -Uflash:w:$(TARGET).hex -P$(DEV)

# Search Path
VPATH += $(TARGET_DIR)
VPATH += $(TMK_DIR)



#
# USB_desc_dump build setting
#
include $(TMK_DIR)/protocol/usb_hid.mk

USB_DESC_DUMP_DIR = $(USB_HOST_SHIELD_DIR)/examples/USB_desc_dump
SRC += $(USB_DESC_DUMP_DIR)/USB_desc_dump.cpp
SRC += $(USB_HOST_SHIELD_DIR)/hidescriptorparser.cpp

# Print Standard descriptor
OPT_DEFS += -DPRINT_DESC
#OPT_DEFS += -DNO_PRINT_DESC

# Print High-speed Hub descriptor
#OPT_DEFS += -DPRINT_DESC_HSHUB
OPT_DEFS += -DNO_PRINT_DESC_HSHUB

# Print HID Report descriptor
#OPT_DEFS += -DPRINT_DESC_REPORT
OPT_DEFS += -DNO_PRINT_DESC_REPORT



include $(TMK_DIR)/protocol/lufa.mk
include $(TMK_DIR)/common.mk
include $(TMK_DIR)/rules.mk



$(OBJDIR)/$(USB_DESC_DUMP_DIR)/USB_desc_dump.cpp.o : $(OBJDIR)/$(USB_DESC_DUMP_DIR)/USB_desc_dump.cpp $(OBJDIR)/$(USB_DESC_DUMP_DIR)/desc.h
	@echo
	mkdir -p $(@D)
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@

$(OBJDIR)/$(USB_DESC_DUMP_DIR)/USB_desc_dump.cpp : $(TMK_DIR)/$(USB_DESC_DUMP_DIR)/USB_desc_dump.ino
	@echo
	mkdir -p $(@D)
	$(COPY) $< $@

$(OBJDIR)/$(USB_DESC_DUMP_DIR)/desc.h : $(TMK_DIR)/$(USB_DESC_DUMP_DIR)/desc.h
	@echo
	mkdir -p $(@D)
	$(COPY) $< $@
