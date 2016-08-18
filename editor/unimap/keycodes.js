// action kind bits
const ACT_MODS          = 0b0000;
const ACT_LMODS         = 0b0000;
const ACT_RMODS         = 0b0001;
const ACT_MODS_TAP      = 0b0010;
const ACT_LMODS_TAP     = 0b0010;
const ACT_RMODS_TAP     = 0b0011;
const ACT_USAGE         = 0b0100;
const ACT_MOUSEKEY      = 0b0101;
const ACT_LAYER         = 0b1000;
const ACT_LAYER_EXT     = 0b1001;
const ACT_LAYER_TAP     = 0b1010; /* Layer  0-15 */
const ACT_LAYER_TAP_EXT = 0b1011; /* Layer 16-31 */
const ACT_MACRO         = 0b1100;
const ACT_BACKLIGHT     = 0b1101;
const ACT_COMMAND       = 0b1110;
const ACT_FUNCTION      = 0b1111;

// Special values for key param of ACT_LAYER_TAP
// 0xF4-FF: reserved
const OP_TAP_TOGGLE     = 0xf0;
const OP_ON_OFF         = 0xf1;
const OP_OFF_ON         = 0xf2;
const OP_SET_CLEAR      = 0xf3;

// Bit operations
const OP_BIT_AND        = 0;
const OP_BIT_OR         = 1;
const OP_BIT_XOR        = 2;
const OP_BIT_SET        = 3;

// Bit operation timing
const ON_SPECIAL_USE    = 0;
const ON_PRESS          = 1;
const ON_RELEASE        = 2;
const ON_BOTH           = 3;

const MOD_LCTL = 0x01;
const MOD_LSFT = 0x02;
const MOD_LALT = 0x04;
const MOD_LGUI = 0x08;
const MOD_RCTL = 0x11;
const MOD_RSFT = 0x12;
const MOD_RALT = 0x14;
const MOD_RGUI = 0x18;

// special keycodes for MODS_TAP
const MODS_ONESHOT      = 0x00;
const MODS_TAP_TOGGLE   = 0x01;

// actoin usage page
const PAGE_SYSTEM       = 0;
const PAGE_CONSUMER     = 1;

// special opts for FUNCTION
const FUNC_TAP = 0x08;


