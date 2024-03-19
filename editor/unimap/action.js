/**********************************************************************
 * Action code elements
 **********************************************************************/
// Action kind bits
ACT_MODS          = 0;  // 0b0000;
ACT_LMODS         = 0;  // 0b0000;
ACT_RMODS         = 1;  // 0b0001;
ACT_MODS_TAP      = 2;  // 0b0010;
ACT_LMODS_TAP     = 2;  // 0b0010;
ACT_RMODS_TAP     = 3;  // 0b0011;
ACT_USAGE         = 4;  // 0b0100;
ACT_MOUSEKEY      = 5;  // 0b0101;
ACT_LAYER         = 8;  // 0b1000;
ACT_LAYER_EXT     = 9;  // 0b1001;
ACT_LAYER_TAP     = 10; // 0b1010; /* Layer  0-15 */
ACT_LAYER_TAP_EXT = 11; // 0b1011; /* Layer 16-31 */
ACT_MACRO         = 12; // 0b1100;
ACT_BACKLIGHT     = 13; // 0b1101;
ACT_COMMAND       = 14; // 0b1110;
ACT_FUNCTION      = 15; // 0b1111;

// Special keycodes for Layer Tap
OP_TAP_TOGGLE     = 0xf0;
OP_ON_OFF         = 0xf1;
OP_OFF_ON         = 0xf2;
OP_SET_CLEAR      = 0xf3;

// Layer Bit operations
OP_BIT_AND        = 0;
OP_BIT_OR         = 1;
OP_BIT_XOR        = 2;
OP_BIT_SET        = 3;

// Layer Bit operation timings
ON_DEFAULT_LAYER  = 0;
ON_PRESS          = 1;
ON_RELEASE        = 2;
ON_BOTH           = 3;

// Modifier bits
MOD_LCTL = 0x01;
MOD_LSFT = 0x02;
MOD_LALT = 0x04;
MOD_LGUI = 0x08;
MOD_RCTL = 0x11;
MOD_RSFT = 0x12;
MOD_RALT = 0x14;
MOD_RGUI = 0x18;

// Special keycodes for Modifier Tap
MODS_ONESHOT      = 0x00;
MODS_TAP_TOGGLE   = 0x01;

// Actoin Usage Page
PAGE_SYSTEM       = 0;
PAGE_CONSUMER     = 1;

// Apecial opts for Function
FUNC_TAP = 0x08;



/**********************************************************************
 * Action kind codes
 **********************************************************************/
var kind_codes = {
    KEY:                ACT_MODS<<12,
    MODS_KEY:           ACT_MODS<<12,
    MODS_TAP_KEY:       ACT_MODS_TAP<<12,
    MODS_ONESHOT:       ACT_MODS_TAP<<12 | MODS_ONESHOT,
    MODS_TAP_TOGGLE:    ACT_MODS_TAP<<12 | MODS_TAP_TOGGLE,
    USAGE_SYSTEM:       ACT_USAGE<<12 | PAGE_SYSTEM<<10,
    USAGE_CONSUMER:     ACT_USAGE<<12 | PAGE_CONSUMER<<10,
    MOUSEKEY:           ACT_MOUSEKEY<<12,
    MACRO:              ACT_MACRO<<12,
    MACRO_TAP:          ACT_MACRO<<12,
    MACRO_OPT:          ACT_MACRO<<12,
    FUNCTION:           ACT_FUNCTION<<12,
    FUNCTION_TAP:       ACT_FUNCTION<<12 | FUNC_TAP<<8,
    FUNCTION_OPT:       ACT_FUNCTION<<12,
    LAYER_CLEAR:        ACT_LAYER<<12 | OP_BIT_AND<<10,
    LAYER_MOMENTARY:    ACT_LAYER_TAP<<12 | OP_ON_OFF,
    LAYER_TOGGLE:       ACT_LAYER<<12 | OP_BIT_XOR<<10 | ON_RELEASE<<8,
    LAYER_INVERT:       ACT_LAYER<<12 | OP_BIT_XOR<<10,
    LAYER_ON:           ACT_LAYER<<12 | OP_BIT_OR<<10,
    LAYER_OFF:          ACT_LAYER<<12 | OP_BIT_AND<<10,
    LAYER_SET:          ACT_LAYER<<12 | OP_BIT_SET<<10,
    LAYER_TAP_KEY:      ACT_LAYER_TAP<<12,
    LAYER_MODS:         ACT_LAYER_TAP<<12 | 0xc0,
    LAYER_TAP_TOGGLE:   ACT_LAYER_TAP<<12 | OP_TAP_TOGGLE,
    LAYER_ON_OFF:       ACT_LAYER_TAP<<12 | OP_ON_OFF,
    LAYER_OFF_ON:       ACT_LAYER_TAP<<12 | OP_OFF_ON,
    LAYER_SET_CLEAR:    ACT_LAYER_TAP<<12 | OP_SET_CLEAR,
    LAYER_BIT_AND:      ACT_LAYER<<12 | OP_BIT_AND<<10,
    LAYER_BIT_OR:       ACT_LAYER<<12 | OP_BIT_OR<<10,
    LAYER_BIT_XOR:      ACT_LAYER<<12 | OP_BIT_XOR<<10,
    LAYER_BIT_SET:      ACT_LAYER<<12 | OP_BIT_SET<<10,
    DEFAULT_LAYER_SET:  ACT_LAYER<<12 | OP_BIT_SET<<10 | ON_DEFAULT_LAYER<<8,
    COMMAND:            ACT_COMMAND<<12,
};



/**********************************************************************
 * Action Object
 **********************************************************************/
