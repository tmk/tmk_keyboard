TMK_LUFA_DIR = protocol/lufa

# Path to the LUFA library
TMK_LUFA_PATH ?= $(TMK_LUFA_DIR)/lufa-abcminiuser


# Create the LUFA source path variables by including the LUFA makefile
ifneq (, $(wildcard $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/Build/LUFA/lufa-sources.mk))
        LUFA_PATH = $(TMK_LUFA_PATH)/LUFA
        include $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/Build/LUFA/lufa-sources.mk
else
    $(error LUFA may be too old or not found: try 'git submodule update --init')
endif

TMK_LUFA_SRC = $(TMK_LUFA_DIR)/lufa.c \
               $(TMK_LUFA_DIR)/descriptor.c \
               $(LUFA_SRC_USB_DEVICE)


#
# LUFA library compile-time options and predefined tokens
#
TMK_LUFA_OPTS  = -DUSB_DEVICE_ONLY
TMK_LUFA_OPTS += -DUSE_FLASH_DESCRIPTORS
TMK_LUFA_OPTS += -DUSE_STATIC_OPTIONS="(USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)"

# Do not enable INTERRUPT_CONTROL_ENDPOINT for converters which requires ISR in particular,
# it can block other ISRs long like 500-1000us for HID keyboard LED report.
#TMK_LUFA_OPTS += -DINTERRUPT_CONTROL_ENDPOINT

TMK_LUFA_OPTS += -DFIXED_CONTROL_ENDPOINT_SIZE=8
TMK_LUFA_OPTS += -DFIXED_NUM_CONFIGURATIONS=1

# Remote wakeup fix for ATmega32U2        https://github.com/tmk/tmk_keyboard/issues/361
ifeq ($(MCU),atmega32u2)
    TMK_LUFA_OPTS += -DNO_LIMITED_CONTROLLER_CONNECT
endif

ifeq (yes,$(strip $(TMK_LUFA_DEBUG)))
    TMK_LUFA_OPTS += -DTMK_LUFA_DEBUG
endif


#
# Console Driver
#
ifeq (yes,$(strip $(CONSOLE_ENABLE)))
    ifeq (CDC, $(strip $(CONSOLE_DRIVER)))
        # CDC Serial
        OPT_DEFS += -DCONSOLE_CDC
        TMK_LUFA_OPTS += -DUSB_STREAM_TIMEOUT_MS=1
        TMK_LUFA_SRC += $(LUFA_ROOT_PATH)/Drivers/USB/Class/Device/CDCClassDevice.c
        TMK_LUFA_SRC += $(TMK_LUFA_DIR)/console_cdc.c
    else ifeq (UART, $(strip $(CONSOLE_DRIVER)))
        OPT_DEFS += -DCONSOLE_UART
        TMK_LUFA_SRC += common/avr/uart.c
        TMK_LUFA_SRC += $(TMK_LUFA_DIR)/console_uart.c
    else ifeq (SUART, $(strip $(CONSOLE_DRIVER)))
        OPT_DEFS += -DCONSOLE_SUART
        TMK_LUFA_SRC += common/avr/suart.S
        TMK_LUFA_SRC += $(TMK_LUFA_DIR)/console_suart.c
    else
        # HID hid_listen
        OPT_DEFS += -DCONSOLE_HID
        TMK_LUFA_SRC += $(TMK_LUFA_DIR)/console_hid.c
    endif

    # Wait until console becomes available at startup
    ifeq (yes,$(strip $(CONSOLE_STARTUP_WAIT)))
        OPT_DEFS += -DCONSOLE_STARTUP_WAIT
    endif
endif


OPT_DEFS += -DF_USB=$(F_USB)UL
OPT_DEFS += -DARCH=ARCH_$(ARCH)
OPT_DEFS += -DPROTOCOL_LUFA
OPT_DEFS += $(TMK_LUFA_OPTS)


SRC += $(TMK_LUFA_SRC)

# Search Path
VPATH += $(TMK_DIR)/$(TMK_LUFA_DIR)
VPATH += $(TMK_DIR)/$(TMK_LUFA_PATH)