var action_kinds = {
    KEY:                { id: "KEY",                    name: "ACTION_KEY",                 desc: "Normal key" },
    MODS_KEY:           { id: "MODS_KEY",               name: "ACTION_MODS_KEY",            desc: "Modified key" },
    MODS_TAP_KEY:       { id: "MODS_TAP_KEY",           name: "ACTION_MODS_TAP_KEY",        desc: "Tap key / Hold modifiers" },
    MODS_ONESHOT:       { id: "MODS_ONESHOT",           name: "ACTION_MODS_ONESHOT",        desc: "Oneshot modifiers" },
    MODS_TAP_TOGGLE:    { id: "MODS_TAP_TOGGLE",        name: "ACTION_MODS_TAP_TOGGLE",     desc: "Tap toggle modifiers" },
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
    LAYER_CLEAR:        { id: "LAYER_CLEAR",            name: "ACTION_LAYER_CLEAR",         desc: "Clear all layer state" },
    LAYER_MOMENTARY:    { id: "LAYER_MOMENTARY",        name: "ACTION_LAYER_MOMENTARY",     desc: "Momentary layer switch" },
    LAYER_TOGGLE:       { id: "LAYER_TOGGLE",           name: "ACTION_LAYER_TOGGLE",        desc: "Toggle a layer" },
    LAYER_INVERT:       { id: "LAYER_INVERT",           name: "ACTION_LAYER_INVERT",        desc: "Invert a layer" },
    LAYER_ON:           { id: "LAYER_ON",               name: "ACTION_LAYER_ON",            desc: "Turn on a layer" },
    LAYER_OFF:          { id: "LAYER_OFF",              name: "ACTION_LAYER_OFF",           desc: "Turn off a layer" },
    LAYER_SET:          { id: "LAYER_SET",              name: "ACTION_LAYER_SET",           desc: "Set a layer" },
    LAYER_ON_OFF:       { id: "LAYER_ON_OFF",           name: "ACTION_LAYER_ON_OFF",        desc: "Turn on with press and off with release" },
    LAYER_OFF_ON:       { id: "LAYER_OFF_ON",           name: "ACTION_LAYER_OFF_ON",        desc: "Turn off with press and on with release" },
    LAYER_SET_CLEAR:    { id: "LAYER_SET_CLEAR",        name: "ACTION_LAYER_SET_CLEAR",     desc: "Turn on only a layer with press clear with release" },
    LAYER_MODS:         { id: "LAYER_MODS",             name: "ACTION_LAYER_MODS",          desc: "Momentary layer switch with modifiers" },
    LAYER_TAP_KEY:      { id: "LAYER_TAP_KEY",          name: "ACTION_LAYER_TAP_KEY",       desc: "Momentary layer switch / Tap key" },
    LAYER_TAP_TOGGLE:   { id: "LAYER_TAP_TOGGLE",       name: "ACTION_LAYER_TAP_TOGGLE",    desc: "Momentary layer switch / Tap toggle" },
/*
    LAYER_BIT_AND:      { id: "LAYER_BIT_AND",          name: "ACTION_LAYER_BIT_AND",       desc: "Layer Bit AND" },
    LAYER_BIT_OR:       { id: "LAYER_BIT_OR",           name: "ACTION_LAYER_BIT_OR",        desc: "Layer Bit OR" },
    LAYER_BIT_XOR:      { id: "LAYER_BIT_XOR",          name: "ACTION_LAYER_BIT_XOR",       desc: "Layer Bit XOR" },
    LAYER_BIT_SET:      { id: "LAYER_BIT_SET",          name: "ACTION_LAYER_BIT_SET",       desc: "Layer Bit SET" },
    DEFAULT_LAYER_SET:  { id: "DEFAULT_LAYER_SET",      name: "ACTION_DEFAULT_LAYER_SET",   desc: "Set a default layer" },
*/
    UNKNOWN:            { id: "UNKNOWN",                name: "ACTION_UNKNOWN",             desc: "Unknown action" },
};

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
    LAYER_MODS:         ACT_LAYER_TAP<<12 | 0xe0,
    LAYER_TAP_TOGGLE:   ACT_LAYER_TAP<<12 | OP_TAP_TOGGLE,
    LAYER_ON_OFF:       ACT_LAYER_TAP<<12 | OP_ON_OFF,
    LAYER_OFF_ON:       ACT_LAYER_TAP<<12 | OP_OFF_ON,
    LAYER_SET_CLEAR:    ACT_LAYER_TAP<<12 | OP_SET_CLEAR,
    LAYER_BIT_AND:      ACT_LAYER<<12 | OP_BIT_AND<<10,
    LAYER_BIT_OR:       ACT_LAYER<<12 | OP_BIT_OR<<10,
    LAYER_BIT_XOR:      ACT_LAYER<<12 | OP_BIT_XOR<<10,
    LAYER_BIT_SET:      ACT_LAYER<<12 | OP_BIT_SET<<10,
    DEFAULT_LAYER_SET:  ACT_LAYER<<12 | OP_BIT_SET<<10 | ON_SPECIAL_USE<<8,
    UNKNOWN:            0,
};



