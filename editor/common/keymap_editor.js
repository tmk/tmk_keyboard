/*
 * TMK keymap editor
 */
// key id under editing
var editing_key;
// layer under editing
var editing_layer = 0;

// load keymap on keyboard key buttons
var load_keymap_on_keyboard = function(layer, keymap) {
    for (var row in keymap) {
        for (var col in keymap[row]) {
            var code = keymap[row][col];
            var key = keycodes[code];
            // row and column takes range of 0-32(0-9a-v)
            $("#key-" + parseInt(row).toString(32) + parseInt(col).toString(32)).text(key.name);
            $("#key-" + parseInt(row).toString(32) + parseInt(col).toString(32)).attr({ title: key.desc });
        }
    }
};

$(function() {
    // Title
    document.title = "TMK Keymap Editor for " + KEYBOARD_DESC;
    $("#page-title").text("TMK Keymap Editor for " + KEYBOARD_DESC);

    /*
     * load keymap from URL hash
     */
    var decoded = decode_keymap(document.location.hash.substring(1));
    if (decoded != null) {
        keymaps = decoded['keymaps'];
    }



    /*
     * Layer selector
     */
    $("#layer_radio").buttonset();

    // layer change
    $(".layer").click(function(ev, ui) {
        var layer = parseInt($(this).attr('id').match(/layer-(\d+)/)[1]);
        editing_layer = layer;
        load_keymap_on_keyboard(layer, keymaps[layer]);
    });



    /*
     * Keyboard(key buttons)
     */
    // load default keymap on startup
    load_keymap_on_keyboard(0, keymaps[0]);

    // Select key button to edit
    $(".key").click(function(ev, ui) {
        editing_key = $(this).attr('id');

        // grey-out key to indicate being under editing
        $(".key").removeClass("key-editing");
        $(this).addClass("key-editing");
    }).focus(function(ev, ui) {
        // select editing_key with tab key focus
        $(this).click();
    });



    /*
     * Keycodes button tab
     */
    $("#keycode_tabs").tabs({
        heightStyle: "auto",
    });

    // Keycodes: read name and description from code table
    $(".action").each(function(index) {
        // get code from code button id: code-[0x]CCCC where CCCC is dec or hex number
        var code = parseInt($(this).attr('id').match(/code-((0x){0,1}[0-9a-fA-F]+)/)[1]);
        $(this).text(keycodes[code].name);
        $(this).attr({ title: keycodes[code].desc });
    });

    $(".action").click(function(ev,ui) {
        if (!editing_key) return;

        // get matrix position from key id: key-RC where R is row and C is column in "0-v"(radix 32)
        var pos = editing_key.match(/key-([0-9a-v])([0-9a-v])/i);
        if (!pos) return;
        var row = parseInt(pos[1], 32), col = parseInt(pos[2], 32);

        // set text and tooltip to key button under editing
        $("#" + editing_key).text($(this).text());
        $("#" + editing_key).attr({ title: $(this).attr('title'), });

        // change keymap array
        // get code from keycode button id: code-[0x]CC where CC is dec or hex number
        var code = $(this).attr('id').match(/code-((0x){0,1}[0-9a-fA-F]+)/)[1];
        keymaps[editing_layer][row][col] = parseInt(code);

        // give focus on editing_key for next tab key operation
        $("#" + editing_key).focus();
    });


    /*
     * Share URL
     */
    // Share URL
    $("#keymap-share").click(function(ev, ui) {
        var hash = encode_keymap({ keymaps: keymaps });
        $("#share-url").text(document.location.origin + document.location.pathname + "#" + hash);
    });

    // Shorten URL
    $("#shorten-url").click(function(ev, ui) {
        var hash = encode_keymap({ keymaps: keymaps });
        var editor_url = document.location.origin + document.location.pathname;
        window.open("https://bitly.com/shorten/?url=" + encodeURIComponent(editor_url + "#" + hash));
        //window.open("http://tinyurl.com/create.php?url=" + encodeURIComponent(editor_url + "#" + hash));
    });


    // Hex Save
    $("#keymap-download").click(function(ev, ui) {
        var keymap_data = fn_actions.concat(keymaps);
        var content = firmware_hex() +
                      hex_output(KEYMAP_START_ADDRESS, keymap_data) +
                      hex_eof();

        // download hex file
        var blob = new Blob([content], {type: "application/octet-stream"});
        var hex_link = $("#hex-download");
        hex_link.attr('href', window.URL.createObjectURL(blob));
        hex_link.attr('download', KEYBOARD_ID + "_firmware.hex");
        // jQuery click() doesn't work straight for 'a' element
        // http://stackoverflow.com/questions/1694595/
        hex_link[0].click();
    });



    /*
     * Output options
     */
    //$("#keymap-output").resizable();  // resizable textarea

    // Hex output
    $("#keymap-hex-generate").click(function(ev, ui) {
        var keymap_data = fn_actions.concat(keymaps);
        $("#keymap-output").text(hex_output(KEYMAP_START_ADDRESS, keymap_data));
    });

    // C source output
    $("#keymap-source-generate").click(function(ev, ui) {
        $("#keymap-output").text(source_output(keymaps));
    });

    // JSON output
    //$("#keymap-json-generate").css('display', 'none');  // hide
    $("#keymap-json-generate").click(function(ev, ui) {
        var keymap_output;
        //keymap_output = JSON.stringify(keymaps, null, 4);
        keymap_output = JSON.stringify({ keymaps: keymaps });
        $("#keymap-output").text(keymap_output);
    });

    // encode keymap
    $("#keymap-encode").click(function(ev, ui) {
        var keymap_output = encode_keymap({ keymaps: keymaps });
        $("#keymap-output").text(keymap_output);
    });

    // decode  keymap
    $("#keymap-decode").click(function(ev, ui) {
        var hash = $("#keymap-output").text();
        var keymap_output = decode_keymap(hash);
        $("#keymap-output").text(JSON.stringify(keymap_output));
    });



    // lost keymap under edting when leave the page
    // TODO: needed only if keymap is changed
    $(window).bind('beforeunload', function(){
          return 'CAUTION: You will lost your change.';
    });
});



