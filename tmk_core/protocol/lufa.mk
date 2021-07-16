TMK_LUFA_DIR = protocol/lufa

# Path to the LUFA library
TMK_LUFA_PATH ?= $(TMK_LUFA_DIR)/lufa-abcminiuser


# Version string
TMK_LUFA_VERSION := $(shell (cd $(TMK_DIR)/$(TMK_LUFA_PATH); git rev-parse --short=6 HEAD || echo 'unknown') 2> /dev/null)
OPT_DEFS += -DTMK_LUFA_VERSION=$(TMK_LUFA_VERSION)

# Create the LUFA source path variables by including the LUFA makefile
ifneq (, $(wildcard $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/Build/LUFA/lufa-sources.mk))
        LUFA_PATH = $(TMK_LUFA_PATH)/LUFA
        include $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/Build/LUFA/lufa-sources.mk
else
    $(error LUFA may be too old or not found: try 'git submodule update --init')
#    ifneq (, $(wildcard $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/Build/lufa_sources.mk))
#        # build system from 20120730
#        LUFA_PATH = $(TMK_LUFA_PATH)
#        LUFA_ROOT_PATH = $(TMK_LUFA_PATH)/LUFA
#        include $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/Build/lufa_sources.mk
#    else
#        include $(TMK_DIR)/$(TMK_LUFA_PATH)/LUFA/makefile
#    endif
endif

TMK_LUFA_SRC = $(TMK_LUFA_DIR)/lufa.c \
               $(TMK_LUFA_DIR)/descriptor.c \
               $(LUFA_SRC_USB_DEVICE)

SRC += $(TMK_LUFA_SRC)

# Search Path
VPATH += $(TMK_DIR)/$(TMK_LUFA_DIR)
VPATH += $(TMK_DIR)/$(TMK_LUFA_PATH)

# Option modules
#ifdef $(or MOUSEKEY_ENABLE, PS2_MOUSE_ENABLE)
#endif

#ifdef EXTRAKEY_ENABLE
#endif

# LUFA library compile-time options and predefined tokens
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

ifeq (yes,$(strip $(TMK_LUFA_DEBUG_SUART)))
    SRC += common/avr/suart.S
    TMK_LUFA_OPTS += -DTMK_LUFA_DEBUG_SUART
    # Keep print/debug lines when disabling HID console. See common.mk.
    DEBUG_PRINT_AVAILABLE = yes
endif

ifeq (yes,$(strip $(TMK_LUFA_DEBUG_UART)))
    SRC += common/avr/uart.c
    TMK_LUFA_OPTS += -DTMK_LUFA_DEBUG_UART
    # Keep print/debug lines when disabling HID console. See common.mk.
    DEBUG_PRINT_AVAILABLE = yes
endif


OPT_DEFS += -DF_USB=$(F_USB)UL
OPT_DEFS += -DARCH=ARCH_$(ARCH)
OPT_DEFS += $(TMK_LUFA_OPTS)

# This indicates using LUFA stack
OPT_DEFS += -DPROTOCOL_LUFA
