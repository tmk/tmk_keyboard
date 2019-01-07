RN42_DIR = rn42

SRC +=  serial_uart.c \
	rn42/suart.S \
	rn42/rn42.c \
	rn42/rn42_task.c \
	rn42/main.c
	
ifeq (yes,$(strip $(BATT_ENABLE)))
SRC +=  rn42/battery.c
endif

OPT_DEFS += -DPROTOCOL_RN42

VPATH += $(RN42_DIR)
