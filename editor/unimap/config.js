keymap_config = {
    hhkb: {
        desc: "HHKB Pro1/Pro2 Controller",
        firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/hhkb/binary/hhkb_unimap.hex",
        firmware_format: "ihex",
    },
    hhkb_jp: {
        desc: "HHKB JP Controller",
        firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/hhkb/binary/hhkb_jp_unimap.hex",
        firmware_format: "ihex",
    },
    adb_usb: {
        desc: "ADB-USB Converter(new; ATMega32u2)",
        id: "adb_usb",
        firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/adb_usb/binary/adb_usb_rev2_unimap.hex",
        firmware_format: "ihex",
    },
    adb_usb_rev1: {
        desc: "ADB-USB Converter rev.1(old; ATMega32u4)",
        firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/adb_usb/binary/adb_usb_rev1_unimap.hex",
        firmware_format: "ihex",
    },
    usb_usb: {
        desc: "USB-USB Converter",
        firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/usb_usb/binary/usb_usb_unimap.hex",
        firmware_format: "ihex",
    },
}
