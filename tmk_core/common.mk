COMMON_DIR = common
SRC +=	$(COMMON_DIR)/host.c \
	$(COMMON_DIR)/keyboard.c \
	$(COMMON_DIR)/matrix.c \
	$(COMMON_DIR)/action.c \
	$(COMMON_DIR)/action_tapping.c \
	$(COMMON_DIR)/action_macro.c \
	$(COMMON_DIR)/action_layer.c \
	$(COMMON_DIR)/action_util.c \
	$(COMMON_DIR)/print.c \
	$(COMMON_DIR)/debug.c \
	$(COMMON_DIR)/util.c \
	$(COMMON_DIR)/hook.c \
	$(COMMON_DIR)/avr/suspend.c \
	$(COMMON_DIR)/avr/xprintf.S \
	$(COMMON_DIR)/avr/timer.c \
	$(COMMON_DIR)/avr/bootloader.c


ifeq (yes,$(strip $(NO_KEYBOARD)))
    OPT_DEFS += -DNO_KEYBOARD
endif

ifeq (yes,$(strip $(UNIMAP_ENABLE)))
    SRC += $(COMMON_DIR)/unimap.c
    OPT_DEFS += -DUNIMAP_ENABLE
    OPT_DEFS += -DACTIONMAP_ENABLE
else
    ifeq (yes,$(strip $(ACTIONMAP_ENABLE)))
	SRC += $(COMMON_DIR)/actionmap.c
	OPT_DEFS += -DACTIONMAP_ENABLE
    else
	SRC += $(COMMON_DIR)/keymap.c
    endif
endif

ifeq (yes,$(strip $(BOOTMAGIC_ENABLE)))
    SRC += $(COMMON_DIR)/bootmagic.c
    SRC += $(COMMON_DIR)/avr/eeconfig.c
    OPT_DEFS += -DBOOTMAGIC_ENABLE
endif

ifeq (yes,$(strip $(MOUSEKEY_ENABLE)))
    SRC += $(COMMON_DIR)/mousekey.c
    OPT_DEFS += -DMOUSEKEY_ENABLE
    OPT_DEFS += -DMOUSE_ENABLE
endif

ifeq (yes,$(strip $(EXTRAKEY_ENABLE)))
    OPT_DEFS += -DEXTRAKEY_ENABLE
endif

ifeq (yes,$(strip $(CONSOLE_ENABLE)))
    OPT_DEFS += -DCONSOLE_ENABLE
else
    # Remove print functions when console is disabled and
    # no other print method like UART is available
    ifneq (yes, $(strip $(DEBUG_PRINT_AVAILABLE)))
	OPT_DEFS += -DNO_PRINT
	OPT_DEFS += -DNO_DEBUG
    endif
endif

ifeq (yes,$(strip $(NO_DEBUG)))
    OPT_DEFS += -DNO_DEBUG
endif

ifeq (yes,$(strip $(NO_PRINT)))
    OPT_DEFS += -DNO_PRINT
endif

ifeq (yes,$(strip $(COMMAND_ENABLE)))
    SRC += $(COMMON_DIR)/command.c
    OPT_DEFS += -DCOMMAND_ENABLE
endif

ifeq (yes,$(strip $(NKRO_ENABLE)))
    OPT_DEFS += -DNKRO_ENABLE
endif

ifeq (yes,$(strip $(NKRO_6KRO_ENABLE)))
    OPT_DEFS += -DNKRO_6KRO_ENABLE
endif

ifeq (yes,$(strip $(USB_6KRO_ENABLE)))
    OPT_DEFS += -DUSB_6KRO_ENABLE
endif

ifeq (yes, $(strip $(KEYBOARD_LOCK_ENABLE)))
    OPT_DEFS += -DKEYBOARD_LOCK_ENABLE
endif

ifeq (yes,$(strip $(SLEEP_LED_ENABLE)))
    SRC += $(COMMON_DIR)/avr/sleep_led.c
    OPT_DEFS += -DSLEEP_LED_ENABLE
    OPT_DEFS += -DNO_SUSPEND_POWER_DOWN
endif

ifeq (yes,$(strip $(BACKLIGHT_ENABLE)))
    SRC += $(COMMON_DIR)/backlight.c
    OPT_DEFS += -DBACKLIGHT_ENABLE
endif

ifeq (yes,$(strip $(KEYMAP_SECTION_ENABLE)))
    OPT_DEFS += -DKEYMAP_SECTION_ENABLE

    ifeq ($(strip $(MCU)),atmega32u2)
	EXTRALDFLAGS += -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr35.x
    else ifeq ($(strip $(MCU)),atmega32u4)
	EXTRALDFLAGS += -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5.x
    else ifeq ($(strip $(MCU)),at90usb1286)
	EXTRALDFLAGS += -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr51.x
    else
	EXTRALDFLAGS += $(error no ldscript for keymap section)
    endif
endif

# Version string
TMK_VERSION := $(shell (git rev-parse --short=6 HEAD || echo 'unknown') 2> /dev/null)
OPT_DEFS += -DTMK_VERSION=$(TMK_VERSION)


# Search Path
VPATH += $(TMK_DIR)/common