function Action(code) {
    this.code = code;

    Object.defineProperty(this, "kind", {
        set: function(val)  { this.code |= (val & 0xf) << 12; },
        get: function()     { return (this.code & 0xf000) >> 12; }
    });
    // KEY
    Object.defineProperty(this, "key_mods", {
        set: function(val)  { this.code |= (val & 0x1f) << 8; },
        get: function()     { return (this.code & 0x1f00) >> 8; }
    });
    Object.defineProperty(this, "key_code", {
        set: function(val)  { this.code |= (val & 0xff); },
        get: function()     { return (this.code & 0xff); }
    });
    // USAGE
    Object.defineProperty(this, "usage_page", {
        set: function(val)  { this.code |= (val & 0x3) << 10; },
        get: function()     { return (this.code & 0x0c00) >> 10; }
    });
    Object.defineProperty(this, "usage_code", {
        set: function(val)  { this.code |= (val & 0x3ff); },
        get: function()     { return (this.code & 0x3ff); }
    });
    // ACT_LAYER_TAP
    Object.defineProperty(this, "layer_tap_val", {
        set: function(val)  { this.code |= (val & 0x1f) << 8; },
        get: function()     { return (this.code & 0x1f00) >> 8; }
    });
    Object.defineProperty(this, "layer_tap_code", {
        set: function(val)  { this.code |= (val & 0xff); },
        get: function()     { return (this.code & 0xff); }
    });
    // ACT_LAYER(BITOP)
    Object.defineProperty(this, "layer_bitop_op", {
        set: function(val)  { this.code |= (val & 0x3) << 10; },
        get: function()     { return (this.code & 0x0c00) >> 10; }
    });
    Object.defineProperty(this, "layer_bitop_on", {
        set: function(val)  { this.code |= (val & 0x3) << 8; },
        get: function()     { return (this.code & 0x0300) >> 8; }
    });
    Object.defineProperty(this, "layer_bitop_part", {
        set: function(val)  { this.code |= (val & 0x7) << 5; },
        get: function()     { return (this.code & 0x00e0) >> 5; }
    });
    Object.defineProperty(this, "layer_bitop_xbit", {
        set: function(val)  { this.code |= (val & 0x1) << 4; },
        get: function()     { return (this.code & 0x0010) >> 4; }
    });
    Object.defineProperty(this, "layer_bitop_bits", {
        set: function(val)  { this.code |= (val & 0xf); },
        get: function()     { return (this.code & 0x000f); }
    });
    Object.defineProperty(this, "layer_bitop_bits_is_single_1", {
        // bits has just single 1
        get: function()     {
            return (this.layer_bitop_bits == 1) ||
                   (this.layer_bitop_bits == 2) ||
                   (this.layer_bitop_bits == 4) ||
                   (this.layer_bitop_bits == 8);
        }
    });
    Object.defineProperty(this, "layer_bitop_bits_is_single_0", {
        // bits has just single 0
        get: function()     {
            return (this.layer_bitop_bits == (~1 & 0xf)) ||
                   (this.layer_bitop_bits == (~2 & 0xf)) ||
                   (this.layer_bitop_bits == (~4 & 0xf)) ||
                   (this.layer_bitop_bits == (~8 & 0xf));
        }
    });
    Object.defineProperty(this, "layer_bitop_layer", {
        get: function()     {
            if (this.layer_bitop_xbit == 0) {
                return ((this.layer_bitop_bits == 1) ? 0 : (
                        (this.layer_bitop_bits == 2) ? 1 : (
                        (this.layer_bitop_bits == 4) ? 2 : (
                        (this.layer_bitop_bits == 8) ? 3 : 0)))) + this.layer_bitop_part * 4;
            } else {
                return ((this.layer_bitop_bits == (~1 & 0xf)) ? 0 : (
                        (this.layer_bitop_bits == (~2 & 0xf)) ? 1 : (
                        (this.layer_bitop_bits == (~4 & 0xf)) ? 2 : (
                        (this.layer_bitop_bits == (~8 & 0xf)) ? 3 : 0)))) + this.layer_bitop_part * 4;
            }
        }
    });
    Object.defineProperty(this, "layer_bitop_bits_str", {
        get: function()     {
            let shift = this.layer_bitop_part*4;
            let bits_shifted = this.layer_bitop_bits*(2**shift); // bits<<shift
            return "0x" + ("0000000" + bits_shifted.toString(16)).substr(-8).toUpperCase();
        }
    });
    Object.defineProperty(this, "layer_bitop_mask_str", {
        get: function()     {
            let shift = this.layer_bitop_part*4;
            let mask = this.layer_bitop_xbit ?  (0xffffffff - (0xf*(2**shift))) : 0; // ~(0xf<<shift)
            return "0x" + ("0000000" + mask.toString(16)).substr(-8).toUpperCase();
        }
    });
    Object.defineProperty(this, "layer_bitop_masked_bits_str", {
        get: function()     {
            let shift = this.layer_bitop_part*4;
            let bits_shifted = this.layer_bitop_bits*(2**shift); // bits<<shift
            let mask = this.layer_bitop_xbit ?  (0xffffffff - (0xf*(2**shift))) : 0; // ~(0xf<<shift)
            let masked_bits = (bits_shifted + mask); // bits_shifted | mask
            return "0x" + ("0000000" + masked_bits.toString(16)).substr(-8).toUpperCase();
        }
    });
    Object.defineProperty(this, "layer_bitop_on_str", {
        get: function()     {
            return ((this.layer_bitop_on == ON_DEFAULT_LAYER) ? "(default layer)" : (
                    (this.layer_bitop_on == ON_PRESS) ? "on press" : (
                    (this.layer_bitop_on == ON_RELEASE) ? "on release" : (
                    (this.layer_bitop_on == ON_BOTH) ? "on both" : ""))));
        }
    });
    // ACT_MOUSEKEY
    Object.defineProperty(this, "mousekey_code", {
        set: function(val)  { this.code |= (val & 0xff); },
        get: function()     { return (this.code & 0x00ff); }
    });
    // ACT_COMMAND
    Object.defineProperty(this, "command_id", {
        set: function(val)  { this.code |= (val & 0xff); },
        get: function()     { return (this.code & 0x00ff); }
    });


    var mods_str = function(mods) {
        var mods_name = "", mods_desc = []
        if (mods & 0x08)  mods_name += "\u2318", mods_desc.push("Gui");
        if (mods & 0x04)  mods_name += "\u2325", mods_desc.push("Alt");
        if (mods & 0x02)  mods_name += "\u21E7", mods_desc.push("Shift");
        if (mods & 0x01)  mods_name += "\u2303", mods_desc.push("Control");
        mods_desc = mods_desc.join(" + ");
        if (mods & 0x10 && mods & 0x0f) {
            mods_name = "R" + mods_name, mods_desc = "Right " + mods_desc;
        } else {
            mods_name = "L" + mods_name, mods_desc = "Left " + mods_desc;
        }
        return { name: mods_name, desc: mods_desc };
    };
    this.get_action = function() {
        switch ((this.code & 0xf000)>>12) {
            case ACT_LMODS:
            case ACT_RMODS:
                if (this.key_mods == 0) {
                    if (keycodes[this.key_code])
                        return $.extend({}, action_kinds.KEY,
                                {
                                    name: keycodes[this.key_code].name,
                                    desc: keycodes[this.key_code].desc
                                });
                } else {
                    var _mods_str = mods_str(this.key_mods);
                    if (keycodes[this.key_code])
                        return $.extend({}, action_kinds.MODS_KEY,
                                {
                                    name: _mods_str.name + " " + keycodes[this.key_code].name,
                                    desc: _mods_str.desc + " + " + keycodes[this.key_code].desc
                                });
                }
                break;
            case ACT_LMODS_TAP:
            case ACT_RMODS_TAP:
                var _mods_str = mods_str(this.key_mods);
                switch (this.key_code) {
                    case MODS_ONESHOT:
                        return $.extend({}, action_kinds.MODS_ONESHOT,
                                {
                                    name: _mods_str.name + " OneShot",
                                    desc: _mods_str.desc + " OneShot"
                                });
                    case MODS_TAP_TOGGLE:
                        return $.extend({}, action_kinds.MODS_TAP_TOGGLE,
                                {
                                    name: _mods_str.name + " TapToggle",
                                    desc: _mods_str.desc + " TapToggle"
                                });
                }
                return $.extend({}, action_kinds.MODS_TAP_KEY,
                        {
                            name: _mods_str.name + " " + keycodes[this.key_code].name,
                            desc: _mods_str.desc + " " + keycodes[this.key_code].desc
                        });
                break;
            case ACT_LAYER:
                switch (this.layer_bitop_op) {
                    case OP_BIT_AND:
                        if (this.layer_bitop_bits == 0 && this.layer_bitop_xbit == 0) {
                            return $.extend({}, action_kinds.LAYER_CLEAR,
                                    {
                                        name: "Lclr",
                                        desc: "Disable all layers " + this.layer_bitop_on_str
                                    });
                        }
                        if (this.layer_bitop_bits_is_single_0 &&
                            this.layer_bitop_layer < CONFIG.layer_num &&
                            this.layer_bitop_xbit == 1) {
                            return $.extend({}, action_kinds.LAYER_OFF,
                                    {
                                        name: "L-" + this.layer_bitop_layer,
                                        desc: "Disable Layer " + this.layer_bitop_layer + " " + this.layer_bitop_on_str
                                    });
                        }
                        return $.extend({}, action_kinds.LAYER_BIT_AND,
                                {
                                    name: "Lba",
                                    desc: "layers & " +  this.layer_bitop_masked_bits_str +
                                            "\n" + this.layer_bitop_on_str
                                });
                    case OP_BIT_OR:
                        if (this.layer_bitop_bits_is_single_1 &&
                            this.layer_bitop_layer < CONFIG.layer_num &&
                            this.layer_bitop_xbit == 0) {
                            return $.extend({}, action_kinds.LAYER_ON,
                                    {
                                        name: "L+" + this.layer_bitop_layer,
                                        desc: "Enable Layer " + this.layer_bitop_layer + " " + this.layer_bitop_on_str
                                    });
                        }
                        return $.extend({}, action_kinds.LAYER_BIT_OR,
                                {
                                    name: "Lbo",
                                    desc: "layers | " +  this.layer_bitop_masked_bits_str +
                                            "\n" + this.layer_bitop_on_str
                                });
                    case OP_BIT_XOR:
                        if (this.layer_bitop_bits_is_single_1 &&
                            this.layer_bitop_layer < CONFIG.layer_num &&
                            this.layer_bitop_xbit == 0) {
                            if (this.layer_bitop_on == ON_RELEASE) {
                                return $.extend({}, action_kinds.LAYER_TOGGLE,
                                        {
                                            name: "T" + this.layer_bitop_layer,
                                            desc: "Toggle Layer " + this.layer_bitop_layer
                                        });
                            } else {
                                return $.extend({}, action_kinds.LAYER_INVERT,
                                        {
                                            name: "Li" + this.layer_bitop_layer,
                                            desc: "Invert Layer " + this.layer_bitop_layer + " " + this.layer_bitop_on_str
                                        });
                            }
                        }
                        return $.extend({}, action_kinds.LAYER_BIT_XOR,
                                {
                                    name: "Lbx",
                                    desc: "layers ^ " +  this.layer_bitop_masked_bits_str +
                                            "\n" + this.layer_bitop_on_str
                                });
                    case OP_BIT_SET:
                        if (this.layer_bitop_bits_is_single_1 &&
                            this.layer_bitop_layer < CONFIG.layer_num &&
                            this.layer_bitop_xbit == 0) {
                            if (this.layer_bitop_on == ON_DEFAULT_LAYER) {
                                return $.extend({}, action_kinds.DEFAULT_LAYER_SET,
                                        {
                                            name: "DLs" + this.layer_bitop_layer,
                                            desc: "Turn on default Layer " + this.layer_bitop_layer + " solely"
                                        });
                            } else {
                                return $.extend({}, action_kinds.LAYER_SET,
                                        {
                                            name: "Ls" + this.layer_bitop_layer,
                                            desc: "Turn on Layer " + this.layer_bitop_layer + " solely"
                                        });
                            }
                        }
                        return $.extend({}, action_kinds.LAYER_BIT_SET,
                                {
                                    name: "Lbs",
                                    desc: (this.layer_bitop_xbit ?
                                            "(layers & " + this.layer_bitop_mask_str + ") | " :
                                            "layers = ") + this.layer_bitop_bits_str + "\n" + this.layer_bitop_on_str
                                });
                }
                break;
            case ACT_LAYER_EXT:
                /* not used */
                break;
            case ACT_LAYER_TAP:
            case ACT_LAYER_TAP_EXT:
                switch (this.layer_tap_code) {
                    case OP_TAP_TOGGLE:
                        return $.extend({}, action_kinds.LAYER_TAP_TOGGLE,
                                {
                                    name: "Lt" + this.layer_tap_val,
                                    desc: "Toggle Layer " + this.layer_tap_val + " / lock it with "+ CONFIG.tapping_toggle + " taps"
                                });
                    case OP_ON_OFF:
                        return $.extend({}, action_kinds.LAYER_MOMENTARY,
                                {
                                    name: "L" + this.layer_tap_val,
                                    desc: "Enable Layer " + this.layer_tap_val + " on press / disable it on release"
                                });
                    case OP_OFF_ON:
                        return $.extend({}, action_kinds.LAYER_OFF_ON,
                                {
                                    name: "L-+" + this.layer_tap_val,
                                    desc: "Disable Layer " + this.layer_tap_val + " on press / enable it on release"
                                });
                    case OP_SET_CLEAR:
                        return $.extend({}, action_kinds.LAYER_SET_CLEAR,
                                {
                                    name: "Lsc" + this.layer_tap_val,
                                    desc: "Turn on Layer " + this.layer_tap_val + " solely on press and clear all layers on release"
                                });
                    default:
                        /* 0xc0 ... 0xdf */
                        if ((this.layer_tap_code & 0xe0) == 0xc0) {
                            var _mods_str =  mods_str(this.layer_tap_code & 0x1f);
                            return $.extend({}, action_kinds.LAYER_MODS,
                                    {
                                        name: "LM" + this.layer_tap_val + " " + _mods_str.name,
                                        desc: "Turn on Layer " + this.layer_tap_val + " with " + _mods_str.desc
                                    });
                        }
                        else {
                            return $.extend({}, action_kinds.LAYER_TAP_KEY,
                                    {
                                        name: "LT" + this.layer_tap_val + " " + keycodes[this.layer_tap_code].name,
                                        desc: "Turn on Layer " + this.layer_tap_val + " and " + keycodes[this.layer_tap_code].name + "(tap)"
                                    });
                        }
                }
                break;
            case ACT_USAGE:
                switch (this.usage_page) {
                    case PAGE_SYSTEM:
                        if (system_codes[this.usage_code])
                            return $.extend({}, action_kinds.USAGE_SYSTEM,
                                    {
                                        name: system_codes[this.usage_code].name,
                                        desc: system_codes[this.usage_code].desc
                                    });
                        else
                            return $.extend({}, action_kinds.USAGE_SYSTEM,
                                    {
                                        name: "System " + this.usage_code.toString(16).toUpperCase(),
                                        desc: "System Control " + this.usage_code.toString(16).toUpperCase(),
                                    });
                    case PAGE_CONSUMER:
                        if (consumer_codes[this.usage_code])
                            return $.extend({}, action_kinds.USAGE_CONSUMER,
                                    {
                                        name: consumer_codes[this.usage_code].name,
                                        desc: consumer_codes[this.usage_code].desc
                                    });
                        else
                            return $.extend({}, action_kinds.USAGE_CONSUMER,
                                    {
                                        name: "Consumer " + this.usage_code.toString(16).toUpperCase(),
                                        desc: "Consumer Page " + this.usage_code.toString(16).toUpperCase(),
                                    });
                }
                break;
            case ACT_MOUSEKEY:
                if (mousekey_codes[this.mousekey_code])
                    return $.extend({}, action_kinds.MOUSEKEY,
                            {
                                name: mousekey_codes[this.mousekey_code].name,
                                desc: mousekey_codes[this.mousekey_code].desc
                            });
                break;
            case ACT_COMMAND:
                if (command_ids[this.command_id])
                    return $.extend({}, action_kinds.COMMAND,
                            {
                                name: command_ids[this.command_id].name,
                                desc: command_ids[this.command_id].desc
                            });
                break;
        };
        return { id: "UNKNOWN", name: "???", desc: "Unknown action" };
    };

    Object.defineProperty(this, "id", {
        get: function() {
            var _action = this.get_action();
            return _action.id;
        }
    });
    Object.defineProperty(this, "name", {
        get: function() {
            var _action = this.get_action();
            return _action.name;
        }
    });
    Object.defineProperty(this, "desc", {
        get: function() {
            var _action = this.get_action();
            return _action.desc;
        }
    });
}


