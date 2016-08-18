/*
Unimap - Universal 128-key keyboard layout(8x16)
        ,-----------------------------------------------.
        |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
|Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |PrS|ScL|Pau|     |VDn|VUp|Mut|
`---'   `-----------------------------------------------'     `-----------'     `-----------'
,-----------------------------------------------------------. ,-----------. ,---------------.
|  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
|-----------------------------------------------------------| |-----------| |---------------|
|Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +|
|-----------------------------------------------------------| `-----------' |---------------|
|CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Retn|               |  4|  5|  6|KP,|
|-----------------------------------------------------------|     ,---.     |---------------|
|Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /| RO|Shift |     |Up |     |  1|  2|  3|KP=|
|-----------------------------------------------------------| ,-----------. |---------------|
|Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|Ent|
`-----------------------------------------------------------' `-----------' `---------------'
App:         Windows Menu key
Gui:         Windows key, Mac ⌘ key or Meta key
VDn Vup Mut: Volume control
< #:         ISO keys(in UK legend)
KP=:         Keypad = for Mac
KP,:         Brazilian Keypad Comma
JPY:         Japanese Yen(￥)
RO:          Japanese ろ(Ro) or Brazilian /(Slash)
MHEN:        Japanese 無変換(Non Conversion) or Korean Hanja
HENK:        Japanese 変換(Conversion) or Korean Hangul/English
KANA:        Japanese かな(Hiragana/Katakana)
https://en.wikipedia.org/wiki/Keyboard_layout#Japanese
https://en.wikipedia.org/wiki/Keyboard_layout#Hangul_.28for_Korean.29

Key:    8x16 = 128 keys
Layer:  2(bytes/action) * 128 = 256 bytes
        8 layers can be defined in 2KB area.(256 * 8 = 2048)
*/
// Keymap section address
KEYMAP_START_ADDRESS = 0x6800;


/**********************************************************************
 * Keymaps
 **********************************************************************/
no_map = function() { return [
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
    [ 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 ],
]; };

transparent_map = function() { return [
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
    [ 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1 ],
]; };

// default keymap
keymaps = [
    [
        [ 0x0088, 0x44EA, 0x44E9, 0x44E2, 0x0004, 0x0005, 0x0006, 0x0007,
          0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F ],
        [ 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
          0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F ], 
        [ 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
          0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F ], 
        [ 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
          0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F ], 
        [ 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
          0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F ], 
        [ 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
          0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F ], 
        [ 0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0085, 0x0067,
          0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F ], 
        [ 0x0070, 0x0071, 0x0072, 0x0073, 0x0089, 0x0087, 0x008A, 0x008B,
          0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7 ], 
    ],
    transparent_map(),
    transparent_map(),
    transparent_map(),
    transparent_map(),
    transparent_map(),
    transparent_map(),
    transparent_map(),
];


/**********************************************************************
 * Source output
 **********************************************************************/
function source_output(keymaps) {
    var output = '';
    output += "#include \"action.h\"\n";
    output += "#include \"action_code.h\"\n";
    output += "#include \"actionmap.h\"\n";
    output += "\n";
    output += "const action_t actionmaps[][";
    output += keymaps[0].length;         // row
    output += "][";
    output += keymaps[0][0].length;      // col
    output += "] __attribute__ ((section (\".keymap.keymaps\"))) = {\n";
    for (var i in keymaps) {
        output += "    {\n";
        for (var j in keymaps[i]) {
            output += "        { ";
            for (var k in keymaps[i][j]) {
                output += '0x' + ('0' + keymaps[i][j][k].toString(16)).substr(-2);
                output += ',';
            }
            output += " },\n";
        }
        output += "    },\n";
    }
    output += "};\n";
    return output;
};


/**********************************************************************
 * Hex output
 **********************************************************************/
function hex_line(address, record_type, data) {
    var hexstr2 = function(b) {
        return ('0'+ b.toString(16)).substr(-2).toUpperCase();
    };

    var sum = 0;
    sum += data.length;
    sum += (address >> 8);
    sum += (address & 0xff);
    sum += record_type;

    var line = '';
    line += ':';
    line += hexstr2(data.length);
    line += hexstr2(address >> 8);
    line += hexstr2(address & 0xff);
    line += hexstr2(record_type);
    for (var i = 0; i < data.length; i++) {
        sum = (sum + data[i]);
        line += hexstr2(data[i]);
    }
    line += hexstr2((~sum + 1)&0xff);  // Checksum
    line +="\r\n";
    return line;
};

function hex_eof() {
    return ":00000001FF\r\n";
};

function hex_output(address, data) {
    var output = '';
    var line = [];

    // flatten data into one dimension array
    [].concat.apply([], [].concat.apply([], data)).forEach(function(e) {
        line.push(e);
        if (line.length == 16) {
            output += hex_line(address, 0x00, line);
            address += 16;
            line.length = 0;   // clear array
        }
    });
    if (line.length > 0) {
        output += hex_line(address, 0x00, line);
    }
    return output;
};

/* hex file whthout keymap region and eof */
function hex_firmware() {
    /*  Flash Map of ATMega32U2/U4(32KB)
     *  +------------+ 0x0000
     *  | .vectors   | 0xac (43vectors * 4bytes)
     *  | .progmem   | PROGMEM variables and PSTR
     *  | .init0-9   |
     *  | .text      | code
     *  | .fini9-0   |
     *  |            | > text region
     *  |------------| _etext
     *  | .data      |
     *  | .bss       |
     *  | .noinit    |
     *  |            | > data region
     *  |------------| 0x6800
     *  | .keymap    | > keymap region(2KB)
     *  |------------| 0x7000
     *  | bootloader | 4KB
     *  +------------+ 0x7FFF
     *
     *  keymap region(.keymap):
     *  +----------------+
     *  |                |
     *  |                |
     *  | keymaps[][][]  | < 2KB-64
     *  |                |
     *  |                |
     *  +----------------+
     */
    // TODO: read hex file directly from remote URL
    // Place HEX format string of firmware(0x0000-0x67ff) excluding keymap region.
    return "\
";
}


/**********************************************************************
 * URL encode/decode
 **********************************************************************/
function url_encode_keymap(obj) {
    if (typeof LZString != "undefined" && typeof Base64 != "undefined") {
        return Base64.encode(LZString.compress(JSON.stringify(obj)));
    }
    return window.btoa(JSON.stringify(obj));
};

function url_decode_keymap(str) {
    try {
        // lz-string-1.3.3.js: LZString.decompress() runs away if given short string.
        if (str == null || typeof str != "string" || str.length < 30) return null;

        if (typeof LZString != "undefined" && typeof Base64 != "undefined") {
            return JSON.parse(LZString.decompress(Base64.decode(str)));
        }
        return JSON.parse(window.atob(str));
    } catch (err) {
        return null;
    }
};
