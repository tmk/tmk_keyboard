CONFIG = {
    layout_default:  "layout-128key.html",
    keymap: {
        /*
         * HHKB Alt Controller
         */
        hhkb: {
            desc: "HHKB Pro1/Pro2 Controller USB",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/hhkb/binary/hhkb_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-hhkb.html",
        },
        hhkb_rn42: {
            desc: "HHKB Pro1/Pro2 Controller Bluetooth",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/hhkb/binary/hhkb_rn42_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-hhkb.html",
        },
        hhkb_jp: {
            desc: "HHKB JP Controller USB",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/hhkb/binary/hhkb_jp_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-hhkb_jp.html",
        },
        hhkb_jp_rn42: {
            desc: "HHKB JP Controller Bluetooth",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/hhkb/binary/hhkb_jp_rn42_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-hhkb_jp.html",
        },
        /*
         * FC660C Alt Controller
         */
        fc660c: {
            desc: "FC660C Controller - Plain",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/fc660c/binary/fc660c.hex",
            firmware_format: "ihex",
            layout: "layout-fc660c.html",
        },
        fc660c_emu: {
            desc: "FC660C Controller - Origianl keymap emulation",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/fc660c/binary/fc660c_emu.hex",
            firmware_format: "ihex",
            layout: "layout-fc660c.html",
        },
        /*
         * FC980C Alt Controller
         */
        fc980c: {
            desc: "FC980C Controller - Plain",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/fc980c/binary/fc980c.hex",
            firmware_format: "ihex",
            layout: "layout-fc980c.html",
        },
        /*
         * ADB-USB
         */
        adb_usb: {
            desc: "ADB-USB Converter rev.2",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/adb_usb/binary/adb_usb_rev2_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-adb_usb.html",
        },
        adb_usb_rev1: {
            desc: "ADB-USB Converter rev.1",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/adb_usb/binary/adb_usb_rev1_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-adb_usb.html",
        },
        /*
         * USB-USB
         */
        usb_usb: {
            desc: "USB-USB Converter",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/usb_usb/binary/usb_usb_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-128key.html",
        },
        /*
         * NeXT-USB
         */
        next_usb: {
            desc: "NeXT-USB Converter rev.2",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/next_usb/binary/next_usb_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-next_usb.html",
        },
        /*
         * M0110-USB
         */
        /* ATmega32U2 */
        m0110_usb: {
            desc: "M0110-USB Converter rev.2 for M0110",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/m0110_usb/binary/m0110_usb_rev2_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-m0110_usb.html",
        },
        m0110_usb_m0110a: {
            desc: "M0110-USB Converter rev.2 for M0110A",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/m0110_usb/binary/m0110_usb_rev2_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-m0110_usb_m0110a.html",
        },
        /* ATmega32U4 */
        m0110_usb_rev1: {
            desc: "M0110-USB Converter rev.1 for M0110",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/m0110_usb/binary/m0110_usb_rev1_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-m0110_usb.html",
        },
        m0110_usb_rev1_m0110a: {
            desc: "M0110-USB Converter rev.1 for M0110A",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/m0110_usb/binary/m0110_usb_rev1_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-m0110_usb_m0110a.html",
        },
        /*
         * PS2-USB
         */
        /* ATmega32U2 */
        ps2_usb: {
            desc: "PS/2-USB Converter rev.2",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ps2_usb/binary/ps2_usb_rev2_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ps2_usb.html",
        },
        /* ATmega32U4 */
        ps2_usb_rev1: {
            desc: "PS/2-USB Converter rev.1",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ps2_usb/binary/ps2_usb_rev1_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ps2_usb.html",
        },
        /*
         * IBM4704-USB
         */
        /* ATmega32U2 */
        ibm4704_usb: {
            desc: "IBM4704-USB Converter rev.2 for 62/77/107-key",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibm4704_usb/binary/ibm4704_usb_rev2_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ibm4704_usb_107key.html",
        },
        ibm4704_usb_50key: {
            desc: "IBM4704-USB Converter rev.2 for 50-key",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibm4704_usb/binary/ibm4704_usb_rev2_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ibm4704_usb_50key.html",
        },
        ibm4704_usb_alps: {
            desc: "IBM4704-USB Converter rev.2 for Alps 102-key",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibm4704_usb/binary/ibm4704_usb_rev2_alps_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ibm4704_usb_alps102key.html",
        },
        /* ATmega32U4 */
        ibm4704_usb_rev1: {
            desc: "IBM4704-USB Converter rev.1 for 62/77/107-key",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibm4704_usb/binary/ibm4704_usb_rev1_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ibm4704_usb_107key.html",
        },
        ibm4704_usb_rev1_50key: {
            desc: "IBM4704-USB Converter rev.1 for 50-key",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibm4704_usb/binary/ibm4704_usb_rev1_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ibm4704_usb_50key.html",
        },
        ibm4704_usb_rev1_alps: {
            desc: "IBM4704-USB Converter rev.1 for Alps 102-key",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibm4704_usb/binary/ibm4704_usb_rev1_alps_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-ibm4704_usb_alps102key.html",
        },
        /*
         * Alps64 PCB
         */
        /* ATmega32U2 */
        alps64: {
            desc: "Alps64 PCB",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/keyboard/alps64/binary/alps64_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-alps64.html",
        },
        /*
         * X68K-USB
         */
        /* ATmega32U2 */
        x68k_usb: {
            desc: "X68000-USB Converter(TMK)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/x68k_usb/binary/x68k_usb_unimap.hex",
            firmware_format: "ihex",
            layout: "layout-x68k_usb.html",
        },
        /*
         * SUN-USB
         */
        sun_usb: {
            desc: "SUN-USB Converter(TMK)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/sun_usb/binary/sun_usb.hex",
            firmware_format: "ihex",
            layout: "layout-sun_type5.html",
        },
        sun_usb_32u4: {
            desc: "SUN-USB Converter(ATmega32U4)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/sun_usb/binary/sun_usb_atmega32u4.hex",
            firmware_format: "ihex",
            layout: "layout-sun_type5.html",
        },
        /*
         * IBMPC-USB
         */
        /* ATmega32U2 */
        ibmpc_usb: {
            desc: "IBMPC-USB Converter(TMK/ATmega32U2)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u2.hex",
            firmware_format: "ihex",
            layout: "layout-ibmpc_full.html",
        },
        ibmpc_usb_xt: {
            desc: "IBMPC-USB Converter(TMK/ATmega32U2) - 83-key XT",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u2.hex",
            firmware_format: "ihex",
            layout: "layout-83key_xt.html",
        },
        ibmpc_usb_at: {
            desc: "IBMPC-USB Converter(TMK/ATmega32U2) - 84-key AT",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u2.hex",
            firmware_format: "ihex",
            layout: "layout-84key_at.html",
        },
        ibmpc_usb_terminal: {
            desc: "IBMPC-USB Converter(TMK/ATmega32U2) - 122-key Terminal",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u2.hex",
            firmware_format: "ihex",
            layout: "layout-122key_terminal.html",
        },
        /* ATmega32U4 */
        ibmpc_usb_32u4: {
            desc: "IBMPC-USB Converter(ATmega32U4)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u4.hex",
            firmware_format: "ihex",
            layout: "layout-ibmpc_full.html",
        },
        ibmpc_usb_32u4_xt: {
            desc: "IBMPC-USB Converter(ATmega32U4) - 83-key XT",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u4.hex",
            firmware_format: "ihex",
            layout: "layout-83key_xt.html",
        },
        ibmpc_usb_32u4_at: {
            desc: "IBMPC-USB Converter(ATmega32U4) - 84-key AT",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u4.hex",
            firmware_format: "ihex",
            layout: "layout-84key_at.html",
        },
        ibmpc_usb_32u4_terminal: {
            desc: "IBMPC-USB Converter(ATmega32U4) - 122-key Terminal",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_atmega32u4.hex",
            firmware_format: "ihex",
            layout: "layout-122key_terminal.html",
        },
        /* AT90USB1286 */
        ibmpc_usb_1286: {
            desc: "IBMPC-USB Converter(AT90USB1286)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_at90usb1286.hex",
            firmware_format: "ihex",
            layout: "layout-ibmpc_full.html",
        },
        ibmpc_usb_1286_xt: {
            desc: "IBMPC-USB Converter(AT90USB1286) - 83-key XT",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_at90usb1286.hex",
            firmware_format: "ihex",
            layout: "layout-83key_xt.html",
        },
        ibmpc_usb_1286_at: {
            desc: "IBMPC-USB Converter(AT90USB1286) - 84-key AT",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_at90usb1286.hex",
            firmware_format: "ihex",
            layout: "layout-84key_at.html",
        },
        ibmpc_usb_1286_terminal: {
            desc: "IBMPC-USB Converter(AT90USB1286) - 122-key Terminal",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/ibmpc_usb/binary/ibmpc_usb_at90usb1286.hex",
            firmware_format: "ihex",
            layout: "layout-122key_terminal.html",
        },
        /*
         * PC98
         */
        pc98_usb: {
            desc: "PC98-USB Converter(ATmega32U2)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/pc98_usb/binary/pc98_usb_atmega32u2.hex",
            firmware_format: "ihex",
            layout: "layout-pc9801.html",
        },
        pc98_usb_32u4: {
            desc: "PC98-USB Converter(ATmega32U4)",
            firmware_url: "https://raw.githubusercontent.com/tmk/tmk_keyboard/master/converter/pc98_usb/binary/pc98_usb_atmega32u4.hex",
            firmware_format: "ihex",
            layout: "layout-pc9801.html",
        },
    },
};
