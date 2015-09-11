#include "keymap_common.h"
const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* 0: Default layer
     * ,-----------------------------------------------------------.
     * |Esc| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | ' | ¡ | Backsp|
     * |-----------------------------------------------------------|
     * | Tab | q | w | e | r | t | y | u | i | o | p | ` | + |Enter|
     * |------------------------------------------------------.    |
     * |CapsLk| a | s | d | f | g | h | j | k | l | ñ | ' | ç |    |
     * |--------------------------O--------------------------------|
     * |Shft| < | z | x | c | v | b | n | m | , | . | - |  Shift   |
     * |-----------------------------------------------------------|
     * |LCtl|LGui|LAlt|m1 |Space|m3 | Fn0 | m2 |RAlt|RGui|Menu|RCtl|
     * `--------------'   '-----'   '-----'    '-------------------'
     * Fn0: hold for Function layer
     */
    KEYMAP(
        ESC ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,8   ,9   ,0   ,MINS,EQL ,BSPC, \
        TAB ,Q   ,W   ,E   ,R   ,T   ,Y   ,U   ,I   ,O   ,P   ,LBRC,RBRC,ENT , \
        CAPS,A   ,S   ,D   ,F   ,G   ,H   ,J   ,K   ,L   ,SCLN,QUOT,NUHS,      \
        LSFT,NUBS,Z   ,X   ,C   ,V   ,B   ,N   ,M   ,COMM,DOT ,SLSH,RSFT,      \
        LCTL,LGUI,LALT,SPC ,FN0 ,RALT,RGUI,APP ,RCTL                           ),

    /* 1: Numpad layer
     * ,-----------------------------------------------------------.
     * |   |   |   |   |   |   |   |Np7|Np8|Np9|   |   |   |       |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |Np4|Np5|Np6|   |   |   |Enter|
     * |------------------------------------------------------. Np |
     * |NumLck|   |   |   |   |   |   |Np1|Np2|Np3|   |   |   |    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |   |Np0|   |   |   |          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |    |
     * `--------------'   '-----'   '-----'    '-------------------'
     */
    KEYMAP(
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,P7  ,P8  ,P9  ,TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,P4  ,P5  ,P6  ,TRNS,TRNS,TRNS,PENT, \
        NLCK,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,P1  ,P2  ,P3  ,TRNS,TRNS,TRNS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,P0  ,TRNS,TRNS,TRNS,TRNS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS                           ),

    /* 2: Programmer's layer
     * ,-----------------------------------------------------------.
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
     * |------------------------------------------------------.    |
     * |      |   |   |   |   |   |   |   |   |   |   |   |   |    |
     * |--------------------------O--------------------------------|
     * |Fn4 |Fn3|   |   |   |   |   |   |   |   |   |   |   Fn5    |
     * |-----------------------------------------------------------|
     * |Fn6 |    |Fn8 |   |     |   |     |    |Fn9 |    |    |Fn7 |
     * `--------------'   '-----'   '-----'    '-------------------'
     * Fn3: tap for '<', hold for RAlt
     * Fn4, Fn5: Custom Functions for LShift and RShift
     * Fn6, Fn7: Custom Functions for LCtrl and RCtrl
     * Fn8, Fn9: Custom Functions for Alt and AltGr
     */
    KEYMAP(
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,      \
        FN4 ,FN3 ,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,FN5 ,      \
        FN6 ,TRNS,FN8 ,TRNS,TRNS,FN9 ,TRNS,TRNS,FN7                            ),

    /* 3: Function and Media layer
     * ,-----------------------------------------------------------.
     * | º | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|       |
     * |-----------------------------------------------------------|
     * |     |Mut|Vl-|Vl+|   |   |   |PUp|Up |   |Prt|SLk|Ins|     |
     * |------------------------------------------------------.    |
     * |Backsp|Prv|Nxt|Stp|Ply|   |Hom|Lft|Dwn|Rgt|End|   |Pau|    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |Fn1|PDn|   |   |Del|          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |Fn2 |
     * `--------------'   '-----'   '-----'    '-------------------'
     * Fn1: toggle Numpad layer
     * Fn2: toggle Programmer's layer
     */
    KEYMAP(
        GRV ,F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,TRNS, \
        TRNS,MUTE,VOLD,VOLU,TRNS,TRNS,TRNS,PGUP,UP  ,TRNS,PSCR,SLCK,INS ,TRNS, \
        BSPC,MPRV,MNXT,MSTP,MPLY,TRNS,HOME,LEFT,DOWN,RGHT,END ,TRNS,PAUS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,FN1 ,PGDN,TRNS,TRNS,DEL ,TRNS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,FN2                            )
};

/*
 * Macro identifiers
 */
/*
enum macro_id {
    HELLO,
    FOUR_SPACES,
    ALT_TAB
};
*/