/**********************************************************************
 * Action kind description
 **********************************************************************/
var action_kinds = {
    KEY:                { id: "KEY",                    name: "ACTION_KEY",                 desc: "Normal key" },
    MODS_KEY:           { id: "MODS_KEY",               name: "ACTION_MODS_KEY",            desc: "Modified key" },
    MODS_TAP_KEY:       { id: "MODS_TAP_KEY",           name: "ACTION_MODS_TAP_KEY",        desc: "Modifiers on hold and a type a key on tap" },
    MODS_ONESHOT:       { id: "MODS_ONESHOT",           name: "ACTION_MODS_ONESHOT",        desc: "Oneshot modifiers" },
    MODS_TAP_TOGGLE:    { id: "MODS_TAP_TOGGLE",        name: "ACTION_MODS_TAP_TOGGLE",     desc: "Modifiers and tap toggle" },
    USAGE_SYSTEM:       { id: "USAGE_SYSTEM",           name: "ACTION_USAGE_SYSTEM",        desc: "System control key" },
    USAGE_CONSUMER:     { id: "USAGE_CONSUMER",         name: "ACTION_USAGE_CONSUMER",      desc: "Consumer key" },
    MOUSEKEY:           { id: "MOUSEKEY",               name: "ACTION_MOUSEKEY",            desc: "Mouse key" },
    /*
    MACRO:              { id: "MACRO",                  name: "ACTION_MACRO",               desc: "Macro" },
    MACRO_TAP:          { id: "MACRO_TAP",              name: "ACTION_MACRO_TAP",           desc: "Macro for tap key" },
    MACRO_OPT:          { id: "MACRO_OPT",              name: "ACTION_MACRO_OPT",           desc: "Macro with option" },
    FUNCTION:           { id: "FUNCTION",               name: "ACTION_FUNCTION",            desc: "Function" },
    FUNCTION_TAP:       { id: "FUNCTION_TAP",           name: "ACTION_FUNCTION_TAP",        desc: "Function for tap key" },
    FUNCTION_OPT:       { id: "FUNCTION_OPT",           name: "ACTION_FUNCTION_OPT",        desc: "Function with option" },
    */
    LAYER_CLEAR:        { id: "LAYER_CLEAR",            name: "ACTION_LAYER_CLEAR",         desc: "Trun off all layers" },
    LAYER_MOMENTARY:    { id: "LAYER_MOMENTARY",        name: "ACTION_LAYER_MOMENTARY",     desc: "Turn on a layer on press and off the layer on release" },
    LAYER_TOGGLE:       { id: "LAYER_TOGGLE",           name: "ACTION_LAYER_TOGGLE",        desc: "Toggle a layer between on and off" },
    LAYER_INVERT:       { id: "LAYER_INVERT",           name: "ACTION_LAYER_INVERT",        desc: "Invert state of a layer" },
    LAYER_ON:           { id: "LAYER_ON",               name: "ACTION_LAYER_ON",            desc: "Turn on a layer" },
    LAYER_OFF:          { id: "LAYER_OFF",              name: "ACTION_LAYER_OFF",           desc: "Turn off a layer" },
    LAYER_SET:          { id: "LAYER_SET",              name: "ACTION_LAYER_SET",           desc: "Turn on a layer solely" },
    LAYER_ON_OFF:       { id: "LAYER_ON_OFF",           name: "ACTION_LAYER_ON_OFF",        desc: "Turn on a layer, then off" },
    LAYER_OFF_ON:       { id: "LAYER_OFF_ON",           name: "ACTION_LAYER_OFF_ON",        desc: "Disable a layer on press and enable the layer on release" },
    LAYER_SET_CLEAR:    { id: "LAYER_SET_CLEAR",        name: "ACTION_LAYER_SET_CLEAR",     desc: "Turn on a layer solely on press and clear all layers on release" },
    LAYER_MODS:         { id: "LAYER_MODS",             name: "ACTION_LAYER_MODS",          desc: "Turn on a layer with modifiers" },
    LAYER_TAP_KEY:      { id: "LAYER_TAP_KEY",          name: "ACTION_LAYER_TAP_KEY",       desc: "Turn on a layer on hold and type a key on tap" },
    LAYER_TAP_TOGGLE:   { id: "LAYER_TAP_TOGGLE",       name: "ACTION_LAYER_TAP_TOGGLE",    desc: "Turn on a layer on hold and tap toggle" },
    LAYER_BIT_AND:      { id: "LAYER_BIT_AND",          name: "ACTION_LAYER_BIT_AND",       desc: "Layer Bit AND" },
    LAYER_BIT_OR:       { id: "LAYER_BIT_OR",           name: "ACTION_LAYER_BIT_OR",        desc: "Layer Bit OR" },
    LAYER_BIT_XOR:      { id: "LAYER_BIT_XOR",          name: "ACTION_LAYER_BIT_XOR",       desc: "Layer Bit XOR" },
    LAYER_BIT_SET:      { id: "LAYER_BIT_SET",          name: "ACTION_LAYER_BIT_SET",       desc: "Layer Bit SET" },
    DEFAULT_LAYER_SET:  { id: "DEFAULT_LAYER_SET",      name: "ACTION_DEFAULT_LAYER_SET",   desc: "Turn on a default layer solely" },
    COMMAND:            { id: "COMMAND",                name: "ACTION_COMMAND",             desc: "Built-in Command" },
};


