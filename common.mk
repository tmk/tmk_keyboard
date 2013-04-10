COMMON_DIR = common
SRC +=	$(COMMON_DIR)/host.c \
	$(COMMON_DIR)/keyboard.c \
	$(COMMON_DIR)/action.c \
	$(COMMON_DIR)/action_tapping.c \
	$(COMMON_DIR)/action_oneshot.c \
	$(COMMON_DIR)/action_macro.c \
	$(COMMON_DIR)/action_layer.c \
	$(COMMON_DIR)/keymap.c \
	$(COMMON_DIR)/timer.c \
	$(COMMON_DIR)/print.c \
	$(COMMON_DIR)/bootloader.c \
	$(COMMON_DIR)/suspend.c \
	$(COMMON_DIR)/util.c


# Option modules
ifdef BOOTMAGIC_ENABLE
    SRC += $(COMMON_DIR)/bootmagic.c
    SRC += $(COMMON_DIR)/eeconfig.c
    OPT_DEFS += -DBOOTMAGIC_ENABLE
endif

ifdef MOUSEKEY_ENABLE
    SRC += $(COMMON_DIR)/mousekey.c
    OPT_DEFS += -DMOUSEKEY_ENABLE
endif

ifdef EXTRAKEY_ENABLE
    OPT_DEFS += -DEXTRAKEY_ENABLE
endif

ifdef CONSOLE_ENABLE
    OPT_DEFS += -DCONSOLE_ENABLE
else
    OPT_DEFS += -DNO_PRINT
    OPT_DEFS += -DNO_DEBUG
endif

ifdef COMMAND_ENABLE
    SRC += $(COMMON_DIR)/command.c
    OPT_DEFS += -DCOMMAND_ENABLE
endif

ifdef NKRO_ENABLE
    OPT_DEFS += -DNKRO_ENABLE
endif

ifdef PS2_MOUSE_ENABLE
    SRC += $(COMMON_DIR)/ps2.c \
           $(COMMON_DIR)/ps2_mouse.c
    OPT_DEFS += -DPS2_MOUSE_ENABLE
endif

ifdef $(or MOUSEKEY_ENABLE, PS2_MOUSE_ENABLE)
    OPT_DEFS += -DMOUSE_ENABLE
endif

ifdef SLEEP_LED_ENABLE
    SRC += $(COMMON_DIR)/sleep_led.c
    OPT_DEFS += -DSLEEP_LED_ENABLE
    OPT_DEFS += -DNO_SUSPEND_POWER_DOWN
endif


# Search Path
VPATH += $(TOP_DIR)/common