// action object
function Action(code = 0) {
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
    // ACT_MOUSEKEY
    Object.defineProperty(this, "mousekey_code", {
        set: function(val)  { this.code |= (val & 0xff); },
        get: function()     { return (this.code & 0x00ff); }
    });


    var mods_str = function(mods) {
        var mods_name = "", mods_desc = []
        if (mods & 0x01)  mods_name += "\u2303", mods_desc.push("Control");
        if (mods & 0x02)  mods_name += "\u21E7", mods_desc.push("Shift");
        if (mods & 0x04)  mods_name += "\u2325", mods_desc.push("Alt");
        if (mods & 0x08)  mods_name += "\u2318", mods_desc.push("Gui");
        mods_desc = mods_desc.join(" + ");
        if (mods & 0x10)  mods_name = "r" + mods_name, mods_desc = "Right " + mods_desc;
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
                        return action_kinds.LAYER_BIT_AND;
                    case OP_BIT_OR:
                        return action_kinds.LAYER_BIT_OR;
                    case OP_BIT_XOR:
                        if (this.layer_bitop_on == ON_RELEASE && this.layer_bitop_xbit == 0 &&
                                (this.layer_bitop_bits == 1 ||
                                 this.layer_bitop_bits == 2 ||
                                 this.layer_bitop_bits == 4 ||
                                 this.layer_bitop_bits == 8)) {
                            // calculate layer: part*4 + MSB(bits)
                            var _layer = (this.layer_bitop_part * 4);
                            for (var _bits = this.layer_bitop_bits; _bits>>1; _bits>>=1) _layer++;
                            return $.extend({}, action_kinds.LAYER_TOGGLE,
                                    {
                                        name: "T" + _layer,
                                        desc: "Toggle on Layer " + _layer
                                    });
                        }
                        return action_kinds.LAYER_BIT_XOR;
                    case OP_BIT_SET:
                        return action_kinds.LAYER_BIT_SET;
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
                                    desc: "Switch Layer " + this.layer_tap_val + " with Tap toggle"
                                });
                    case OP_ON_OFF:
                        return $.extend({}, action_kinds.LAYER_MOMENTARY,
                                {
                                    name: "L" + this.layer_tap_val,
                                    desc: "Change to Layer " + this.layer_tap_val + "(Momentary)"
                                });
                    case OP_OFF_ON:
                        return action_kinds.LAYER_OFF_ON;
                    case OP_SET_CLEAR:
                        return action_kinds.LAYER_SET_CLEAR;
                    default:
                        if ((this.layer_tap_code & 0xf0) == 0xe0) {
                            var _mods_str =  mods_str(this.layer_tap_code & 0x0f);
                            return $.extend({}, action_kinds.LAYER_MODS,
                                    {
                                        name: "LM" + this.layer_tap_val + " " + _mods_str.name,
                                        desc: "Change to Layer " + this.layer_tap_val + " with " + _mods_str.desc
                                    });
                        }
                        else {
                            return $.extend({}, action_kinds.LAYER_TAP_KEY,
                                    {
                                        name: "LT" + this.layer_tap_val + " " + keycodes[this.layer_tap_code].name,
                                        desc: "Change to Layer " + this.layer_tap_val + " and " + keycodes[this.layer_tap_code].name + "(tap)"
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
                    case PAGE_CONSUMER:
                        if (consumer_codes[this.usage_code])
                            return $.extend({}, action_kinds.USAGE_CONSUMER,
                                    {
                                        name: consumer_codes[this.usage_code].name,
                                        desc: consumer_codes[this.usage_code].desc
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
        };
        return action_kinds.UNKNOWN;
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


/*
 * Keyboard/Keypad Page(0x07) - keycodes
 */
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
/*
keycodes[0x00A5] = {id: 'RESERVED-165',                name: 'RESERVED-165',                desc: 'RESERVED-165(0xA5)'};
keycodes[0x00A6] = {id: 'RESERVED-166',                name: 'RESERVED-166',                desc: 'RESERVED-166(0xA6)'};
keycodes[0x00A7] = {id: 'RESERVED-167',                name: 'RESERVED-167',                desc: 'RESERVED-167(0xA7)'};
keycodes[0x00A8] = {id: 'RESERVED-168',                name: 'RESERVED-168',                desc: 'RESERVED-168(0xA8)'};
keycodes[0x00A9] = {id: 'RESERVED-169',                name: 'RESERVED-169',                desc: 'RESERVED-169(0xA9)'};
keycodes[0x00AA] = {id: 'RESERVED-170',                name: 'RESERVED-170',                desc: 'RESERVED-170(0xAA)'};
keycodes[0x00AB] = {id: 'RESERVED-171',                name: 'RESERVED-171',                desc: 'RESERVED-171(0xAB)'};
keycodes[0x00AC] = {id: 'RESERVED-172',                name: 'RESERVED-172',                desc: 'RESERVED-172(0xAC)'};
keycodes[0x00AD] = {id: 'RESERVED-173',                name: 'RESERVED-173',                desc: 'RESERVED-173(0xAD)'};
keycodes[0x00AE] = {id: 'RESERVED-174',                name: 'RESERVED-174',                desc: 'RESERVED-174(0xAE)'};
keycodes[0x00AF] = {id: 'RESERVED-175',                name: 'RESERVED-175',                desc: 'RESERVED-175(0xAF)'};
*/
keycodes[0x00B0] = {id: 'KP_00',                       name: 'KP_00',                       desc: 'KP_00'};
keycodes[0x00B1] = {id: 'KP_000',                      name: 'KP_000',                      desc: 'KP_000'};
keycodes[0x00B2] = {id: 'THOUSANDS_SEPARATOR',         name: 'THOUSANDS_SEPARATOR',         desc: 'THOUSANDS_SEPARATOR'};
keycodes[0x00B3] = {id: 'DECIMAL_SEPARATOR',           name: 'DECIMAL_SEPARATOR',           desc: 'DECIMAL_SEPARATOR'};
keycodes[0x00B4] = {id: 'CURRENCY_UNIT',               name: 'CURRENCY_UNIT',               desc: 'CURRENCY_UNIT'};
keycodes[0x00B5] = {id: 'CURRENCY_SUB_UNIT',           name: 'CURRENCY_SUB_UNIT',           desc: 'CURRENCY_SUB_UNIT'};
keycodes[0x00B6] = {id: 'KP_LPAREN',                   name: 'KP_LPAREN',                   desc: 'KP_LPAREN'};
keycodes[0x00B7] = {id: 'KP_RPAREN',                   name: 'KP_RPAREN',                   desc: 'KP_RPAREN'};
keycodes[0x00B8] = {id: 'KP_LCBRACKET',                name: 'KP_LCBRACKET',                desc: 'KP_LCBRACKET'};
keycodes[0x00B9] = {id: 'KP_RCBRACKET',                name: 'KP_RCBRACKET',                desc: 'KP_RCBRACKET'};
keycodes[0x00BA] = {id: 'KP_TAB',                      name: 'KP_TAB',                      desc: 'KP_TAB'};
keycodes[0x00BB] = {id: 'KP_BSPACE',                   name: 'KP_BSPACE',                   desc: 'KP_BSPACE'};
keycodes[0x00BC] = {id: 'KP_A',                        name: 'KP_A',                        desc: 'KP_A'};
keycodes[0x00BD] = {id: 'KP_B',                        name: 'KP_B',                        desc: 'KP_B'};
keycodes[0x00BE] = {id: 'KP_C',                        name: 'KP_C',                        desc: 'KP_C'};
keycodes[0x00BF] = {id: 'KP_D',                        name: 'KP_D',                        desc: 'KP_D'};
keycodes[0x00C0] = {id: 'KP_E',                        name: 'KP_E',                        desc: 'KP_E'};
keycodes[0x00C1] = {id: 'KP_F',                        name: 'KP_F',                        desc: 'KP_F'};
keycodes[0x00C2] = {id: 'KP_XOR',                      name: 'KP_XOR',                      desc: 'KP_XOR'};
keycodes[0x00C3] = {id: 'KP_HAT',                      name: 'KP_HAT',                      desc: 'KP_HAT'};
keycodes[0x00C4] = {id: 'KP_PERC',                     name: 'KP_PERC',                     desc: 'KP_PERC'};
keycodes[0x00C5] = {id: 'KP_LT',                       name: 'KP_LT',                       desc: 'KP_LT'};
keycodes[0x00C6] = {id: 'KP_GT',                       name: 'KP_GT',                       desc: 'KP_GT'};
keycodes[0x00C7] = {id: 'KP_AND',                      name: 'KP_AND',                      desc: 'KP_AND'};
keycodes[0x00C8] = {id: 'KP_LAZYAND',                  name: 'KP_LAZYAND',                  desc: 'KP_LAZYAND'};
keycodes[0x00C9] = {id: 'KP_OR',                       name: 'KP_OR',                       desc: 'KP_OR'};
keycodes[0x00CA] = {id: 'KP_LAZYOR',                   name: 'KP_LAZYOR',                   desc: 'KP_LAZYOR'};
keycodes[0x00CB] = {id: 'KP_COLON',                    name: 'KP_COLON',                    desc: 'KP_COLON'};
keycodes[0x00CC] = {id: 'KP_HASH',                     name: 'KP_HASH',                     desc: 'KP_HASH'};
keycodes[0x00CD] = {id: 'KP_SPACE',                    name: 'KP_SPACE',                    desc: 'KP_SPACE'};
keycodes[0x00CE] = {id: 'KP_ATMARK',                   name: 'KP_ATMARK',                   desc: 'KP_ATMARK'};
keycodes[0x00CF] = {id: 'KP_EXCLAMATION',              name: 'KP_EXCLAMATION',              desc: 'KP_EXCLAMATION'};
keycodes[0x00D0] = {id: 'KP_MEM_STORE',                name: 'KP_MEM_STORE',                desc: 'KP_MEM_STORE'};
keycodes[0x00D1] = {id: 'KP_MEM_RECALL',               name: 'KP_MEM_RECALL',               desc: 'KP_MEM_RECALL'};
keycodes[0x00D2] = {id: 'KP_MEM_CLEAR',                name: 'KP_MEM_CLEAR',                desc: 'KP_MEM_CLEAR'};
keycodes[0x00D3] = {id: 'KP_MEM_ADD',                  name: 'KP_MEM_ADD',                  desc: 'KP_MEM_ADD'};
keycodes[0x00D4] = {id: 'KP_MEM_SUB',                  name: 'KP_MEM_SUB',                  desc: 'KP_MEM_SUB'};
keycodes[0x00D5] = {id: 'KP_MEM_MUL',                  name: 'KP_MEM_MUL',                  desc: 'KP_MEM_MUL'};
keycodes[0x00D6] = {id: 'KP_MEM_DIV',                  name: 'KP_MEM_DIV',                  desc: 'KP_MEM_DIV'};
keycodes[0x00D7] = {id: 'KP_PLUS_MINUS',               name: 'KP_PLUS_MINUS',               desc: 'KP_PLUS_MINUS'};
keycodes[0x00D8] = {id: 'KP_CLEAR',                    name: 'KP_CLEAR',                    desc: 'KP_CLEAR'};
keycodes[0x00D9] = {id: 'KP_CLEAR_ENTRY',              name: 'KP_CLEAR_ENTRY',              desc: 'KP_CLEAR_ENTRY'};
keycodes[0x00DA] = {id: 'KP_BINARY',                   name: 'KP_BINARY',                   desc: 'KP_BINARY'};
keycodes[0x00DB] = {id: 'KP_OCTAL',                    name: 'KP_OCTAL',                    desc: 'KP_OCTAL'};
keycodes[0x00DC] = {id: 'KP_DECIMAL',                  name: 'KP_DECIMAL',                  desc: 'KP_DECIMAL'};
keycodes[0x00DD] = {id: 'KP_HEXADECIMAL',              name: 'KP_HEXADECIMAL',              desc: 'KP_HEXADECIMAL'};
/*
keycodes[0x00DE] = {id: 'RESERVED-222',                name: 'RESERVED-222',                desc: 'RESERVED-222(0xDE)'};
keycodes[0x00DF] = {id: 'RESERVED-223',                name: 'RESERVED-223',                desc: 'RESERVED-223(0xDF)'};
*/
keycodes[0x00E0] = {id: 'LCTL',                        name: 'LCtrl',                       desc: 'Left Control'};
keycodes[0x00E1] = {id: 'LSFT',                        name: 'LShift',                      desc: 'Left Shift'};
keycodes[0x00E2] = {id: 'LALT',                        name: 'LAlt',                        desc: 'Left Alt(\u2325)'};
keycodes[0x00E3] = {id: 'LGUI',                        name: 'LGui',                        desc: 'Left Windows(\u2318)'};
keycodes[0x00E4] = {id: 'RCTL',                        name: 'RCtrl',                       desc: 'Right Control'};
keycodes[0x00E5] = {id: 'RSFT',                        name: 'RShift',                      desc: 'Right Shift'};
keycodes[0x00E6] = {id: 'RALT',                        name: 'RAlt',                        desc: 'Right Alt(\u2325)'};
keycodes[0x00E7] = {id: 'RGUI',                        name: 'RGui',                        desc: 'Right Windows(\u2318)'};
/*
keycodes[0x00E8] = {id: 'RESERVED-232',                name: 'RESERVED-232',                desc: 'RESERVED-232(0xE8)'};
keycodes[0x00E9] = {id: 'RESERVED-233',                name: 'RESERVED-233',                desc: 'RESERVED-233(0xE9)'};
keycodes[0x00EA] = {id: 'RESERVED-234',                name: 'RESERVED-234',                desc: 'RESERVED-234(0xEA)'};
keycodes[0x00EB] = {id: 'RESERVED-235',                name: 'RESERVED-235',                desc: 'RESERVED-235(0xEB)'};
keycodes[0x00EC] = {id: 'RESERVED-236',                name: 'RESERVED-236',                desc: 'RESERVED-236(0xEC)'};
keycodes[0x00ED] = {id: 'RESERVED-237',                name: 'RESERVED-237',                desc: 'RESERVED-237(0xED)'};
keycodes[0x00EE] = {id: 'RESERVED-238',                name: 'RESERVED-238',                desc: 'RESERVED-238(0xEE)'};
keycodes[0x00EF] = {id: 'RESERVED-239',                name: 'RESERVED-239',                desc: 'RESERVED-239(0xEF)'};
keycodes[0x00F0] = {id: 'RESERVED-240',                name: 'RESERVED-240',                desc: 'RESERVED-240(0xF0)'};
keycodes[0x00F1] = {id: 'RESERVED-241',                name: 'RESERVED-241',                desc: 'RESERVED-241(0xF1)'};
keycodes[0x00F2] = {id: 'RESERVED-242',                name: 'RESERVED-242',                desc: 'RESERVED-242(0xF2)'};
keycodes[0x00F3] = {id: 'RESERVED-243',                name: 'RESERVED-243',                desc: 'RESERVED-243(0xF3)'};
keycodes[0x00F4] = {id: 'RESERVED-244',                name: 'RESERVED-244',                desc: 'RESERVED-244(0xF4)'};
keycodes[0x00F5] = {id: 'RESERVED-245',                name: 'RESERVED-245',                desc: 'RESERVED-245(0xF5)'};
keycodes[0x00F6] = {id: 'RESERVED-246',                name: 'RESERVED-246',                desc: 'RESERVED-246(0xF6)'};
keycodes[0x00F7] = {id: 'RESERVED-247',                name: 'RESERVED-247',                desc: 'RESERVED-247(0xF7)'};
keycodes[0x00F8] = {id: 'RESERVED-248',                name: 'RESERVED-248',                desc: 'RESERVED-248(0xF8)'};
keycodes[0x00F9] = {id: 'RESERVED-249',                name: 'RESERVED-249',                desc: 'RESERVED-249(0xF9)'};
keycodes[0x00FA] = {id: 'RESERVED-250',                name: 'RESERVED-250',                desc: 'RESERVED-250(0xFA)'};
keycodes[0x00FB] = {id: 'RESERVED-251',                name: 'RESERVED-251',                desc: 'RESERVED-251(0xFB)'};
keycodes[0x00FC] = {id: 'RESERVED-252',                name: 'RESERVED-252',                desc: 'RESERVED-252(0xFC)'};
keycodes[0x00FD] = {id: 'RESERVED-253',                name: 'RESERVED-253',                desc: 'RESERVED-253(0xFD)'};
keycodes[0x00FE] = {id: 'RESERVED-254',                name: 'RESERVED-254',                desc: 'RESERVED-254(0xFE)'};
keycodes[0x00FF] = {id: 'RESERVED-255',                name: 'RESERVED-255',                desc: 'RESERVED-255(0xFF)'};
*/

/*
 * Generic Desktop Page(0x01) - system power control
 */
system_codes = [];
system_codes[0x81] = {id: 'PWR ',                        name: 'Sys Power',                   desc: 'System Power'};
system_codes[0x82] = {id: 'SLEP',                        name: 'Sys Sleep',                   desc: 'System Sleep'};
system_codes[0x83] = {id: 'WAKE',                        name: 'Sys Wake',                    desc: 'System Wake'};

/*
 * Consumer page(0x0C)
 */
consumer_codes = [];
consumer_codes[0x0E2] = {id: 'MUTE',                        name: 'Mute',                        desc: 'Audio Mute'};
consumer_codes[0x0E9] = {id: 'VOLU',                        name: 'Vol Up',                      desc: 'Audio Vol Up'};
consumer_codes[0x0EA] = {id: 'VOLD',                        name: 'Vol Down',                    desc: 'Audio Vol Down'};
consumer_codes[0x0B5] = {id: 'MNXT',                        name: 'Next Track',                  desc: 'Next Track'};
consumer_codes[0x0B6] = {id: 'MPRV',                        name: 'Prev Track',                  desc: 'Previous Track'};
consumer_codes[0x0B7] = {id: 'MSTP',                        name: 'Stop',                        desc: 'Media Stop'};
consumer_codes[0x0CD] = {id: 'MPLY',                        name: 'Play Pause',                  desc: 'Play Pause'};
consumer_codes[0x083] = {id: 'MSEL',                        name: 'Media Select',                desc: 'Media Select'};
consumer_codes[0x0CC] = {id: 'EJCT',                        name: 'Eject',                       desc: 'Media Eject'};
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

/*
 * Mouse key - TMK specific codes
 */
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

/*
 * Modifier key in Action
 */
/*
var id_mods = [];
mods_ids[0x01] = { id: MOD_LCTL,     name: "Left Control",       desc: "Left Control" };
mods_ids[0x02] = { id: MOD_LSFT,     name: "Left Shift",         desc: "Left Shift" };
mods_ids[0x04] = { id: MOD_LALT,     name: "Left Alt",           desc: "Left Alt" };
mods_ids[0x08] = { id: MOD_LGUI,     name: "Left Gui",           desc: "Left Gui" };
mods_ids[0x11] = { id: MOD_RCTL,     name: "Right Control",      desc: "Right Control" };
mods_ids[0x12] = { id: MOD_RSFT,     name: "Right Shift",        desc: "Right Shift" };
mods_ids[0x14] = { id: MOD_RALT,     name: "Right Alt",          desc: "Right Alt" };
mods_ids[0x18] = { id: MOD_RGUI,     name: "Right Gui",          desc: "Right Gui" };
*/

/*
 * Modifier combination in Action
 */
mods_codes = [];
mods_codes[0x0] = { id: 0x0,    name: "    ",   desc: "none" };
mods_codes[0x1] = { id: 0x1,    name: "   \u2303",   desc: "Control" };
mods_codes[0x2] = { id: 0x2,    name: "  \u21E7 ",   desc: "Shift" };
mods_codes[0x3] = { id: 0x3,    name: "  \u21E7\u2303",   desc: "Shift + Control" };
mods_codes[0x4] = { id: 0x4,    name: " \u2325  ",   desc: "Alt" };
mods_codes[0x5] = { id: 0x5,    name: " \u2325 \u2303",   desc: "Alt + Control" };
mods_codes[0x6] = { id: 0x6,    name: " \u2325\u21E7 ",   desc: "Alt + Shift" };
mods_codes[0x7] = { id: 0x7,    name: " \u2325\u21E7\u2303",   desc: "Alt + Shift + Control" };
mods_codes[0x8] = { id: 0x8,    name: "\u2318   ",   desc: "Gui" };
mods_codes[0x9] = { id: 0x9,    name: "\u2318  \u2303",   desc: "Gui + Control" };
mods_codes[0xA] = { id: 0xA,    name: "\u2318 \u21E7 ",   desc: "Gui + Shift" };
mods_codes[0xB] = { id: 0xB,    name: "\u2318 \u21E7\u2303",   desc: "Gui + Shift + Control" };
mods_codes[0xC] = { id: 0xC,    name: "\u2318\u2325  ",   desc: "Gui + Alt" };
mods_codes[0xD] = { id: 0xD,    name: "\u2318\u2325 \u2303",   desc: "Gui + Alt + Control" };
mods_codes[0xE] = { id: 0xE,    name: "\u2318\u2325\u21E7 ",   desc: "Gui + Alt + Shift" };
mods_codes[0xF] = { id: 0xF,    name: "\u2318\u2325\u21E7\u2303",   desc: "Gui + Alt + Shift + Control" };

/*
 * Timing of Action
 */
on_codes = [];
//on_codes[0x0] = { id: 0x1, name: "Special Use", desc: "Special Use" };
on_codes[0x1] = { id: 0x1, name: "On Press", desc: "On Press" };
on_codes[0x2] = { id: 0x1, name: "On Release", desc: "On Release" };
on_codes[0x3] = { id: 0x1, name: "On Both", desc: "On Press and Release" };