/**********************************************************************
 * Keyboard/Keypad Page(0x07) - keycodes
 **********************************************************************/
var keycodes = [];
keycodes[0x0000] = {id: 'NO  ',                        name: 'NO',                          desc: 'No action'};
keycodes[0x0001] = {id: 'TRNS',                        name: 'TRNS',                        desc: 'Transparent'};
/*
keycodes[0x0002] = {id: 'POST_FAIL',                   name: 'POST_FAIL',                   desc: 'POST_FAIL'};
keycodes[0x0003] = {id: 'UNDEFINED',                   name: 'UNDEFINED',                   desc: 'UNDEFINED'};
*/
keycodes[0x0004] = {id: 'A',                           name: 'A',                           desc: 'A'};
keycodes[0x0005] = {id: 'B',                           name: 'B',                           desc: 'B'};
keycodes[0x0006] = {id: 'C',                           name: 'C',                           desc: 'C'};
keycodes[0x0007] = {id: 'D',                           name: 'D',                           desc: 'D'};
keycodes[0x0008] = {id: 'E',                           name: 'E',                           desc: 'E'};
keycodes[0x0009] = {id: 'F',                           name: 'F',                           desc: 'F'};
keycodes[0x000A] = {id: 'G',                           name: 'G',                           desc: 'G'};
keycodes[0x000B] = {id: 'H',                           name: 'H',                           desc: 'H'};
keycodes[0x000C] = {id: 'I',                           name: 'I',                           desc: 'I'};
keycodes[0x000D] = {id: 'J',                           name: 'J',                           desc: 'J'};
keycodes[0x000E] = {id: 'K',                           name: 'K',                           desc: 'K'};
keycodes[0x000F] = {id: 'L',                           name: 'L',                           desc: 'L'};
keycodes[0x0010] = {id: 'M',                           name: 'M',                           desc: 'M'};
keycodes[0x0011] = {id: 'N',                           name: 'N',                           desc: 'N'};
keycodes[0x0012] = {id: 'O',                           name: 'O',                           desc: 'O'};
keycodes[0x0013] = {id: 'P',                           name: 'P',                           desc: 'P'};
keycodes[0x0014] = {id: 'Q',                           name: 'Q',                           desc: 'Q'};
keycodes[0x0015] = {id: 'R',                           name: 'R',                           desc: 'R'};
keycodes[0x0016] = {id: 'S',                           name: 'S',                           desc: 'S'};
keycodes[0x0017] = {id: 'T',                           name: 'T',                           desc: 'T'};
keycodes[0x0018] = {id: 'U',                           name: 'U',                           desc: 'U'};
keycodes[0x0019] = {id: 'V',                           name: 'V',                           desc: 'V'};
keycodes[0x001A] = {id: 'W',                           name: 'W',                           desc: 'W'};
keycodes[0x001B] = {id: 'X',                           name: 'X',                           desc: 'X'};
keycodes[0x001C] = {id: 'Y',                           name: 'Y',                           desc: 'Y'};
keycodes[0x001D] = {id: 'Z',                           name: 'Z',                           desc: 'Z'};
keycodes[0x001E] = {id: '1',                           name: '1',                           desc: '1'};
keycodes[0x001F] = {id: '2',                           name: '2',                           desc: '2'};
keycodes[0x0020] = {id: '3',                           name: '3',                           desc: '3'};
keycodes[0x0021] = {id: '4',                           name: '4',                           desc: '4'};
keycodes[0x0022] = {id: '5',                           name: '5',                           desc: '5'};
keycodes[0x0023] = {id: '6',                           name: '6',                           desc: '6'};
keycodes[0x0024] = {id: '7',                           name: '7',                           desc: '7'};
keycodes[0x0025] = {id: '8',                           name: '8',                           desc: '8'};
keycodes[0x0026] = {id: '9',                           name: '9',                           desc: '9'};
keycodes[0x0027] = {id: '0',                           name: '0',                           desc: '0'};
keycodes[0x0028] = {id: 'ENT',                         name: 'Enter',                       desc: 'ENTER'};
keycodes[0x0029] = {id: 'ESC',                         name: 'Esc',                         desc: 'Escape'};
keycodes[0x002A] = {id: 'BSPC',                        name: 'Back space',                  desc: 'Backspace'};
keycodes[0x002B] = {id: 'TAB',                         name: 'Tab',                         desc: 'Tab'};
keycodes[0x002C] = {id: 'SPC',                         name: 'Space',                       desc: 'Space'};
keycodes[0x002D] = {id: 'MINS',                        name: '-',                           desc: 'MINUS'};
keycodes[0x002E] = {id: 'EQL',                         name: '=',                           desc: 'EQUAL'};
keycodes[0x002F] = {id: 'LBRC',                        name: '[',                           desc: 'Left Bracket'};
keycodes[0x0030] = {id: 'RBRC',                        name: ']',                           desc: 'Right Bracket'};
keycodes[0x0031] = {id: 'BSLS',                        name: "\\",                          desc: 'Backslash'};
keycodes[0x0032] = {id: 'NUHS',                        name: 'ISO #',                       desc: 'Non-US Hash'};
keycodes[0x0033] = {id: 'SCLN',                        name: ';',                           desc: 'Semicolon'};
keycodes[0x0034] = {id: 'QUOT',                        name: "'",                           desc: 'Quote'};
keycodes[0x0035] = {id: 'GRV ',                        name: '`',                           desc: 'Grave'};
keycodes[0x0036] = {id: 'COMM',                        name: ',',                           desc: 'Comma'};
keycodes[0x0037] = {id: 'DOT ',                        name: '.',                           desc: 'Dot'};
keycodes[0x0038] = {id: 'SLSH',                        name: '/',                           desc: 'Slash'};
keycodes[0x0039] = {id: 'CAPS',                        name: 'Caps Lock',                   desc: 'Need this? Sure? :)'};
keycodes[0x003A] = {id: 'F1  ',                        name: 'F1',                          desc: 'F1'};
keycodes[0x003B] = {id: 'F2  ',                        name: 'F2',                          desc: 'F2'};
keycodes[0x003C] = {id: 'F3  ',                        name: 'F3',                          desc: 'F3'};
keycodes[0x003D] = {id: 'F4  ',                        name: 'F4',                          desc: 'F4'};
keycodes[0x003E] = {id: 'F5  ',                        name: 'F5',                          desc: 'F5'};
keycodes[0x003F] = {id: 'F6  ',                        name: 'F6',                          desc: 'F6'};
keycodes[0x0040] = {id: 'F7  ',                        name: 'F7',                          desc: 'F7'};
keycodes[0x0041] = {id: 'F8  ',                        name: 'F8',                          desc: 'F8'};
keycodes[0x0042] = {id: 'F9  ',                        name: 'F9',                          desc: 'F9'};
keycodes[0x0043] = {id: 'F10 ',                        name: 'F10',                         desc: 'F10'};
keycodes[0x0044] = {id: 'F11 ',                        name: 'F11',                         desc: 'F11'};
keycodes[0x0045] = {id: 'F12 ',                        name: 'F12',                         desc: 'F12'};
keycodes[0x0046] = {id: 'PSCR',                        name: 'Print Screen',                desc: 'Print Screen'};
keycodes[0x0047] = {id: 'SLCK',                        name: 'Scroll Lock',                 desc: 'Scroll Lock'};
keycodes[0x0048] = {id: 'PAUS',                        name: 'Pause',                       desc: 'Pause'};
keycodes[0x0049] = {id: 'INS ',                        name: 'Insert',                      desc: 'Insert'};
keycodes[0x004A] = {id: 'HOME',                        name: 'Home',                        desc: 'Home'};
keycodes[0x004B] = {id: 'PGUP',                        name: 'Page Up',                     desc: 'Page Up'};
keycodes[0x004C] = {id: 'DEL ',                        name: 'Delete',                      desc: 'Delete'};
keycodes[0x004D] = {id: 'END ',                        name: 'End',                         desc: 'End'};
keycodes[0x004E] = {id: 'PGDN',                        name: 'Page Down',                   desc: 'Page Down'};
keycodes[0x004F] = {id: 'RGHT',                        name: '\u2192',                      desc: 'Right'};
keycodes[0x0050] = {id: 'LEFT',                        name: '\u2190',                      desc: 'Left'};
keycodes[0x0051] = {id: 'DOWN',                        name: '\u2193',                      desc: 'Down'};
keycodes[0x0052] = {id: 'UP  ',                        name: '\u2191',                      desc: 'Up'};
keycodes[0x0053] = {id: 'NLCK',                        name: 'Num Lock',                    desc: 'Num Lock'};
keycodes[0x0054] = {id: 'PSLS',                        name: 'P/',                          desc: 'Keypad Slash'};
keycodes[0x0055] = {id: 'PAST',                        name: 'P*',                          desc: 'Keypad Asterisk'};
keycodes[0x0056] = {id: 'PMNS',                        name: 'P-',                          desc: 'Keypad Minus'};
keycodes[0x0057] = {id: 'PPLS',                        name: 'P+',                          desc: 'Keypad Plus'};
keycodes[0x0058] = {id: 'PENT',                        name: 'P\u21A9',                     desc: 'Keypad Enter'};
keycodes[0x0059] = {id: 'P1  ',                        name: 'P1',                          desc: 'Keypad 1'};
keycodes[0x005A] = {id: 'P2  ',                        name: 'P2',                          desc: 'Keypad 2'};
keycodes[0x005B] = {id: 'P3  ',                        name: 'P3',                          desc: 'Keypad 3'};
keycodes[0x005C] = {id: 'P4  ',                        name: 'P4',                          desc: 'Keypad 4'};
keycodes[0x005D] = {id: 'P5  ',                        name: 'P5',                          desc: 'Keypad 5'};
keycodes[0x005E] = {id: 'P6  ',                        name: 'P6',                          desc: 'Keypad 6'};
keycodes[0x005F] = {id: 'P7  ',                        name: 'P7',                          desc: 'Keypad 7'};
keycodes[0x0060] = {id: 'P8  ',                        name: 'P8',                          desc: 'Keypad 8'};
keycodes[0x0061] = {id: 'P9  ',                        name: 'P9',                          desc: 'Keypad 9'};
keycodes[0x0062] = {id: 'P0  ',                        name: 'P0',                          desc: 'Keypad 0'};
keycodes[0x0063] = {id: 'PDOT',                        name: 'P.',                          desc: 'Keypad Dot'};
keycodes[0x0064] = {id: 'NUBS',                        name: 'ISO \\',                      desc: 'Non-US Backslash'};
keycodes[0x0065] = {id: 'APP ',                        name: 'Application',                 desc: 'Menu on Windows'};
keycodes[0x0066] = {id: 'POWER',                       name: '_Power',                      desc: 'Power(Not work on Windows)'};
keycodes[0x0067] = {id: 'PEQL',                        name: 'P=',                          desc: 'Keymapd Equal'};
keycodes[0x0068] = {id: 'F13 ',                        name: 'F13',                         desc: 'F13'};
keycodes[0x0069] = {id: 'F14 ',                        name: 'F14',                         desc: 'F14'};
keycodes[0x006A] = {id: 'F15 ',                        name: 'F15',                         desc: 'F15'};
keycodes[0x006B] = {id: 'F16 ',                        name: 'F16',                         desc: 'F16'};
keycodes[0x006C] = {id: 'F17 ',                        name: 'F17',                         desc: 'F17'};
keycodes[0x006D] = {id: 'F18 ',                        name: 'F18',                         desc: 'F18'};
keycodes[0x006E] = {id: 'F19 ',                        name: 'F19',                         desc: 'F19'};
keycodes[0x006F] = {id: 'F20 ',                        name: 'F20',                         desc: 'F20'};
keycodes[0x0070] = {id: 'F21 ',                        name: 'F21',                         desc: 'F21'};
keycodes[0x0071] = {id: 'F22 ',                        name: 'F22',                         desc: 'F22'};
keycodes[0x0072] = {id: 'F23 ',                        name: 'F23',                         desc: 'F23'};
keycodes[0x0073] = {id: 'F24 ',                        name: 'F24',                         desc: 'F24'};
keycodes[0x0074] = {id: 'EXECUTE',                     name: 'Execute',                     desc: 'Execute(Unix)'};
keycodes[0x0075] = {id: 'HELP',                        name: 'Help',                        desc: 'Help(Unix)'};
keycodes[0x0076] = {id: 'MENU',                        name: 'Menu',                        desc: 'Menu(Unix)'};
keycodes[0x0077] = {id: 'SELECT',                      name: 'Select',                      desc: 'Select(Unix)'};
keycodes[0x0078] = {id: 'STOP',                        name: 'Stop',                        desc: 'Stop(Unix)'};
keycodes[0x0079] = {id: 'AGAIN',                       name: 'Again',                       desc: 'Again(Unix)'};
keycodes[0x007A] = {id: 'UNDO',                        name: 'Undo',                        desc: 'Undo(Unix)'};
keycodes[0x007B] = {id: 'CUT',                         name: 'Cut',                         desc: 'Cut(Unix)'};
keycodes[0x007C] = {id: 'COPY',                        name: 'Copy',                        desc: 'Copy(Unix)'};
keycodes[0x007D] = {id: 'PASTE',                       name: 'Paste',                       desc: 'Paste(Unix)'};
keycodes[0x007E] = {id: 'FIND',                        name: 'Find',                        desc: 'Find(Unix)'};
keycodes[0x007F] = {id: '_MUTE',                       name: '_MUTE',                       desc: '_MUTE(Not work on Windows)'};
keycodes[0x0080] = {id: '_VOLUP',                      name: '_VOLUP',                      desc: '_VOLUP(Not work on Windows)'};
keycodes[0x0081] = {id: '_VOLDOWN',                    name: '_VOLDOWN',                    desc: '_VOLDOWN(Not work on Windows)'};
keycodes[0x0082] = {id: 'LCAP',                        name: 'Locking Caps Lock',           desc: 'Locking Caps Lock'};
keycodes[0x0083] = {id: 'LNUM',                        name: 'Locking Num Lock',            desc: 'Locking Num Lock'};
keycodes[0x0084] = {id: 'LSCR',                        name: 'Locking Scroll Lock',         desc: 'Locking Scroll Lock'};
keycodes[0x0085] = {id: 'PCMM',                        name: 'P,',                          desc: 'Keypad Comma'};
keycodes[0x0086] = {id: 'KP_EQUAL_AS400',              name: 'P= (AS400)',                  desc: 'Keypad Equal (AS400)'};
keycodes[0x0087] = {id: 'INT1',                        name: 'JIS \u308D',                  desc: 'Japanese RO'};
keycodes[0x0088] = {id: 'INT2',                        name: '\u3072\u3089\u304c\u306a',    desc: 'Japanese Hiragana'};
keycodes[0x0089] = {id: 'INT3',                        name: 'JIS \uffe5',                  desc: 'Japanese Yen'};
keycodes[0x008A] = {id: 'INT4',                        name: '\u5909\u63db',                desc: 'Japanese Henkan'};
keycodes[0x008B] = {id: 'INT5',                        name: '\u7121\u5909\u63db',          desc: 'Japanese Muhenkan'};
keycodes[0x008C] = {id: 'INT6',                        name: 'INT6',                        desc: 'INT6'};
keycodes[0x008D] = {id: 'INT7',                        name: 'INT7',                        desc: 'INT7'};
keycodes[0x008E] = {id: 'INT8',                        name: 'INT8',                        desc: 'INT8'};
keycodes[0x008F] = {id: 'INT9',                        name: 'INT9',                        desc: 'INT9'};
keycodes[0x0090] = {id: 'LANG1',                       name: '\ud55c/\uc601',               desc: 'Korean Hangul/English'};
keycodes[0x0091] = {id: 'LANG2',                       name: '\ud55c\uc790',                desc: 'Korean Hanja'};
keycodes[0x0092] = {id: 'LANG3',                       name: 'LANG3',                       desc: 'LANG3'};
keycodes[0x0093] = {id: 'LANG4',                       name: 'LANG4',                       desc: 'LANG4'};
keycodes[0x0094] = {id: 'LANG5',                       name: 'LANG5',                       desc: 'LANG5'};
keycodes[0x0095] = {id: 'LANG6',                       name: 'LANG6',                       desc: 'LANG6'};
keycodes[0x0096] = {id: 'LANG7',                       name: 'LANG7',                       desc: 'LANG7'};
keycodes[0x0097] = {id: 'LANG8',                       name: 'LANG8',                       desc: 'LANG8'};
keycodes[0x0098] = {id: 'LANG9',                       name: 'LANG9',                       desc: 'LANG9'};
keycodes[0x0099] = {id: 'ALT_ERASE',                   name: 'ALT_ERASE',                   desc: 'ALT_ERASE'};
keycodes[0x009A] = {id: 'SYSREQ',                      name: 'SYSREQ',                      desc: 'SYSREQ'};
keycodes[0x009B] = {id: 'CANCEL',                      name: 'CANCEL',                      desc: 'CANCEL'};
keycodes[0x009C] = {id: 'CLEAR',                       name: 'CLEAR',                       desc: 'CLEAR'};
keycodes[0x009D] = {id: 'PRIOR',                       name: 'PRIOR',                       desc: 'PRIOR'};
keycodes[0x009E] = {id: 'RETURN',                      name: 'RETURN',                      desc: 'RETURN'};
keycodes[0x009F] = {id: 'SEPARATOR',                   name: 'SEPARATOR',                   desc: 'SEPARATOR'};
keycodes[0x00A0] = {id: 'OUT',                         name: 'OUT',                         desc: 'OUT'};
keycodes[0x00A1] = {id: 'OPER',                        name: 'OPER',                        desc: 'OPER'};
keycodes[0x00A2] = {id: 'CLEAR_AGAIN',                 name: 'CLEAR_AGAIN',                 desc: 'CLEAR_AGAIN'};
keycodes[0x00A3] = {id: 'CRSEL',                       name: 'CRSEL',                       desc: 'CRSEL'};
keycodes[0x00A4] = {id: 'EXSEL',                       name: 'EXSEL',                       desc: 'EXSEL'};

