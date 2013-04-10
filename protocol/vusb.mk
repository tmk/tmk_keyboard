VUSB_DIR = protocol/vusb

OPT_DEFS += -DPROTOCOL_VUSB

SRC +=	$(VUSB_DIR)/main.c \
	$(VUSB_DIR)/vusb.c \
	$(VUSB_DIR)/usbdrv/usbdrv.c \
	$(VUSB_DIR)/usbdrv/usbdrvasm.S \
	$(VUSB_DIR)/usbdrv/oddebug.c


ifdef NO_UART
SRC +=	sendchar_null.c
else
SRC +=	sendchar_uart.c \
	uart.c
endif


# Search Path
VPATH += $(TOP_DIR)/protocol/vusb:$(TOP_DIR)/protocol/vusb/usbdrv
