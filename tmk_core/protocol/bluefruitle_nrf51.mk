BLUEFRUIT_DIR = protocol/bluefruitle_nrf51
PJRC_DIR = protocol/pjrc
#
#
# Arduino fake
#
DUMMY_DIR = $(BLUEFRUIT_DIR)/dummy
ARDUINO_DIR = $(BLUEFRUIT_DIR)/arduino-1.6.17
ARDUINO_CORES_DIR = $(ARDUINO_DIR)/cores/arduino
ARDUINO_CORES_SRC = \
	$(ARDUINO_CORES_DIR)/Print.cpp \
	$(ARDUINO_CORES_DIR)/Stream.cpp \
	$(ARDUINO_CORES_DIR)/WMath.cpp \
	$(ARDUINO_CORES_DIR)/wiring_digital.cpp \
	$(ARDUINO_DIR)/libraries/SPI/src/SPI.cpp

# replace arduino/CDC.cpp
SRC += $(DUMMY_DIR)/override_Serial.cpp

# replace arduino/wiring.c
SRC += $(DUMMY_DIR)/override_wiring.c
SRC += common/avr/timer.c
SRC += $(ARDUINO_CORES_SRC)
OPT_DEFS += -DARDUINO=101
#
# Search Path
#
VPATH += $(TMK_DIR)/$(USB_HID_DIR)
VPATH += $(TMK_DIR)/$(USB_HOST_SHIELD_DIR)

# for #include "Arduino.h"
VPATH += $(TMK_DIR)/$(ARDUINO_CORES_DIR)

# for #include "pins_arduino.h"
VPATH += $(TMK_DIR)/$(ARDUINO_DIR)/variants/leonardo

# for SPI
VPATH += $(TMK_DIR)/$(ARDUINO_DIR)/libraries/SPI/src

# ad hoc workaround for compile problem on Windows:
#    Windows doesn't know difference between common/print.h and arduino/Print.h.
#    On Linux no problem.
#    Change file name common/print.h to console.h ?
VPATH := $(TMK_DIR)/common $(VPATH)


SRC +=	$(BLUEFRUIT_DIR)/main.cpp \
	$(BLUEFRUIT_DIR)/bluefruitle.cpp \
	$(BLUEFRUIT_DIR)/Adafruit_ATParser.cpp \
	$(BLUEFRUIT_DIR)/Adafruit_BLE.cpp \
	$(BLUEFRUIT_DIR)/Adafruit_BLEBattery.cpp \
	$(BLUEFRUIT_DIR)/Adafruit_BLEEddystone.cpp \
	$(BLUEFRUIT_DIR)/Adafruit_BLEGatt.cpp \
	$(BLUEFRUIT_DIR)/utility/Adafruit_FIFO.cpp \
	$(BLUEFRUIT_DIR)/Adafruit_BluefruitLE_SPI.cpp \
	$(PJRC_DIR)/pjrc.c \
	$(PJRC_DIR)/usb_keyboard.c \
	$(PJRC_DIR)/usb_debug.c \
	$(PJRC_DIR)/usb.c

# Option modules
ifdef $(or MOUSEKEY_ENABLE, PS2_MOUSE_ENABLE)
    SRC += $(PJRC_DIR)/usb_mouse.c
endif

ifdef EXTRAKEY_ENABLE
    SRC += $(PJRC_DIR)/usb_extra.c
endif

# Search Path
VPATH += $(TMK_DIR)/$(BLUEFRUIT_DIR)
#VPATH += $(TMK_DIR)/$(BLUEFRUIT_DIR)/usb_debug_only
VPATH += $(TMK_DIR)/$(PJRC_DIR)

OPT_DEFS += -DPROTOCOL_BLUEFRUITLE
OPT_DEFS += -DPROTOCOL_PJRC