keycodes[0x00A5] = {id: 'PWR ',                        name: 'Sys Power',                   desc: 'System Power'};
keycodes[0x00A6] = {id: 'SLEP',                        name: 'Sys Sleep',                   desc: 'System Sleep'};
keycodes[0x00A7] = {id: 'WAKE',                        name: 'Sys Wake',                    desc: 'System Wake'};
keycodes[0x00A8] = {id: 'MUTE',                        name: 'Mute',                        desc: 'Audio Mute'};
keycodes[0x00A9] = {id: 'VOLU',                        name: 'Vol Up',                      desc: 'Audio Vol Up'};
keycodes[0x00AA] = {id: 'VOLD',                        name: 'Vol Down',                    desc: 'Audio Vol Down'};
keycodes[0x00AB] = {id: 'MNXT',                        name: 'Next Track',                  desc: 'Next Track'};
keycodes[0x00AC] = {id: 'MPRV',                        name: 'Prev Track',                  desc: 'Previous Track'};
keycodes[0x00AD] = {id: 'MFFD',                        name: 'Fast Forward',                desc: 'Media Fast Forward(Mac)'};
keycodes[0x00AE] = {id: 'MRWD',                        name: 'Rewind',                      desc: 'Media Rewind(Mac)'};
keycodes[0x00AF] = {id: 'MSTP',                        name: 'Stop',                        desc: 'Media Stop'};
keycodes[0x00B0] = {id: 'MPLY',                        name: 'Play Pause',                  desc: 'Play Pause'};
keycodes[0x00B1] = {id: 'EJCT',                        name: 'Eject',                       desc: 'Media Eject'};
keycodes[0x00B2] = {id: 'MSEL',                        name: 'Media Select',                desc: 'Media Select'};
keycodes[0x00B3] = {id: 'MAIL',                        name: 'Mail',                        desc: 'Mail'};
keycodes[0x00B4] = {id: 'CALC',                        name: 'Calc',                        desc: 'Calculator'};
keycodes[0x00B5] = {id: 'MYCM',                        name: 'My Computer',                 desc: 'My Computer'};
keycodes[0x00B6] = {id: 'WSCH',                        name: 'Web Search',                  desc: 'WWW Search'};
keycodes[0x00B7] = {id: 'WHOM',                        name: 'Web Home',                    desc: 'WWW Home'};
keycodes[0x00B8] = {id: 'WBAK',                        name: 'Web Back',                    desc: 'WWW Back'};
keycodes[0x00B9] = {id: 'WFWD',                        name: 'Web Forward',                 desc: 'WWW Forward'};
keycodes[0x00BA] = {id: 'WSTP',                        name: 'Web Stop',                    desc: 'WWW Stop'};
keycodes[0x00BB] = {id: 'WREF',                        name: 'Web Refresh',                 desc: 'WWW Refresh'};
keycodes[0x00BC] = {id: 'WFAV',                        name: 'Web Favorites',               desc: 'WWW Favorites'};
keycodes[0x00BD] = {id: 'BRTI',                        name: 'Brightness Inc',              desc: 'Brightness Increment'};
keycodes[0x00BE] = {id: 'BRTD',                        name: 'Brightness Dec',              desc: 'Brightness Decrement'};
keycodes[0x00BF] = {id: 'BTLD',                        name: 'Bootloader',                  desc: 'Start Bootloader'    };

