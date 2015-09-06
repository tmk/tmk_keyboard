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
     * |Shft|Fn1| z | x | c | v | b | n | m | , | . | - |  Shift   |
     * |-----------------------------------------------------------|
     * |LCtl|LGui|LAlt|m1 |Space|m3 | Fn0 | m2 |RAlt|RGui|Menu|RCtl|
     * `--------------'   '-----'   '-----'    '-------------------'
     * Fn0: hold for Function layer
     * Fn1: tap for '<', hold for RAlt
     */
    KEYMAP(
        ESC ,1   ,2   ,3   ,4   ,5   ,6   ,7   ,8   ,9   ,0   ,MINS,EQL ,BSPC, \
        TAB ,Q   ,W   ,E   ,R   ,T   ,Y   ,U   ,I   ,O   ,P   ,LBRC,RBRC,ENT , \
        CAPS,A   ,S   ,D   ,F   ,G   ,H   ,J   ,K   ,L   ,SCLN,QUOT,NUHS,      \
        LSFT,FN1 ,Z   ,X   ,C   ,V   ,B   ,N   ,M   ,COMM,DOT ,SLSH,RSFT,      \
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

    /* 2: Spare layer
     * ,-----------------------------------------------------------.
     * |   |   |   |   |   |   |   |   |   |   |   |   |   |       |
     * |-----------------------------------------------------------|
     * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
     * |------------------------------------------------------.    |
     * |      |   |   |   |   |   |   |   |   |   |   |   |   |    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |   |   |   |   |   |          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |    |
     * `--------------'   '-----'   '-----'    '-------------------'
     */
    KEYMAP(
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS                           ),

    /* 3: Function and Media layer
     * ,-----------------------------------------------------------.
     * | º | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|       |
     * |-----------------------------------------------------------|
     * |     |Mut|Vl-|Vl+|   |   |   |PUp|Up |   |Prt|SLk|Ins|     |
     * |------------------------------------------------------.    |
     * |Backsp|Prv|Nxt|Stp|Ply|   |Hom|Lft|Dwn|Rgt|End|   |Pau|    |
     * |--------------------------O--------------------------------|
     * |    |   |   |   |   |   |   |Fn2|PDn|   |   |Del|          |
     * |-----------------------------------------------------------|
     * |    |    |    |   |     |   |     |    |    |    |    |    |
     * `--------------'   '-----'   '-----'    '-------------------'
     * Fn2: toggle Numpad layer
     */
    KEYMAP(
        GRV ,F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,TRNS, \
        TRNS,MUTE,VOLD,VOLU,TRNS,TRNS,TRNS,PGUP,UP  ,TRNS,PSCR,SLCK,INS ,TRNS, \
        BSPC,MPRV,MNXT,MSTP,MPLY,TRNS,HOME,LEFT,DOWN,RGHT,END ,TRNS,PAUS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,FN2 ,PGDN,TRNS,TRNS,DEL ,TRNS,      \
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS                           )
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
 * Function key actions
 */
const uint16_t PROGMEM fn_actions[] = {
//  [0] = ACTION_LAYER_MOMENTARY(3),             // Hold for Function layer
    [0] = ACTION_LAYER_TAP_TOGGLE(3),            // Hold for Function layer, tap 5 times to toggle
    [1] = ACTION_MODS_TAP_MOD(MOD_RALT,KC_NUBS), // Tap for '<', hold for RAlt
    [2] = ACTION_LAYER_TOGGLE(1)                 // Toggle Numpad layer
//  [3] = ACTION_MACRO(FOUR_SPACES)              // Send 4 spaces
};