/*
 * Share URL
 */
function encode_keymap(obj)
{
    if (typeof LZString != "undefined" && typeof Base64 != "undefined") {
        return Base64.encode(LZString.compress(JSON.stringify(obj)));
    }
    return window.btoa(JSON.stringify(obj));
}

function decode_keymap(str)
{
    try {
        /* lz-string-1.3.3.js: LZString.decompress() runs away if given short string. */
        if (str == null || typeof str != "string" || str.length < 30) return null;

        if (typeof LZString != "undefined" && typeof Base64 != "undefined") {
            return JSON.parse(LZString.decompress(Base64.decode(str)));
        }
        return JSON.parse(window.atob(str));
    } catch (err) {
        return null;
    }
}

/*
 * Hex file
 */
function hexstr2(b)
{
    return ('0'+ b.toString(16)).substr(-2).toUpperCase();
}

function hex_line(address, record_type, data)
{
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
}

function hex_eof()
{
    return ":00000001FF\r\n";
}

function hex_output(address, data) {
    var output = '';
    var line = [];

    // TODO: refine: flatten data into one dimension array
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
}



/*
 * Source file
 */
function source_output(keymaps) {
    var output = '';
    // fn actions
    output += "/*\n";
    output += " * Keymap for " + KEYBOARD_ID + "\n";;
    output += " *   generated by tmk keymap editor\n";
    output += " */\n";
    output += "#include <stdint.h>\n";
    output += "#include <stdbool.h>\n";
    output += "#include <avr/pgmspace.h>\n";
    output += "#include \"keycode.h\"\n";
    output += "#include \"action.h\"\n";
    output += "#include \"action_macro.h\"\n";
    output += "#include \"keymap.h\"\n\n";

    output += "#ifdef KEYMAP_SECTION_ENABLE\n";
    output += "const uint16_t fn_actions[] __attribute__ ((section (\".keymap.fn_actions\"))) = {\n";
    output += "#else\n";
    output += "static const uint16_t fn_actions[] PROGMEM = {\n";
    output += "#endif\n";
    output += fn_actions_source;
    output += "};\n\n";

    // keymaps
    output += "#ifdef KEYMAP_SECTION_ENABLE\n";
    output += "const uint8_t keymaps[][";
    output += keymaps[0].length;         // row
    output += "][";
    output += keymaps[0][0].length;      // col
    output += "] __attribute__ ((section (\".keymap.keymaps\"))) = {\n";
    output += "#else\n";
    output += "static const uint8_t keymaps[][";
    output += keymaps[0].length;         // row
    output += "][";
    output += keymaps[0][0].length;      // col
    output += "] PROGMEM = {\n";
    output += "#endif\n";
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



/*
 * keycodes
 */
var keycodes = [
    // {id, name(text), description(tooltip)}
    {id: 'NO  ',                        name: 'NO',                          desc: 'No action'},
    {id: 'TRNS',                        name: 'TRNS',                        desc: 'Transparent'},
    {id: 'POST_FAIL',                   name: 'POST_FAIL',                   desc: 'POST_FAIL'},
    {id: 'UNDEFINED',                   name: 'UNDEFINED',                   desc: 'UNDEFINED'},
    {id: 'A',                           name: 'A',                           desc: 'A'},
    {id: 'B',                           name: 'B',                           desc: 'B'},
    {id: 'C',                           name: 'C',                           desc: 'C'},
    {id: 'D',                           name: 'D',                           desc: 'D'},
    {id: 'E',                           name: 'E',                           desc: 'E'},
    {id: 'F',                           name: 'F',                           desc: 'F'},
    {id: 'G',                           name: 'G',                           desc: 'G'},
    {id: 'H',                           name: 'H',                           desc: 'H'},
    {id: 'I',                           name: 'I',                           desc: 'I'},
    {id: 'J',                           name: 'J',                           desc: 'J'},
    {id: 'K',                           name: 'K',                           desc: 'K'},
    {id: 'L',                           name: 'L',                           desc: 'L'},
    {id: 'M',                           name: 'M',                           desc: 'M'},
    {id: 'N',                           name: 'N',                           desc: 'N'},
    {id: 'O',                           name: 'O',                           desc: 'O'},
    {id: 'P',                           name: 'P',                           desc: 'P'},
    {id: 'Q',                           name: 'Q',                           desc: 'Q'},
    {id: 'R',                           name: 'R',                           desc: 'R'},
    {id: 'S',                           name: 'S',                           desc: 'S'},
    {id: 'T',                           name: 'T',                           desc: 'T'},
    {id: 'U',                           name: 'U',                           desc: 'U'},
    {id: 'V',                           name: 'V',                           desc: 'V'},
    {id: 'W',                           name: 'W',                           desc: 'W'},
    {id: 'X',                           name: 'X',                           desc: 'X'},
    {id: 'Y',                           name: 'Y',                           desc: 'Y'},
    {id: 'Z',                           name: 'Z',                           desc: 'Z'},
    {id: '1',                           name: '1',                           desc: '1'},
    {id: '2',                           name: '2',                           desc: '2'},
    {id: '3',                           name: '3',                           desc: '3'},
    {id: '4',                           name: '4',                           desc: '4'},
    {id: '5',                           name: '5',                           desc: '5'},
    {id: '6',                           name: '6',                           desc: '6'},
    {id: '7',                           name: '7',                           desc: '7'},
    {id: '8',                           name: '8',                           desc: '8'},
    {id: '9',                           name: '9',                           desc: '9'},
    {id: '0',                           name: '0',                           desc: '0'},
    {id: 'ENT',                         name: 'Enter',                       desc: 'ENTER'},
    {id: 'ESC',                         name: 'Esc',                         desc: 'Escape'},
    {id: 'BSPC',                        name: 'Back space',                  desc: 'Backspace'},
    {id: 'TAB',                         name: 'Tab',                         desc: 'Tab'},
    {id: 'SPC',                         name: 'Space',                       desc: 'Space'},
    {id: 'MINS',                        name: '-',                           desc: 'MINUS'},
    {id: 'EQL',                         name: '=',                           desc: 'EQUAL'},
    {id: 'LBRC',                        name: '[',                           desc: 'Left Bracket'},
    {id: 'RBRC',                        name: ']',                           desc: 'Right Bracket'},
    {id: 'BSLS',                        name: "\\",                          desc: 'Backslash'},
    {id: 'NUHS',                        name: 'ISO #',                       desc: 'Non-US Hash'},
    {id: 'SCLN',                        name: ';',                           desc: 'Semicolon'},
    {id: 'QUOT',                        name: "'",                           desc: 'Quote'},
    {id: 'GRV ',                        name: '`',                           desc: 'Grave'},
    {id: 'COMM',                        name: ',',                           desc: 'Comma'},
    {id: 'DOT ',                        name: '.',                           desc: 'Dot'},
    {id: 'SLSH',                        name: '/',                           desc: 'Slash'},
    {id: 'CAPS',                        name: 'Caps Lock',                   desc: 'Need this? Sure? :)'},
    {id: 'F1  ',                        name: 'F1',                          desc: 'F1'},
    {id: 'F2  ',                        name: 'F2',                          desc: 'F2'},
    {id: 'F3  ',                        name: 'F3',                          desc: 'F3'},
    {id: 'F4  ',                        name: 'F4',                          desc: 'F4'},
    {id: 'F5  ',                        name: 'F5',                          desc: 'F5'},
    {id: 'F6  ',                        name: 'F6',                          desc: 'F6'},
    {id: 'F7  ',                        name: 'F7',                          desc: 'F7'},
    {id: 'F8  ',                        name: 'F8',                          desc: 'F8'},
    {id: 'F9  ',                        name: 'F9',                          desc: 'F9'},
    {id: 'F10 ',                        name: 'F10',                         desc: 'F10'},
    {id: 'F11 ',                        name: 'F11',                         desc: 'F11'},
    {id: 'F12 ',                        name: 'F12',                         desc: 'F12'},
    {id: 'PSCR',                        name: 'Print Screen',                desc: 'Print Screen'},
    {id: 'SLCK',                        name: 'Scroll Lock',                 desc: 'Scroll Lock'},
    {id: 'PAUS',                        name: 'Pause',                       desc: 'Pause'},
    {id: 'INS ',                        name: 'Insert',                      desc: 'Insert'},
    {id: 'HOME',                        name: 'Home',                        desc: 'Home'},
    {id: 'PGUP',                        name: 'Page Up',                     desc: 'Page Up'},
    {id: 'DEL ',                        name: 'Delete',                      desc: 'Delete'},
    {id: 'END ',                        name: 'End',                         desc: 'End'},
    {id: 'PGDN',                        name: 'Page Down',                   desc: 'Page Down'},
    {id: 'RGHT',                        name: '\u2192',                      desc: 'Right'},
    {id: 'LEFT',                        name: '\u2190',                      desc: 'Left'},
    {id: 'DOWN',                        name: '\u2193',                      desc: 'Down'},
    {id: 'UP  ',                        name: '\u2191',                      desc: 'Up'},
    {id: 'NLCK',                        name: 'Num Lock',                    desc: 'Num Lock'},
    {id: 'PSLS',                        name: 'P/',                          desc: 'Keypad Slash'},
    {id: 'PAST',                        name: 'P*',                          desc: 'Keypad Asterisk'},
    {id: 'PMNS',                        name: 'P-',                          desc: 'Keypad Minus'},
    {id: 'PPLS',                        name: 'P+',                          desc: 'Keypad Plus'},
    {id: 'PENT',                        name: 'P\u21A9',                     desc: 'Keypad Enter'},
    {id: 'P1  ',                        name: 'P1',                          desc: 'Keypad 1'},
    {id: 'P2  ',                        name: 'P2',                          desc: 'Keypad 2'},
    {id: 'P3  ',                        name: 'P3',                          desc: 'Keypad 3'},
    {id: 'P4  ',                        name: 'P4',                          desc: 'Keypad 4'},
    {id: 'P5  ',                        name: 'P5',                          desc: 'Keypad 5'},
    {id: 'P6  ',                        name: 'P6',                          desc: 'Keypad 6'},
    {id: 'P7  ',                        name: 'P7',                          desc: 'Keypad 7'},
    {id: 'P8  ',                        name: 'P8',                          desc: 'Keypad 8'},
    {id: 'P9  ',                        name: 'P9',                          desc: 'Keypad 9'},
    {id: 'P0  ',                        name: 'P0',                          desc: 'Keypad 0'},
    {id: 'PDOT',                        name: 'P.',                          desc: 'Keypad Dot'},
    {id: 'NUBS',                        name: 'ISO \\',                      desc: 'Non-US Backslash'},
    {id: 'APP ',                        name: 'Application',                 desc: 'Application'},
    {id: 'POWER',                       name: '_Power',                      desc: 'Power(Not work on Windows)'},
    {id: 'PEQL',                        name: 'P=',                          desc: 'Keymapd Equal'},
    {id: 'F13 ',                        name: 'F13',                         desc: 'F13'},
    {id: 'F14 ',                        name: 'F14',                         desc: 'F14'},
    {id: 'F15 ',                        name: 'F15',                         desc: 'F15'},
    {id: 'F16 ',                        name: 'F16',                         desc: 'F16'},
    {id: 'F17 ',                        name: 'F17',                         desc: 'F17'},
    {id: 'F18 ',                        name: 'F18',                         desc: 'F18'},
    {id: 'F19 ',                        name: 'F19',                         desc: 'F19'},
    {id: 'F20 ',                        name: 'F20',                         desc: 'F20'},
    {id: 'F21 ',                        name: 'F21',                         desc: 'F21'},
    {id: 'F22 ',                        name: 'F22',                         desc: 'F22'},
    {id: 'F23 ',                        name: 'F23',                         desc: 'F23'},
    {id: 'F24 ',                        name: 'F24',                         desc: 'F24'},
    {id: 'EXECUTE',                     name: 'EXECUTE',                     desc: 'EXECUTE'},
    {id: 'HELP',                        name: 'HELP',                        desc: 'HELP'},
    {id: 'MENU',                        name: 'MENU',                        desc: 'MENU'},
    {id: 'SELECT',                      name: 'SELECT',                      desc: 'SELECT'},
    {id: 'STOP',                        name: 'STOP',                        desc: 'STOP'},
    {id: 'AGAIN',                       name: 'AGAIN',                       desc: 'AGAIN'},
    {id: 'UNDO',                        name: 'UNDO',                        desc: 'UNDO'},
    {id: 'CUT',                         name: 'CUT',                         desc: 'CUT'},
    {id: 'COPY',                        name: 'COPY',                        desc: 'COPY'},
    {id: 'PASTE',                       name: 'PASTE',                       desc: 'PASTE'},
    {id: 'FIND',                        name: 'FIND',                        desc: 'FIND'},
    {id: '_MUTE',                       name: '_MUTE',                       desc: '_MUTE(Not work on Windows)'},
    {id: '_VOLUP',                      name: '_VOLUP',                      desc: '_VOLUP(Not work on Windows)'},
    {id: '_VOLDOWN',                    name: '_VOLDOWN',                    desc: '_VOLDOWN(Not work on Windows)'},
    {id: 'LCAP',                        name: 'Locking Caps Lock',           desc: 'Locking Caps Lock'},
    {id: 'LNUM',                        name: 'Locking Num Lock',            desc: 'Locking Num Lock'},
    {id: 'LSCR',                        name: 'Locking Scroll Lock',         desc: 'Locking Scroll Lock'},
    {id: 'PCMM',                        name: 'P,',                          desc: 'Keypad Comma'},
    {id: 'KP_EQUAL_AS400',              name: 'P= (AS400)',                  desc: 'Keypad Equal (AS400)'},
    {id: 'INT1',                        name: '\u308D',                      desc: 'Japanese RO'},
    {id: 'INT2',                        name: '\u3072\u3089\u304c\u306a',    desc: 'Japanese Hiragana'},
    {id: 'INT3',                        name: '\uffe5',                      desc: 'Japanese Yen'},
    {id: 'INT4',                        name: '\u5909\u63db',                desc: 'Japanese Henkan'},
    {id: 'INT5',                        name: '\u7121\u5909\u63db',          desc: 'Japanese Muhenkan'},
    {id: 'INT6',                        name: 'INT6',                        desc: 'INT6'},
    {id: 'INT7',                        name: 'INT7',                        desc: 'INT7'},
    {id: 'INT8',                        name: 'INT8',                        desc: 'INT8'},
    {id: 'INT9',                        name: 'INT9',                        desc: 'INT9'},
    {id: 'LANG1',                       name: '\ud55c/\uc601',                       desc: 'Korean Hangul/English'},
    {id: 'LANG2',                       name: '\ud55c\uc790',                desc: 'Korean Hanja'},
    {id: 'LANG3',                       name: 'LANG3',                       desc: 'LANG3'},
    {id: 'LANG4',                       name: 'LANG4',                       desc: 'LANG4'},
    {id: 'LANG5',                       name: 'LANG5',                       desc: 'LANG5'},
    {id: 'LANG6',                       name: 'LANG6',                       desc: 'LANG6'},
    {id: 'LANG7',                       name: 'LANG7',                       desc: 'LANG7'},
    {id: 'LANG8',                       name: 'LANG8',                       desc: 'LANG8'},
    {id: 'LANG9',                       name: 'LANG9',                       desc: 'LANG9'},
    {id: 'ALT_ERASE',                   name: 'ALT_ERASE',                   desc: 'ALT_ERASE'},
    {id: 'SYSREQ',                      name: 'SYSREQ',                      desc: 'SYSREQ'},
    {id: 'CANCEL',                      name: 'CANCEL',                      desc: 'CANCEL'},
    {id: 'CLEAR',                       name: 'CLEAR',                       desc: 'CLEAR'},
    {id: 'PRIOR',                       name: 'PRIOR',                       desc: 'PRIOR'},
    {id: 'RETURN',                      name: 'RETURN',                      desc: 'RETURN'},
    {id: 'SEPARATOR',                   name: 'SEPARATOR',                   desc: 'SEPARATOR'},
    {id: 'OUT',                         name: 'OUT',                         desc: 'OUT'},
    {id: 'OPER',                        name: 'OPER',                        desc: 'OPER'},
    {id: 'CLEAR_AGAIN',                 name: 'CLEAR_AGAIN',                 desc: 'CLEAR_AGAIN'},
    {id: 'CRSEL',                       name: 'CRSEL',                       desc: 'CRSEL'},
    {id: 'EXSEL',                       name: 'EXSEL',                       desc: 'EXSEL'},

    /* System & Media key 165-191(0xa5-bf) */
    {id: 'PWR ',                        name: 'Sys Power',                   desc: 'System Power'},
    {id: 'SLEP',                        name: 'Sys Sleep',                   desc: 'System Sleep'},
    {id: 'WAKE',                        name: 'Sys Wake',                    desc: 'System Wake'},
    {id: 'MUTE',                        name: 'Mute',                        desc: 'Audio Mute'},
    {id: 'VOLU',                        name: 'Vol Up',                      desc: 'Audio Vol Up'},
    {id: 'VOLD',                        name: 'Vol Down',                    desc: 'Audio Vol Down'},
    {id: 'MNXT',                        name: 'Next Track',                  desc: 'Next Track'},
    {id: 'MPRV',                        name: 'Prev Track',                  desc: 'Previous Track'},
    {id: 'MSTP',                        name: 'Stop',                        desc: 'Media Stop'},
    {id: 'MPLY',                        name: 'Play Pause',                  desc: 'Play Pause'},
    {id: 'MSEL',                        name: 'Select',                      desc: 'Media Select'},
    {id: 'EJCT',                        name: 'Eject',                       desc: 'Media Eject'},
    {id: 'MAIL',                        name: 'Mail',                        desc: 'Mail'},
    {id: 'CALC',                        name: 'Calc',                        desc: 'Calculator'},
    {id: 'MYCM',                        name: 'My Computer',                 desc: 'My Computer'},
    {id: 'WSCH',                        name: 'Web Search',                  desc: 'WWW Search'},
    {id: 'WHOM',                        name: 'Web Home',                    desc: 'WWW Home'},
    {id: 'WBAK',                        name: 'Web Back',                    desc: 'WWW Back'},
    {id: 'WFWD',                        name: 'Web Forward',                 desc: 'WWW Forward'},
    {id: 'WSTP',                        name: 'Web Stop',                    desc: 'WWW Stop'},
    {id: 'WREF',                        name: 'Web Refresh',                 desc: 'WWW Refresh'},
    {id: 'WFAV',                        name: 'Web Favorites',               desc: 'WWW Favorites'},
    {id: 'RESERVED-187',                name: 'RESERVED-187',                desc: 'RESERVED-187'},
    {id: 'RESERVED-188',                name: 'RESERVED-188',                desc: 'RESERVED-188'},
    {id: 'RESERVED-189',                name: 'RESERVED-189',                desc: 'RESERVED-189'},
    {id: 'RESERVED-190',                name: 'RESERVED-190',                desc: 'RESERVED-190'},
    {id: 'RESERVED-191',                name: 'RESERVED-191',                desc: 'RESERVED-191'},

    /* Fn key 192-223(0xc0-df) */
    {id: 'FN0 ',                        name: 'L1',                          desc: 'Change to Layer 1(Momentary) '},
    {id: 'FN1 ',                        name: 'L2',                          desc: 'Change to Layer 2(Momentary) '},
    {id: 'FN2 ',                        name: 'L3',                          desc: 'Change to Layer 3(Momentary) '},
    {id: 'FN3 ',                        name: 'L4',                          desc: 'Change to Layer 4(Momentary) '},
    {id: 'FN4 ',                        name: 'L5',                          desc: 'Change to Layer 5(Momentary) '},
    {id: 'FN5 ',                        name: 'L6',                          desc: 'Change to Layer 6(Momentary) '},
    {id: 'FN6 ',                        name: 'L7',                          desc: 'Change to Layer 7(Momentary) '},
    {id: 'FN7 ',                        name: 'T1',                          desc: 'Change to Layer 1(Toggle) '},
    {id: 'FN8 ',                        name: 'T2',                          desc: 'Change to Layer 2(Toggle) '},
    {id: 'FN9 ',                        name: 'T3',                          desc: 'Change to Layer 3(Toggle) '},
    {id: 'FN10',                        name: 'L1t',                         desc: 'Change to Layer 1(Momentary with Tap Toggle) '},
    {id: 'FN11',                        name: 'L2t',                         desc: 'Change to Layer 2(Momentary with Tap Toggle) '},
    {id: 'FN12',                        name: 'L3t',                         desc: 'Change to Layer 3(Momentary with Tap Toggle) '},
    /* Layer swith with Tap key */
    {id: 'FN13',                        name: '\\ (L1)',                     desc: '\\ with with L1(Tap key)'},
    {id: 'FN14',                        name: 'Tab (L2)',                    desc: 'Tab with with L2(Tap key)'},
    {id: 'FN15',                        name: 'Enter (L3)',                  desc: 'Enter with with L3(Tap key)'},
    {id: 'FN16',                        name: 'Space (L4)',                  desc: 'Space with L4(Tap key)'},
    {id: 'FN17',                        name: '; (L5)',                      desc: 'Semicolon with L5(Tap key)'},
    {id: 'FN18',                        name: '\'(L6)',                      desc: 'Quote with L6(Tap key)'},
    {id: 'FN19',                        name: '/ (L7)',                      desc: 'Slash with with L7(Tap key)'},
    /* Modifier on alpha key(Tap key, Dual-role key) */
    {id: 'FN20',                        name: 'Space (LShift)',              desc: 'Space with Left Sfhit(Tap key)'},
    {id: 'FN21',                        name: 'Space (LCtrl)',               desc: 'Space with Left Control(Tap key)'},
    {id: 'FN22',                        name: '\' (RCtrl)',                  desc: 'Quote with Right Control(Tap key)'},
    {id: 'FN23',                        name: 'Enter (RCtrl)',               desc: 'Enter with Right Control(Tap key)'},
    /* Modifier with a key(Tap key, Dual-role key) */
    {id: 'FN24',                        name: 'LCtrl (Esc)',                 desc: 'Left Control with Escape'},
    {id: 'FN25',                        name: 'LCtrl (Backspace)',           desc: 'Left Control with Backspace'},
    {id: 'FN26',                        name: 'LCtrl (OneShot)',             desc: 'Left Control(OneShot Modifier)'},
    {id: 'FN27',                        name: 'LShift (Esc)',                desc: 'Left Shift with Escape'},
    {id: 'FN28',                        name: 'LShift (Backspace)',          desc: 'Left Shift with Backspace'},
    {id: 'FN29',                        name: 'LShift (OneShot)',            desc: 'Left Shift(OneShot Modifier)'},
    {id: 'FN30',                        name: 'RShift (`)',                  desc: 'Right Shift with Grave(Tap key)'},
    {id: 'FN31',                        name: 'RShift (\\)',                 desc: 'Right Shift with Backslash(Tap key)'},

/* Standeard codes for 16bit Action 165-223(0xa5-df)
    {id: 'RESERVED-165',                name: 'RESERVED-165',                desc: 'RESERVED-165'},
    {id: 'RESERVED-166',                name: 'RESERVED-166',                desc: 'RESERVED-166'},
    {id: 'RESERVED-167',                name: 'RESERVED-167',                desc: 'RESERVED-167'},
    {id: 'RESERVED-168',                name: 'RESERVED-168',                desc: 'RESERVED-168'},
    {id: 'RESERVED-169',                name: 'RESERVED-169',                desc: 'RESERVED-169'},
    {id: 'RESERVED-170',                name: 'RESERVED-170',                desc: 'RESERVED-170'},
    {id: 'RESERVED-171',                name: 'RESERVED-171',                desc: 'RESERVED-171'},
    {id: 'RESERVED-172',                name: 'RESERVED-172',                desc: 'RESERVED-172'},
    {id: 'RESERVED-173',                name: 'RESERVED-173',                desc: 'RESERVED-173'},
    {id: 'RESERVED-174',                name: 'RESERVED-174',                desc: 'RESERVED-174'},
    {id: 'RESERVED-175',                name: 'RESERVED-175',                desc: 'RESERVED-175'},
    {id: 'KP_00',                       name: 'KP_00',                       desc: 'KP_00'},
    {id: 'KP_000',                      name: 'KP_000',                      desc: 'KP_000'},
    {id: 'THOUSANDS_SEPARATOR',         name: 'THOUSANDS_SEPARATOR',         desc: 'THOUSANDS_SEPARATOR'},
    {id: 'DECIMAL_SEPARATOR',           name: 'DECIMAL_SEPARATOR',           desc: 'DECIMAL_SEPARATOR'},
    {id: 'CURRENCY_UNIT',               name: 'CURRENCY_UNIT',               desc: 'CURRENCY_UNIT'},
    {id: 'CURRENCY_SUB_UNIT',           name: 'CURRENCY_SUB_UNIT',           desc: 'CURRENCY_SUB_UNIT'},
    {id: 'KP_LPAREN',                   name: 'KP_LPAREN',                   desc: 'KP_LPAREN'},
    {id: 'KP_RPAREN',                   name: 'KP_RPAREN',                   desc: 'KP_RPAREN'},
    {id: 'KP_LCBRACKET',                name: 'KP_LCBRACKET',                desc: 'KP_LCBRACKET'},
    {id: 'KP_RCBRACKET',                name: 'KP_RCBRACKET',                desc: 'KP_RCBRACKET'},
    {id: 'KP_TAB',                      name: 'KP_TAB',                      desc: 'KP_TAB'},
    {id: 'KP_BSPACE',                   name: 'KP_BSPACE',                   desc: 'KP_BSPACE'},
    {id: 'KP_A',                        name: 'KP_A',                        desc: 'KP_A'},
    {id: 'KP_B',                        name: 'KP_B',                        desc: 'KP_B'},
    {id: 'KP_C',                        name: 'KP_C',                        desc: 'KP_C'},
    {id: 'KP_D',                        name: 'KP_D',                        desc: 'KP_D'},
    {id: 'KP_E',                        name: 'KP_E',                        desc: 'KP_E'},
    {id: 'KP_F',                        name: 'KP_F',                        desc: 'KP_F'},
    {id: 'KP_XOR',                      name: 'KP_XOR',                      desc: 'KP_XOR'},
    {id: 'KP_HAT',                      name: 'KP_HAT',                      desc: 'KP_HAT'},
    {id: 'KP_PERC',                     name: 'KP_PERC',                     desc: 'KP_PERC'},
    {id: 'KP_LT',                       name: 'KP_LT',                       desc: 'KP_LT'},
    {id: 'KP_GT',                       name: 'KP_GT',                       desc: 'KP_GT'},
    {id: 'KP_AND',                      name: 'KP_AND',                      desc: 'KP_AND'},
    {id: 'KP_LAZYAND',                  name: 'KP_LAZYAND',                  desc: 'KP_LAZYAND'},
    {id: 'KP_OR',                       name: 'KP_OR',                       desc: 'KP_OR'},
    {id: 'KP_LAZYOR',                   name: 'KP_LAZYOR',                   desc: 'KP_LAZYOR'},
    {id: 'KP_COLON',                    name: 'KP_COLON',                    desc: 'KP_COLON'},
    {id: 'KP_HASH',                     name: 'KP_HASH',                     desc: 'KP_HASH'},
    {id: 'KP_SPACE',                    name: 'KP_SPACE',                    desc: 'KP_SPACE'},
    {id: 'KP_ATMARK',                   name: 'KP_ATMARK',                   desc: 'KP_ATMARK'},
    {id: 'KP_EXCLAMATION',              name: 'KP_EXCLAMATION',              desc: 'KP_EXCLAMATION'},
    {id: 'KP_MEM_STORE',                name: 'KP_MEM_STORE',                desc: 'KP_MEM_STORE'},
    {id: 'KP_MEM_RECALL',               name: 'KP_MEM_RECALL',               desc: 'KP_MEM_RECALL'},
    {id: 'KP_MEM_CLEAR',                name: 'KP_MEM_CLEAR',                desc: 'KP_MEM_CLEAR'},
    {id: 'KP_MEM_ADD',                  name: 'KP_MEM_ADD',                  desc: 'KP_MEM_ADD'},
    {id: 'KP_MEM_SUB',                  name: 'KP_MEM_SUB',                  desc: 'KP_MEM_SUB'},
    {id: 'KP_MEM_MUL',                  name: 'KP_MEM_MUL',                  desc: 'KP_MEM_MUL'},
    {id: 'KP_MEM_DIV',                  name: 'KP_MEM_DIV',                  desc: 'KP_MEM_DIV'},
    {id: 'KP_PLUS_MINUS',               name: 'KP_PLUS_MINUS',               desc: 'KP_PLUS_MINUS'},
    {id: 'KP_CLEAR',                    name: 'KP_CLEAR',                    desc: 'KP_CLEAR'},
    {id: 'KP_CLEAR_ENTRY',              name: 'KP_CLEAR_ENTRY',              desc: 'KP_CLEAR_ENTRY'},
    {id: 'KP_BINARY',                   name: 'KP_BINARY',                   desc: 'KP_BINARY'},
    {id: 'KP_OCTAL',                    name: 'KP_OCTAL',                    desc: 'KP_OCTAL'},
    {id: 'KP_DECIMAL',                  name: 'KP_DECIMAL',                  desc: 'KP_DECIMAL'},
    {id: 'KP_HEXADECIMAL',              name: 'KP_HEXADECIMAL',              desc: 'KP_HEXADECIMAL'},
    {id: 'RESERVED-222',                name: 'RESERVED-222',                desc: 'RESERVED-222'},
    {id: 'RESERVED-223',                name: 'RESERVED-223',                desc: 'RESERVED-223'},
*/

    /* Modifier 224-231(0xe0-e7) */
    {id: 'LCTL',                        name: 'LCtrl',                       desc: 'Left Control'},
    {id: 'LSFT',                        name: 'LShift',                      desc: 'Left Shift'},
    {id: 'LALT',                        name: 'LAlt',                        desc: 'Left Alt(\u2325)'},
    {id: 'LGUI',                        name: 'LGui',                        desc: 'Left Windows(\u2318)'},
    {id: 'RCTL',                        name: 'RCtrl',                       desc: 'Right Control'},
    {id: 'RSFT',                        name: 'RShift',                      desc: 'Right Shift'},
    {id: 'RALT',                        name: 'RAlt',                        desc: 'Right Alt(\u2325)'},
    {id: 'RGUI',                        name: 'RGui',                        desc: 'Right Windows(\u2318)'},

    /* Not used 232-239(0xe8-ef) */
    {id: 'RESERVED-232',                name: 'RESERVED-232',                desc: 'RESERVED-232'},
    {id: 'RESERVED-233',                name: 'RESERVED-233',                desc: 'RESERVED-233'},
    {id: 'RESERVED-234',                name: 'RESERVED-234',                desc: 'RESERVED-234'},
    {id: 'RESERVED-235',                name: 'RESERVED-235',                desc: 'RESERVED-235'},
    {id: 'RESERVED-236',                name: 'RESERVED-236',                desc: 'RESERVED-236'},
    {id: 'RESERVED-237',                name: 'RESERVED-237',                desc: 'RESERVED-237'},
    {id: 'RESERVED-238',                name: 'RESERVED-238',                desc: 'RESERVED-238'},
    {id: 'RESERVED-239',                name: 'RESERVED-239',                desc: 'RESERVED-239'},

    /* Mousekey 240-255(0xf0-ff) */
    {id: 'MS_U',                        name: 'Mouse Up',                    desc: 'Mouse UP'},
    {id: 'MS_D',                        name: 'Mouse down',                  desc: 'Mouse Down'},
    {id: 'MS_L',                        name: 'Mouse left',                  desc: 'Mouse Left'},
    {id: 'MS_R',                        name: 'Mouse right',                 desc: 'Mouse Right'},
    {id: 'BTN1',                        name: 'Mouse Btn1',                  desc: 'Mouse Button1'},
    {id: 'BTN2',                        name: 'Mouse Btn2',                  desc: 'Mouse Button2'},
    {id: 'BTN3',                        name: 'Mouse Btn3',                  desc: 'Mouse Button3'},
    {id: 'BTN4',                        name: 'Mouse Btn4',                  desc: 'Mouse Button4'},
    {id: 'BTN5',                        name: 'Mouse Btn5',                  desc: 'Mouse Button5'},
    {id: 'WH_U',                        name: 'Wheel Up',                    desc: 'Wheel Up'},
    {id: 'WH_D',                        name: 'Wheel Down',                  desc: 'Wheel Down'},
    {id: 'WH_L',                        name: 'Wheel Left',                  desc: 'Wheel Left'},
    {id: 'WH_R',                        name: 'Wheel Right',                 desc: 'Wheel Right'},
    {id: 'ACL0',                        name: 'Mouse Slow',                  desc: 'Mouse Slow'},
    {id: 'ACL1',                        name: 'Mouse Medium',                desc: 'Mouse Medium'},
    {id: 'ACL2',                        name: 'Mouse Fast',                  desc: 'Mouse Fast'},
];