keycodes[0x00E0] = {id: 'LCTL',                        name: 'LCtrl',                       desc: 'Left Control'};
keycodes[0x00E1] = {id: 'LSFT',                        name: 'LShift',                      desc: 'Left Shift'};
keycodes[0x00E2] = {id: 'LALT',                        name: 'LAlt',                        desc: 'Left Alt(\u2325)'};
keycodes[0x00E3] = {id: 'LGUI',                        name: 'LGui',                        desc: 'Left Windows(\u2318)'};
keycodes[0x00E4] = {id: 'RCTL',                        name: 'RCtrl',                       desc: 'Right Control'};
keycodes[0x00E5] = {id: 'RSFT',                        name: 'RShift',                      desc: 'Right Shift'};
keycodes[0x00E6] = {id: 'RALT',                        name: 'RAlt',                        desc: 'Right Alt(\u2325)'};
keycodes[0x00E7] = {id: 'RGUI',                        name: 'RGui',                        desc: 'Right Windows(\u2318)'};

keycodes[0x00F0] = {id: 'MS_U',                        name: 'Mouse Up',                    desc: 'Mouse UP'};
keycodes[0x00F1] = {id: 'MS_D',                        name: 'Mouse down',                  desc: 'Mouse Down'};
keycodes[0x00F2] = {id: 'MS_L',                        name: 'Mouse left',                  desc: 'Mouse Left'};
keycodes[0x00F3] = {id: 'MS_R',                        name: 'Mouse right',                 desc: 'Mouse Right'};
keycodes[0x00F4] = {id: 'BTN1',                        name: 'Mouse Btn1',                  desc: 'Mouse Button1'};
keycodes[0x00F5] = {id: 'BTN2',                        name: 'Mouse Btn2',                  desc: 'Mouse Button2'};
keycodes[0x00F6] = {id: 'BTN3',                        name: 'Mouse Btn3',                  desc: 'Mouse Button3'};
keycodes[0x00F7] = {id: 'BTN4',                        name: 'Mouse Btn4',                  desc: 'Mouse Button4'};
keycodes[0x00F8] = {id: 'BTN5',                        name: 'Mouse Btn5',                  desc: 'Mouse Button5'};
keycodes[0x00F9] = {id: 'WH_U',                        name: 'Wheel Up',                    desc: 'Wheel Up'};
keycodes[0x00FA] = {id: 'WH_D',                        name: 'Wheel Down',                  desc: 'Wheel Down'};
keycodes[0x00FB] = {id: 'WH_L',                        name: 'Wheel Left',                  desc: 'Wheel Left'};
keycodes[0x00FC] = {id: 'WH_R',                        name: 'Wheel Right',                 desc: 'Wheel Right'};
keycodes[0x00FD] = {id: 'ACL0',                        name: 'Mouse Slow',                  desc: 'Mouse Slow'};
keycodes[0x00FE] = {id: 'ACL1',                        name: 'Mouse Medium',                desc: 'Mouse Medium'};
keycodes[0x00FF] = {id: 'ACL2',                        name: 'Mouse Fast',                  desc: 'Mouse Fast'};


