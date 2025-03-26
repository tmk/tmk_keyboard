/*
 * TMK keymap editor - unimap/actionmap
 */
import * as AtmelDFU from './AtmelDFU.js';

$(function() {
    // Key button id under editing
    var editing_key;
    // Layer under editing
    var editing_layer = 0;
    var keymaps_changed = false;

    // keyboard variant from URL search(?...)
    let variant = document.location.search.substring(1);

    // keyboard layout
    let layout = CONFIG.layout_default;
    if (CONFIG.keymap[variant] && CONFIG.keymap[variant].layout) {
        layout = CONFIG.keymap[variant].layout;
    }

    // load keymap from URL hash(#...)
    var decoded = url_decode_keymap(document.location.hash.substring(1));
    if (decoded != null) {
        keymaps = decoded['keymaps'];
    }


    /**********************************************************************
     * General Setup
     **********************************************************************/
    // jquery tooltip
    $( document ).tooltip();

    // Title
    document.title = "TMK Keymap Editor";
    $("#page-title").text("TMK Keymap Editor");


    /**********************************************************************
     * Layer Selector
     **********************************************************************/
    $("#layer_radio").buttonset();

    // layer change
    $(".layer").on("click", function(ev, ui) {
        var layer = parseInt($(this).attr('id').match(/layer-(\d+)/)[1]);
        editing_layer = layer;
        load_keymap_on_keyboard(keymaps[layer]);
    });

    var load_keymap_on_keyboard = function(keymap) {
        for (var row in keymap) {
            for (var col in keymap[row]) {
                var code = keymap[row][col];
                var act = new Action(code);
                $("*#key-" + parseInt(row).toString(32) + parseInt(col).toString(32))
                    .text(act.name)
                    .attr({ title: act.desc });
            }
        }
    };


    /**********************************************************************
     * Action Codes
     **********************************************************************/
    $("#keycode_tabs").tabs({
        heightStyle: "auto",
    });

    // read name and description from code table
    $(".action").each(function(index) {
        // get code from code button id: code-[0x]CCCC where CCCC is dec or hex number
        var code = parseInt($(this).attr('id').match(/code-((0x){0,1}[0-9a-fA-F]+)/)[1]);
        var act = new Action(code);
        $(this).text(act.name);
        $(this).attr({ title: act.desc });
    });

    $(".action").on("click", function(ev,ui) {
        // get code from code button id: code-[0x]CCCC where CCCC is dec or hex number
        var code = parseInt($(this).attr('id').match(/code-((0x){0,1}[0-9a-fA-F]+)/)[1]);
        action_editor_set_code(code);

        if (!editing_key) return;
        $(this).blur();
        editing_key_set(code);
    });

    var get_pos = function(id) {
        // get matrix position from key id: key-RC where R is row and C is column in "0-v"(radix 32)
        var pos = editing_key.match(/key-([0-9a-v])([0-9a-v])/i);
        if (!pos) throw "invalid id";
        return { row: parseInt(pos[1], 32), col: parseInt(pos[2], 32) };
    };

    var editing_key_set = function(code) {
        var pos = get_pos(editing_key);
        keymaps[editing_layer][pos.row][pos.col] = code;
        keymaps_changed = true;

        // set text and tooltip to key button under editing
        var act = new Action(code);
        $("*#" + editing_key).text(act.name);
        $("*#" + editing_key).attr({ title: act.desc });

        // to give back focus on editing_key for moving to next key with Tab
        $("*#" + editing_key).focus();
    };


    /**********************************************************************
     * Action Code Editor
     **********************************************************************/
    $(".editor_dropdown").hide();
    for (var kind in action_kinds) {
        $("#kind_dropdown").append($("<option></option>")
                .attr({ value: action_kinds[kind].id, title: action_kinds[kind].desc })
                .text(action_kinds[kind].name));
    }
    for (var code in keycodes) {
        $("#keycodes_dropdown").append($("<option></option>")
                .attr({ value: code, title: keycodes[code].desc })
                .text(keycodes[code].name));
    }
    for (var code in system_codes) {
        $("#system_codes_dropdown").append($("<option></option>")
                .attr({ value: code, title: system_codes[code].desc })
                .text(system_codes[code].name));
    }
    for (var code in consumer_codes) {
        $("#consumer_codes_dropdown").append($("<option></option>")
                .attr({ value: code, title: consumer_codes[code].desc })
                .text(consumer_codes[code].name));
    }
    for (var code in mousekey_codes) {
        $("#mousekey_codes_dropdown").append($("<option></option>")
                .attr({ value: code, title: mousekey_codes[code].desc })
                .text(mousekey_codes[code].name));
    }
    for (var i = 0; i < CONFIG.layer_num; i++) {
        $("#layer_dropdown").append($("<option></option>")
                .attr({ value: i, title: "Layer " + i })
                .text("Layer " + i));
    }
    for (var code in mods_codes) {
        $("#key_mods_dropdown").append($("<option></option>")
                .attr({ value: code, title: mods_codes[code].desc })
                .text(mods_codes[code].name));
        $("#layer_mods_dropdown").append($("<option></option>")
                .attr({ value: code, title: mods_codes[code].desc })
                .text(mods_codes[code].name));
    }
    for (let i = 0; i < 8; i++) {
        $("#layer_part_dropdown").append($("<option></option>")
                .attr({ value: i, title: "(Bits&0xF) << " + i*4 })
                .text("Part " + i));
    }
    for (let i = 0; i < 32; i++) {
        $("#layer_bits_dropdown").append($("<option></option>")
                .attr({ value: i, title: "bits: " + ('000' + i.toString(2)).substr(-4) +
                                         "\nmask: " + ((i & 0x10) ? "~(((uint32_t)0xF)<<(part*4))" : "0") })
                .text("Bits " + ('0000' + i.toString(2)).substr(-5)));
    }
    for (var code in on_codes) {
        $("#layer_on_dropdown").append($("<option></option>")
                .attr({ value: code, title: on_codes[code].desc })
                .text(on_codes[code].name));
    }
    for (var ids in command_ids) {
        $("#command_ids_dropdown").append($("<option></option>")
                .attr({ value: ids, title: command_ids[ids].desc })
                .text(command_ids[ids].name));
    }
    $(window).on("load", function() { action_editor_set_code(0); });

    // set code to editor
    var action_editor_set_code = function(code) {
        var act = new Action(code);
        $("#kind_dropdown").val(act.id);
        $("#kind_dropdown").trigger("change");
        $("#keycodes_dropdown").val(act.key_code);
        $("#key_mods_dropdown").val(act.key_mods);
        $("#system_codes_dropdown").val(act.usage_code);
        $("#consumer_codes_dropdown").val(act.usage_code);
        $("#mousekey_codes_dropdown").val(act.mousekey_code);
        if (act.kind == ACT_LAYER) {
            $("#layer_dropdown").val(act.layer_bitop_layer);
        } else {
            $("#layer_dropdown").val(act.layer_tap_val);
        }
        $("#layer_mods_dropdown").val(act.layer_tap_code & 0x1f);
        $("#layer_part_dropdown").val(act.layer_bitop_part);
        $("#layer_bits_dropdown").val(act.layer_bitop_xbit<<4 | act.layer_bitop_bits);
        $("#layer_on_dropdown").val(act.layer_bitop_on);
        $("#command_ids_dropdown").val(act.command_id);
        $("#code_hex").val(('000' + code.toString(16)).substr(-4).toUpperCase());
    };

    // compile action code from editor
    var action_editor_get_code = function() {
        var action_kind = $("#kind_dropdown").val();
        var keycode = parseInt($("#keycodes_dropdown").val());
        var key_mods = parseInt($("#key_mods_dropdown").val());
        var consumer_code = parseInt($("#consumer_codes_dropdown").val());
        var system_code = parseInt($("#system_codes_dropdown").val());
        var mousekey_code = parseInt($("#mousekey_codes_dropdown").val());
        var layer = parseInt($("#layer_dropdown").val());
        var layer_mods = parseInt($("#layer_mods_dropdown").val());
        let layer_part = parseInt($("#layer_part_dropdown").val());
        let layer_bits = parseInt($("#layer_bits_dropdown").val());
        var layer_on =  parseInt($("#layer_on_dropdown").val());
        var command_id = parseInt($("#command_ids_dropdown").val());
        switch (action_kind) {
            case "KEY":
                return kind_codes[action_kind] | keycode;
            case "MODS_KEY":
                return kind_codes[action_kind] | key_mods<<8 | keycode;
            case "MODS_TAP_KEY":
                return kind_codes[action_kind] | key_mods<<8 | keycode;
            case "MODS_ONESHOT":
                return kind_codes[action_kind] | key_mods<<8;
            case "MODS_TAP_TOGGLE":
                return kind_codes[action_kind] | key_mods<<8;
            case "TAP_KEY_MODS":
                return kind_codes[action_kind] | key_mods<<8 | keycode;

            case "USAGE_SYSTEM":
                return kind_codes[action_kind] | system_code;
            case "USAGE_CONSUMER":
                return kind_codes[action_kind] | consumer_code;
            case "MOUSEKEY":
                return kind_codes[action_kind] | mousekey_code;

            case "LAYER_MOMENTARY":
            case "LAYER_ON_OFF":
            case "LAYER_OFF_ON":
            case "LAYER_SET_CLEAR":
            case "LAYER_TAP_TOGGLE":
                return kind_codes[action_kind] | layer<<8;
            case "LAYER_TAP_KEY":
                return kind_codes[action_kind] | layer<<8 | keycode;
            case "LAYER_MODS":
                return kind_codes[action_kind] | layer<<8 | layer_mods;

            case "LAYER_INVERT":
            case "LAYER_ON":
            case "LAYER_SET":
                return kind_codes[action_kind] | layer_on<<8 | (layer/4)<<5 | 1<<(layer%4);
            case "LAYER_OFF":
                return kind_codes[action_kind] | layer_on<<8 | (layer/4)<<5 | 1<<4 | (~(1<<(layer%4)) & 0xf);
            case "LAYER_TOGGLE":
                return kind_codes[action_kind] | (layer/4)<<5 | 1<<(layer%4);
            case "LAYER_CLEAR":
                return kind_codes[action_kind] | layer_on<<8 | 0<<5 | 0;

            case "LAYER_BIT_AND":
            case "LAYER_BIT_OR":
            case "LAYER_BIT_XOR":
            case "LAYER_BIT_SET":
                return kind_codes[action_kind] | layer_on<<8 | layer_part<<5 | layer_bits;

            case "DEFAULT_LAYER_SET":
                return kind_codes[action_kind] | (layer/4)<<5 | 1<<(layer%4);

            case "COMMAND":
                return kind_codes[action_kind] | command_id;
        };
        return parseInt($("#code_hex").val(), 16);  // UNKNOWN
    };

    // control display of dropdown elements
    $("#kind_dropdown").on("change", function(ev) {
        $(".editor_dropdown").hide();
        $("#kind_dropdown").show();
        switch ($(this).val()) {
            case "KEY":
                $("#keycodes_dropdown").show();
                break;
            case "MODS_KEY":
                $("#keycodes_dropdown").show();
                $("#key_mods_dropdown").show();
                break;
            case "MODS_TAP_KEY":
                $("#key_mods_dropdown").show();
                $("#keycodes_dropdown").show();
                break;
            case "MODS_ONESHOT":
                $("#key_mods_dropdown").show();
                break;
            case "MODS_TAP_TOGGLE":
                $("#key_mods_dropdown").show();
                break;
            case "TAP_KEY_MODS":
                $("#keycodes_dropdown").show();
                $("#key_mods_dropdown").show();
                break;

            case "USAGE_SYSTEM":
                $("#system_codes_dropdown").show();
                break;
            case "USAGE_CONSUMER":
                $("#consumer_codes_dropdown").show();
                break;
            case "MOUSEKEY":
                $("#mousekey_codes_dropdown").show();
                break;

            case "LAYER_MOMENTARY":
            case "LAYER_ON_OFF":
            case "LAYER_OFF_ON":
            case "LAYER_SET_CLEAR":
            case "LAYER_TAP_TOGGLE":
                $("#layer_dropdown").show();
                break;
            case "LAYER_TAP_KEY":
                $("#layer_dropdown").show();
                $("#keycodes_dropdown").show();
                break;
            case "LAYER_MODS":
                $("#layer_dropdown").show();
                $("#layer_mods_dropdown").show();
                break;

            case "LAYER_INVERT":
            case "LAYER_ON":
            case "LAYER_OFF":
            case "LAYER_SET":
                $("#layer_dropdown").show();
                $("#layer_on_dropdown").show();
                break;
            case "LAYER_TOGGLE":
                $("#layer_dropdown").show();
                break;
            case "LAYER_CLEAR":
                $("#layer_on_dropdown").show();
                break;
            case "LAYER_BIT_AND":
            case "LAYER_BIT_OR":
            case "LAYER_BIT_XOR":
            case "LAYER_BIT_SET":
                $("#layer_part_dropdown").show();
                $("#layer_bits_dropdown").show();
                $("#layer_on_dropdown").show();
                break;
            case "DEFAULT_LAYER_SET":
                $("#layer_dropdown").show();
                break;
            case "COMMAND":
                $("#command_ids_dropdown").show();
                break;
        };
    });

    // apply button
    $(".action-apply").on("click", function(ev) {
        if (!editing_key) return;
        var action_code = action_editor_get_code();
        editing_key_set(action_code);
    });


    /**********************************************************************
     * Base firmware setting
     **********************************************************************/
    var firmware_before = [];
    var firmware_after = [];
    var firmware_keymaps = [];

    // Load ihex firmware
    let loadHexURL = function(firmware_url) {
        return $.ajax({
            method: "GET",
            url: firmware_url,
            //async: false,
        }).done(function(s) {
            var lines = hex_split_firmware(s, KEYMAP_START_ADDRESS, KEYMAP_SIZE);
            firmware_before = lines.before;
            firmware_after = lines.after;
            firmware_keymaps = lines.keymaps;
        });
    };

    // Base firmware - Select from config
    for (var prod in CONFIG.keymap) {
        $("#firmware-dropdown").append($("<option></option>")
                .attr({ value: prod, title: CONFIG.keymap[prod].desc })
                .text(CONFIG.keymap[prod].desc));
    }
    $("#firmware-dropdown").on("change", function() {
        let v = $(this).val();
        $("#firmware-flash").prop("disabled", true);
        $("#firmware-download").prop("disabled", true);
        $("#keymap-load").prop("disabled", true);

        if (!CONFIG.keymap[v]) { return; }
        $("#firmware-dropdown").prop("disabled", true);
        loadHexURL(CONFIG.keymap[v].firmware_url).done(function(s) {
            $("#firmware-flash").prop("disabled", false);
            $("#firmware-download").prop("disabled", false);
            $("#keymap-load").prop("disabled", false);
        }).fail(function() {
            $("#firmware-flash").prop("disabled", true);
            $("#firmware-download").prop("disabled", true);
            $("#keymap-load").prop("disabled", true);
        }).always(function() {
            $("#firmware-dropdown").prop("disabled", false);

            if (CONFIG.keymap[v].layout) {
                $("#keymap-desc").text(CONFIG.keymap[v].desc);
                layout_load(CONFIG.keymap[v].layout);
            } else {
                $("#keymap-desc").text("Unknown");
                layout_load(CONFIG.layout_default);
            }
        });
    });

    // Base firmware - File chooser
    $("#firmwareFile").on("change", function(ev) {
        // called after choosing file
        var f = ev.target.files[0];
        if (!f) {
            $("#firmware-flash").prop("disabled", true);
            $("#firmware-download").prop("disabled", true);
            $("#keymap-load").prop("disabled", true);
            return;
        }

        var fr = new FileReader();
        fr.onloadend = function(e) {
            // TODO: support .bin format
            var lines = hex_split_firmware(this.result, KEYMAP_START_ADDRESS, KEYMAP_SIZE);
            firmware_before = lines.before;
            firmware_after = lines.after;
            firmware_keymaps = lines.keymaps;
            $("#firmware-flash").prop("disabled", false);
            $("#firmware-download").prop("disabled", false);
            $("#keymap-load").prop("disabled", false);
        };
        fr.readAsText(f);
    });

    $("#firmwareURL").on("change", function(ev) {
        var firmware_url = $(this).val();
        if (!firmware_url) {
            $("#firmware-flash").prop("disabled", true);
            $("#firmware-download").prop("disabled", true);
            $("#keymap-load").prop("disabled", true);
            return;
        }

        $("#firmwareURL").prop("disabled", true);
        loadHexURL(firmware_url).done(function(s) {
            $("#firmware-flash").prop("disabled", false);
            $("#firmware-download").prop("disabled", false);
            $("#keymap-load").prop("disabled", false);
        }).fail(function(d) {
            $("#firmware-flash").prop("disabled", true);
            $("#firmware-download").prop("disabled", true);
            $("#keymap-load").prop("disabled", true);
        }).always(function() {
            $("#firmwareURL").prop("disabled", false);
        });
    });

    // Load keymap from base firmware
    $("#keymap-load").prop("disabled", true);
    $("#keymap-load").on("click", function(ev, ui) {
        // load keymap from firmware
        keymaps = $.extend(true, [], firmware_keymaps); // copy
        while (keymaps.length < KEYMAP_LAYERS) keymaps.push(transparent_map());
        load_keymap_on_keyboard(keymaps[editing_layer]);
    });

    // Base firmeare - radio button
    $(".base-firm").on("change", function() {
        console.log($(this).val());
        $("#firmware-flash").prop("disabled", true);
        $("#firmware-download").prop("disabled", true);
        $("#keymap-load").prop("disabled", true);
        switch ($(this).val()) {
            case "config":
                $("#firmware-dropdown").prop("disabled", false);
                $("#firmwareURL").prop("disabled", true);
                $("#firmwareFile").prop("disabled", true);
                $("#keymap-load").prop("disabled", true);
                $("#firmware-dropdown").trigger("change");
                break;
            case "url":
                $("#firmware-dropdown").prop("disabled", true);
                $("#firmwareURL").prop("disabled", false);
                $("#firmwareFile").prop("disabled", true);
                $("#keymap-load").prop("disabled", true);
                $("#firmwareURL").trigger("change");
                break;
            case "file":
                $("#firmware-dropdown").prop("disabled", true);
                $("#firmwareURL").prop("disabled", true);
                $("#firmwareFile").prop("disabled", false);
                $("#keymap-load").prop("disabled", true);
                $("#firmwareFile").trigger("change");
                break;
        };
    });
    $(".base-firm:checked").trigger("change");


    /**********************************************************************
     * Download firmware
     **********************************************************************/
    $("#firmware-download").prop("disabled", true);
    $("#firmware-download").on("click", function(ev, ui) {
        // TODO: support .bin format
        if ( $("#firmwareFile")[0].files[0] &&
                $("#firmwareFile")[0].files[0].name.match(/\.hex/)) {
            console.log("hex file");
        }

        var content = [].concat(firmware_before)
                        .concat(hex_keymaps(KEYMAP_START_ADDRESS))
                        .concat(firmware_after).join("\r\n").concat("\r\n");

        // download hex file
        var blob = new Blob([content], {type: "application/octet-stream"});
        // For MicroSoft IE and Edge support
        if (window.navigator.msSaveBlob) {
            window.navigator.msSaveBlob(blob, "unimap.hex");
        } else {
            var hex_link = $("#hex-download");
            hex_link.attr('href', window.URL.createObjectURL(blob));
            hex_link.attr('download', "unimap.hex");
            // jQuery click() doesn't work straight for 'a' element
            // http://stackoverflow.com/questions/1694595/
            hex_link[0].click();
        }
    });


    /**********************************************************************
     * Flash WebUSB
     **********************************************************************/
    $("#flash-console").hide();
    let flash_console = function(msg) {
        $("#flash-console").text($("#flash-console").text() + msg);
    };
    let target = null;
    $("#firmware-flash").on("click", async function(ev, ui) {
        console.log('firmware-flash: start');
        try {
            let result;

            $("#flash-console").show();
            $("#flash-console").text("");

            // Select device
            target = await AtmelDFU.getAtmelDevice();
            if (target === null) {
                flash_console("No device selected\n");
                flash_console("NOTE: Firefox and Safari do not support this function(WebUSB). Use Chrome or Edge.");
                return;
            }
            flash_console("Device: " + target.productName + "\n");
            console.log(target.productName);
            let d = AtmelDFU.deviceInfo.find((e) => e.productId === target.productId);
            if (d === undefined) {
                flash_console('Unsupported Device');
                return;
            }
            flash_console("Flash/Boot Size: " + d.flashSize + "/" + d.bootSize + "\n");

            // Erase Chip
            flash_console('Erasing Flash...');
            result = await AtmelDFU.chipErase(target);
            flash_console('Erased\n');

            // Blank check
            flash_console('Checking Blank...');
            result = await AtmelDFU.blankCheck(target, 0, d.flashSize - 1);
            result = await AtmelDFU.getStatus(target);
            let stat = AtmelDFU.parseStatus(result.data);
            if (stat.bStatus == AtmelDFU.bStatus.OK) {
                flash_console('Blank\n');
            } else if (stat.bStatus === AtmelDFU.bStatus.errCHECK_ERASED) {
                flash_console('Not Blank');
                return;
            } else {
                flash_console('Unknown Error');
                return;
            }

            // Load Firmware
            flash_console('Loading Firmware...');
            let hex_text = [].concat(firmware_before)
                             .concat(hex_keymaps(KEYMAP_START_ADDRESS))
                             .concat(firmware_after).join("\r\n").concat("\r\n");
            let data = parseHex(hex_text);
            flash_console('Data Length: ' + data.length + '\n');

            // Write firmware
            flash_console('Writing Firmware...');
            result = await AtmelDFU.writeMemory(target, 0x0000, data.length - 1, data);
            flash_console('OK\n');

            // Verify
            flash_console('Verifying Content...');
            let mem = await AtmelDFU.readMemory(target, 0, data.length - 1);
            for (let i = 0; i < mem.byteLength; i++) {
                if (mem[i] !== data[i]) {
                    flash_console('Failed at' + i.toString(16));
                    return;
                }
            }
            flash_console('OK\n');

            // Start App
            flash_console('Starting Keyboard...');
            result = await AtmelDFU.launch(target);
            flash_console(result.status);   // 'ok' or 'stall'

            flash_console('\nFlashed firmware successfully.\n');
        } catch(e) {
            target = null;
            flash_console('\nError...\n');
            flash_console(e);
            console.log(e);
        }
        console.log('firmware-flash: done');
    });


    /**********************************************************************
     * Share URL
     **********************************************************************/
    // Share URL
    $("#share-url-display").hide();
    $("#keymap-share").on("click", function(ev, ui) {
        var hash = url_encode_keymap({ keymaps: keymaps });
        var editor_url = document.location.origin + document.location.pathname + document.location.search;
        $("#share-url-display").show();
        $("#share-url").text(editor_url + "#" + hash);
    });

    // Shorten URL
    $("#shorten-url").on("click", function(ev, ui) {
        var hash = url_encode_keymap({ keymaps: keymaps });
        var editor_url = document.location.origin + document.location.pathname + document.location.search;

        // Don't compromise this token :D
        BITLY_TOKEN = "fe1d845c97a9ee8872333eb531b07b64c14829ce";
        $.ajax({
            method: "POST",
            url: "https://api-ssl.bitly.com/v4/shorten",
            contentType: "application/json",
            data: '{ "long_url": "' + editor_url + '#' + hash + '" }',
            headers:  { Authorization: "Bearer " + BITLY_TOKEN }
        }).done(function(d) {
            console.log(d);
            $("#share-url-display").show();
            $("#share-url").text(d.link);
        }).fail(function(d) {
            console.log(d);
            console.log('{ long_url: "' + editor_url + '#' + hash + '" }');
        });
    });


    /**********************************************************************
     * Output options for debug
     **********************************************************************/
    $("#debug-output").hide();
    $("#debug-collapse").on("click", function() {
        if ($("#debug-output").css("display") == "none") {
            $("#debug-collapse").text("\u25b2");
        } else {
            $("#debug-collapse").text("\u25bc");
        }
        $("#debug-output").toggle("slow");
    });
    //$("#keymap-output").resizable();  // resizable textarea

    // Hex output
    $("#keymap-hex-generate").on("click", function(ev, ui) {
        $("#keymap-output").text(hex_keymaps(KEYMAP_START_ADDRESS).join("\r\n"));
    });

    // C source output
    $("#keymap-source-generate").on("click", function(ev, ui) {
        $("#keymap-output").text(source_output(keymaps));
    });

    // JSON output
    //$("#keymap-json-generate").css('display', 'none');  // hide
    $("#keymap-json-generate").on("click", function(ev, ui) {
        var keymap_output;
        //keymap_output = JSON.stringify(keymaps, null, 4);
        keymap_output = JSON.stringify({ keymaps: keymaps });
        $("#keymap-output").text(keymap_output);
    });

    // encode keymap
    $("#keymap-encode").on("click", function(ev, ui) {
        var keymap_output = url_encode_keymap({ keymaps: keymaps });
        $("#keymap-output").text(keymap_output);
    });

    // decode  keymap
    $("#keymap-decode").on("click", function(ev, ui) {
        var hash = $("#keymap-output").text();
        var keymap_output = url_decode_keymap(hash);
        $("#keymap-output").text(JSON.stringify(keymap_output));
    });


    // prevent losing keymap under editing when leave the page
    $(window).on('beforeunload', function(e){
        e.preventDefault();
        e.returnValue = 'CAUTION: You will lost your change.';
        return 'CAUTION: You will lost your change.';
    });


    /**********************************************************************
     * Load keyboard layout
     **********************************************************************/
    let layout_load = function(layout) {
        $("#keyboard-outline").load(layout, function() {
            // executed after loading layout and creating DOM
            /**********************************************************************
             * Keyboard(key buttons)
             **********************************************************************/
            // Select key button to edit
            $(".key").on("focus", function(ev) {
                $(this).click();
            });
            $(".key").on("click", function(ev) {
                editing_key = $(this).attr('id');

                // grey-out key to indicate being under editing
                $(".key").removeClass("key-editing");
                $("*#" + editing_key).addClass("key-editing");
                var pos = get_pos(editing_key);
                var code = keymaps[editing_layer][pos.row][pos.col];

                action_editor_set_code(code);
                $(this).blur();
            });

            load_keymap_on_keyboard(keymaps[editing_layer]);
        });
    };

    /**********************************************************************
     * Load firmware from URL in config
     **********************************************************************/
    if (CONFIG.keymap[variant] && CONFIG.keymap[variant].firmware_url) {
        $("#firmware-dropdown").val(variant);

        loadHexURL(CONFIG.keymap[variant].firmware_url).done(function(s) {
            // load keymap from firmware if #hash(keymap) doesn't exist in URL
            if (!document.location.hash) {
                keymaps = $.extend(true, [], firmware_keymaps); // copy
                while (keymaps.length < KEYMAP_LAYERS) keymaps.push(transparent_map());
            }
            $("#firmware-flash").prop("disabled", false);
            $("#firmware-download").prop("disabled", false);
            $("#keymap-load").prop("disabled", false);
        }).fail(function(d) {
            $("#firmware-flash").prop("disabled", true);
            $("#firmware-download").prop("disabled", true);
            $("#keymap-load").prop("disabled", true);
        }).always(function() {
            $("#keymap-desc").text(CONFIG.keymap[variant].desc);
            layout_load(layout);
        });
    } else {
        $("#keymap-desc").text("Unknown");
        layout_load(layout);
    }
});