/*
 * Macro definitions
 */
/*
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case HELLO:
            return (record->event.pressed ?
                    MACRO( I(0), T(H), T(E), T(L), T(L), W(255), T(O), END ) :
                    MACRO_NONE );
        case FOUR_SPACES:
            return (record->event.pressed ?
                    MACRO( T(SPC), T(SPC), T(SPC), T(SPC), END ) :
                    MACRO_NONE );
        case ALT_TAB:
            return (record->event.pressed ?
                    MACRO( D(LALT), D(TAB), END ) :
                    MACRO( U(TAB), END ));
    }
    return MACRO_NONE;
}
*/

/*
 * Function identifiers
 */
enum function_id {
    CUSTOM_LSFT,
    CUSTOM_RSFT,
    CUSTOM_LCTL,
    CUSTOM_RCTL,
    CUSTOM_LALT,
    CUSTOM_RALT
};

/*
 * Custom function. Works like ACTION_MODS_TAP_KEY, but applies a second modifier to the tap key.
 * Sends "mod_held" when the key is held, "mod_tap"+"key_tap" when the key is tapped
 */
void action_mods_tap_mod_key(keyrecord_t *record, uint8_t mod_held, uint8_t mod_tap, uint8_t key_tap)
{
    if (record->event.pressed) {
        if (record->tap.count > 0) {
            if (record->tap.interrupted) {
                record->tap.count = 0;  // ad hoc: cancel tap
                register_mods(MOD_BIT(mod_held));
            } else {
                add_weak_mods(MOD_BIT(mod_tap));
                send_keyboard_report();
                register_code(key_tap);
            }
        } else {
            register_mods(MOD_BIT(mod_held));
        }
    } else {
        if (record->tap.count > 0) {
            unregister_code(key_tap);
            del_weak_mods(MOD_BIT(mod_tap));
            send_keyboard_report();
        } else {
            unregister_mods(MOD_BIT(mod_held));
        }
    }
}

/*
 * Function definitions
 */
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case CUSTOM_LSFT:
            action_mods_tap_mod_key(record, KC_LSFT, // Mod to send when held
                                            KC_RSFT, KC_8); // Mod+Key to send when tapped
            break;
        case CUSTOM_RSFT:
            action_mods_tap_mod_key(record, KC_RSFT, // Mod to send when held
                                            KC_LSFT, KC_9); // Mod+Key to send when tapped
            break;
        case CUSTOM_LCTL:
            action_mods_tap_mod_key(record, KC_LCTL, // Mod to send when held
                                            KC_RALT, KC_LBRC); // Mod+Key to send when tapped
            break;
        case CUSTOM_RCTL:
            action_mods_tap_mod_key(record, KC_RCTL, // Mod to send when held
                                            KC_RALT, KC_RBRC); // Mod+Key to send when tapped
            break;
        case CUSTOM_LALT:
            action_mods_tap_mod_key(record, KC_LALT, // Mod to send when held
                                            KC_RALT, KC_QUOT); // Mod+Key to send when tapped
            break;
        case CUSTOM_RALT:
            action_mods_tap_mod_key(record, KC_RALT, // Mod to send when held
                                            KC_RALT, KC_NUHS); // Mod+Key to send when tapped
            break;
        default:
            break;
    }
}

/*
 * Function key actions
 */
const uint16_t PROGMEM fn_actions[] = {
//  [0] = ACTION_LAYER_MOMENTARY(3),  // Hold for Function layer
    [0] = ACTION_LAYER_TAP_TOGGLE(3), // Hold for Function layer, tap 5 times to toggle
    [1] = ACTION_LAYER_TOGGLE(1), // Toggle Numpad layer
    [2] = ACTION_LAYER_TOGGLE(2), // Toggle Programmer's layer
    [3] = ACTION_MODS_TAP_KEY(MOD_RALT, KC_NUBS), // Tap for '<', hold for RAlt
    [4] = ACTION_FUNCTION_TAP(CUSTOM_LSFT),  // Tap for '(', hold for LShift
    [5] = ACTION_FUNCTION_TAP(CUSTOM_RSFT),  // Tap for ')', hold for RShift
    [6] = ACTION_FUNCTION_TAP(CUSTOM_LCTL),  // Tap for '[', hold for LCtrl
    [7] = ACTION_FUNCTION_TAP(CUSTOM_RCTL),  // Tap for ']', hold for RCtrl
    [8] = ACTION_FUNCTION_TAP(CUSTOM_LALT),  // Tap for '{', hold for LAlt
    [9] = ACTION_FUNCTION_TAP(CUSTOM_RALT)   // Tap for '}', hold for RAlt
//  [x] = ACTION_MACRO(FOUR_SPACES) // Send 4 spaces
};