/**********************************************************************
 * Generic Desktop Page(0x01) - system power control
 **********************************************************************/
system_codes = [];
system_codes[0x81] = {id: 'PWR ',                        name: 'Sys Power',                   desc: 'System Power'};
system_codes[0x82] = {id: 'SLEP',                        name: 'Sys Sleep',                   desc: 'System Sleep'};
system_codes[0x83] = {id: 'WAKE',                        name: 'Sys Wake',                    desc: 'System Wake'};


/**********************************************************************
 * Consumer page(0x0C) - media/application control
 **********************************************************************/
consumer_codes = [];
consumer_codes[0x0E2] = {id: 'MUTE',                        name: 'Mute',                        desc: 'Audio Mute'};
consumer_codes[0x0E9] = {id: 'VOLU',                        name: 'Vol Up',                      desc: 'Audio Vol Up'};
consumer_codes[0x0EA] = {id: 'VOLD',                        name: 'Vol Down',                    desc: 'Audio Vol Down'};
consumer_codes[0x0B5] = {id: 'MNXT',                        name: 'Next Track',                  desc: 'Next Track'};
consumer_codes[0x0B6] = {id: 'MPRV',                        name: 'Prev Track',                  desc: 'Previous Track'};
consumer_codes[0x0B7] = {id: 'MSTP',                        name: 'Stop',                        desc: 'Media Stop'};
consumer_codes[0x0B8] = {id: 'EJCT',                        name: 'Eject',                       desc: 'Media Eject'};
consumer_codes[0x0CD] = {id: 'MPLY',                        name: 'Play Pause',                  desc: 'Play Pause'};
consumer_codes[0x183] = {id: 'MSEL',                        name: 'Media Select',                desc: 'Media Select'};
consumer_codes[0x0CC] = {id: 'STEJ',                        name: 'Stop Eject',                  desc: 'Stop Eject'};
consumer_codes[0x18A] = {id: 'MAIL',                        name: 'Mail',                        desc: 'Mail'};
consumer_codes[0x192] = {id: 'CALC',                        name: 'Calc',                        desc: 'Calculator'};
consumer_codes[0x194] = {id: 'MYCM',                        name: 'My Computer',                 desc: 'My Computer'};
consumer_codes[0x221] = {id: 'WSCH',                        name: 'Web Search',                  desc: 'WWW Search'};
consumer_codes[0x223] = {id: 'WHOM',                        name: 'Web Home',                    desc: 'WWW Home'};
consumer_codes[0x224] = {id: 'WBAK',                        name: 'Web Back',                    desc: 'WWW Back'};
consumer_codes[0x225] = {id: 'WFWD',                        name: 'Web Forward',                 desc: 'WWW Forward'};
consumer_codes[0x226] = {id: 'WSTP',                        name: 'Web Stop',                    desc: 'WWW Stop'};
consumer_codes[0x227] = {id: 'WREF',                        name: 'Web Refresh',                 desc: 'WWW Refresh'};
consumer_codes[0x22A] = {id: 'WFAV',                        name: 'Web Favorites',               desc: 'WWW Favorites'};
consumer_codes[0x0B3] = {id: 'MFFD',                        name: 'Fast Forward',                desc: 'Media Fast Forward(Mac)'};
consumer_codes[0x0B4] = {id: 'MRWD',                        name: 'Rewind',                      desc: 'Media Rewind(Mac)'};
consumer_codes[0x06F] = {id: 'BRTI',                        name: 'Bright Inc',                  desc: 'Brightness Increment'};
consumer_codes[0x070] = {id: 'BRTD',                        name: 'Bright Dec',                  desc: 'Brightness Decrement'};


/**********************************************************************
 * Mouse key - TMK specific codes
 **********************************************************************/
mousekey_codes = [];
mousekey_codes[0x00F0] = {id: 'MS_U',                        name: 'Mouse Up',                    desc: 'Mouse UP'};
mousekey_codes[0x00F1] = {id: 'MS_D',                        name: 'Mouse down',                  desc: 'Mouse Down'};
mousekey_codes[0x00F2] = {id: 'MS_L',                        name: 'Mouse left',                  desc: 'Mouse Left'};
mousekey_codes[0x00F3] = {id: 'MS_R',                        name: 'Mouse right',                 desc: 'Mouse Right'};
mousekey_codes[0x00F4] = {id: 'BTN1',                        name: 'Mouse Btn1',                  desc: 'Mouse Button1'};
mousekey_codes[0x00F5] = {id: 'BTN2',                        name: 'Mouse Btn2',                  desc: 'Mouse Button2'};
mousekey_codes[0x00F6] = {id: 'BTN3',                        name: 'Mouse Btn3',                  desc: 'Mouse Button3'};
mousekey_codes[0x00F7] = {id: 'BTN4',                        name: 'Mouse Btn4',                  desc: 'Mouse Button4'};
mousekey_codes[0x00F8] = {id: 'BTN5',                        name: 'Mouse Btn5',                  desc: 'Mouse Button5'};
mousekey_codes[0x00F9] = {id: 'WH_U',                        name: 'Wheel Up',                    desc: 'Wheel Up'};
mousekey_codes[0x00FA] = {id: 'WH_D',                        name: 'Wheel Down',                  desc: 'Wheel Down'};
mousekey_codes[0x00FB] = {id: 'WH_L',                        name: 'Wheel Left',                  desc: 'Wheel Left'};
mousekey_codes[0x00FC] = {id: 'WH_R',                        name: 'Wheel Right',                 desc: 'Wheel Right'};
mousekey_codes[0x00FD] = {id: 'ACL0',                        name: 'Mouse Slow',                  desc: 'Mouse Slow'};
mousekey_codes[0x00FE] = {id: 'ACL1',                        name: 'Mouse Medium',                desc: 'Mouse Medium'};
mousekey_codes[0x00FF] = {id: 'ACL2',                        name: 'Mouse Fast',                  desc: 'Mouse Fast'};


/**********************************************************************
 * Modifier combination
 **********************************************************************/
mods_codes = [];
mods_codes[0x00] = { id: 0x0,    name: "",                          desc: "none" };
mods_codes[0x01] = { id: 0x1,    name: "L\u2303",                   desc: "Left Control" };
mods_codes[0x02] = { id: 0x2,    name: "L\u21E7",                   desc: "Left Shift" };
mods_codes[0x03] = { id: 0x3,    name: "L\u21E7\u2303",             desc: "Left Shift + Control" };
mods_codes[0x04] = { id: 0x4,    name: "L\u2325",                   desc: "Left Alt" };
mods_codes[0x05] = { id: 0x5,    name: "L\u2325\u2303",             desc: "Left Alt + Control" };
mods_codes[0x06] = { id: 0x6,    name: "L\u2325\u21E7",             desc: "Left Alt + Shift" };
mods_codes[0x07] = { id: 0x7,    name: "L\u2325\u21E7\u2303",       desc: "Left Alt + Shift + Control" };
mods_codes[0x08] = { id: 0x8,    name: "L\u2318",                   desc: "Left Gui" };
mods_codes[0x09] = { id: 0x9,    name: "L\u2318\u2303",             desc: "Left Gui + Control" };
mods_codes[0x0A] = { id: 0xA,    name: "L\u2318\u21E7",             desc: "Left Gui + Shift" };
mods_codes[0x0B] = { id: 0xB,    name: "L\u2318\u21E7\u2303",       desc: "Left Gui + Shift + Control" };
mods_codes[0x0C] = { id: 0xC,    name: "L\u2318\u2325",             desc: "Left Gui + Alt" };
mods_codes[0x0D] = { id: 0xD,    name: "L\u2318\u2325\u2303",       desc: "Left Gui + Alt + Control" };
mods_codes[0x0E] = { id: 0xE,    name: "L\u2318\u2325\u21E7",       desc: "Left Gui + Alt + Shift" };
mods_codes[0x0F] = { id: 0xF,    name: "L\u2318\u2325\u21E7\u2303", desc: "Left Gui + Alt + Shift + Control" };
//mods_codes[0x10] = { id: 0x10,   name: "",                          desc: "none" };
mods_codes[0x11] = { id: 0x11,   name: "R\u2303",                   desc: "Right Control" };
mods_codes[0x12] = { id: 0x12,   name: "R\u21E7",                   desc: "Right Shift" };
mods_codes[0x13] = { id: 0x13,   name: "R\u21E7\u2303",             desc: "Right Shift + Control" };
mods_codes[0x14] = { id: 0x14,   name: "R\u2325",                   desc: "Right Alt" };
mods_codes[0x15] = { id: 0x15,   name: "R\u2325\u2303",             desc: "Right Alt + Control" };
mods_codes[0x16] = { id: 0x16,   name: "R\u2325\u21E7",             desc: "Right Alt + Shift" };
mods_codes[0x17] = { id: 0x17,   name: "R\u2325\u21E7\u2303",       desc: "Right Alt + Shift + Control" };
mods_codes[0x18] = { id: 0x18,   name: "R\u2318",                   desc: "Right Gui" };
mods_codes[0x19] = { id: 0x19,   name: "R\u2318\u2303",             desc: "Right Gui + Control" };
mods_codes[0x1A] = { id: 0x1A,   name: "R\u2318\u21E7",             desc: "Right Gui + Shift" };
mods_codes[0x1B] = { id: 0x1B,   name: "R\u2318\u21E7\u2303",       desc: "Right Gui + Shift + Control" };
mods_codes[0x1C] = { id: 0x1C,   name: "R\u2318\u2325",             desc: "Right Gui + Alt" };
mods_codes[0x1D] = { id: 0x1D,   name: "R\u2318\u2325\u2303",       desc: "Right Gui + Alt + Control" };
mods_codes[0x1E] = { id: 0x1E,   name: "R\u2318\u2325\u21E7",       desc: "Right Gui + Alt + Shift" };
mods_codes[0x1F] = { id: 0x1F,   name: "R\u2318\u2325\u21E7\u2303", desc: "Right Gui + Alt + Shift + Control" };


/**********************************************************************
 * Timing of Action
 **********************************************************************/
on_codes = [];
//on_codes[0x0] = { id: 0x1, name: "Special Use", desc: "Special Use" };
on_codes[0x1] = { id: 0x1, name: "On Press", desc: "On Press" };
on_codes[0x2] = { id: 0x1, name: "On Release", desc: "On Release" };
on_codes[0x3] = { id: 0x1, name: "On Both", desc: "On Press and Release" };


/**********************************************************************
 * Commands
 **********************************************************************/
command_ids = [];
command_ids[0] = { id: 'BTLD', name: 'BTLD', desc: 'Start Bootloader' };
