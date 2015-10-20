/*
 * SpaceFN
 * http://geekhack.org/index.php?topic=51069.0
 *
 * This is a custom spacefn layout with an additional fn layer.
 *
 * It additionally demonstrates the use of actions to enable
 * variations such as Esc/~ and Backspace/|.
 *
 * Macros are also used in the fn layer.
 *
 */
#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Layer 0: Default Layer
 ,----------------------------------------------------------------------------.
 |Esc/~ |   1|   2|   3|   4|   5|   6|   7|   8|   9|   0|   -|   =|VolD|VolU|
 |----------------------------------------------------------------------------|
 | Tab  |   Q|   W|   E|   R|   T|   Y|   U|   I|   O|   P|   [|   ]|BkSp/Pipe|
 |----------------------------------------------------------------------------|
 |Control |   A|   S|   D|   F|   G|   H|   J|   K|   L|   ;|   '| Enter      |
 |----------------------------------------------------------------------------|
 | Shift   |   Z|   X|   C|   V|   B|   N|   M|   ,|   .|   /| Shift    |Fn   |
 `----------------------------------------------------------------------------'
           |Gui  | Alt   |         SpaceFN         |  Alt  |Gui  |
           `-----------------------------------------------------'
 
 Esc/~
     Tap normally to generate an Escape
     Shift press to generate a tilde
     (grave accent is available on the SpaceFN layer below)

 Backspace/Pipe (|)
     Tap or press normally to backspace
     Shift press to generate a pipe character
     (Backslash is on the SpaceFN layer below)

*/

KEYMAP(
  FN2 ,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0,MINS,EQL ,VOLD,VOLU,\
  TAB ,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,LBRC,RBRC,FN3 ,     \
  LCTL,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCLN,QUOT,ENT ,          \
  LSFT,   Z,   X,   C,   V,   B,   N,   M,COMM,DOT ,SLSH,RSFT,FN1 ,          \
  LGUI,LALT,FN0 ,RALT,RGUI),

/* Layer 1: SpaceFN
 ,----------------------------------------------------------------------------.
 |`     |  F1|  F2|  F3|  F4|  F5|  F6|  F7|  F8|  F9| F10| F11| F12|Mute|OutP|
 |----------------------------------------------------------------------------|
 | FIND | KP6| KP7| KP8| KP9| KP0|    |    |    |    |PSCN|    |    |Backslash|
 |----------------------------------------------------------------------------|
 |        | KP1| KP2| KP3| KP4| KP5|LEFT|DOWN|UP  |RGHT|HOME|END | Execute    |
 |----------------------------------------------------------------------------|
 |         |UNDO|CUT |COPY|PAST|HELP|MENU|SELC|AGAN|LNG1|LNG2|          |Stop |
 `----------------------------------------------------------------------------'
           |     |       |                         |       |     |
           `-----------------------------------------------------'

Backslash notes:
  A custom function is used in lieu of the standard "BSLS" in the keymap
  as, at least in testing under linux, it did not generate the correct
  key (backslash). Instead we call ACTION_KEY(KC_BSLS) (see functions below)
  which does work as intended.

*/

KEYMAP(
  GRV ,F1  ,F2  ,F3  ,F4  ,F5  ,F6  ,F7  ,F8  ,F9  ,F10 ,F11 ,F12 ,MUTE,OUT ,\
  FIND,KP_6,KP_7,KP_8,KP_9,KP_0,TRNS,TRNS,TRNS,TRNS,PSCR,TRNS,TRNS,FN4 ,     \
  LCTL,KP_1,KP_2,KP_3,KP_4,KP_5,LEFT,DOWN,UP  ,RGHT,HOME,END ,EXECUTE,       \
  TRNS,UNDO,CUT ,COPY,PASTE,HELP,MENU,SELECT,AGAIN,LANG1,LANG2,TRNS,STOP,    \
  TRNS,TRNS,TRNS,TRNS,TRNS),

/* Layer 2: Macros (FN1)
 ,----------------------------------------------------------------------------.
 |`     |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
 |----------------------------------------------------------------------------|
 |      |    |    |MACR|    |    |MACR|    |    |MACR|    |    |    |         |
 |----------------------------------------------------------------------------|
 |        |    |    |    |    |    |    |    |    |    |    |    |            |
 |----------------------------------------------------------------------------|
 |         |    |    |    |    |    |MACR|    |    |    |    |          |     |
 `----------------------------------------------------------------------------'
           |     |       |                         |       |     |
           `-----------------------------------------------------'
 
Macros:
  E: email address
  T: twitter nick
  I: irc nick
  N: full name

*/

KEYMAP(
  NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,VOLU,VOLD,\
  NO  ,NO  ,NO  ,FN21,NO  ,FN23,NO  ,NO  ,FN24,NO  ,NO  ,NO  ,NO  ,NO  ,     \
  NO  ,NO  ,NO  ,FN25,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,          \
  NO  ,NO  ,NO  ,NO  ,NO  ,NO  ,FN22,NO  ,NO  ,NO  ,NO  ,NO  ,TRNS,          \
  NO  ,NO  ,NO  ,NO  ,NO  ),

};

/*
 * user defined action function
 */
enum function_id {
    SHIFT_ESC,   // Shift + Esc -> Tilde
    SHIFT_BSPC,  // Shift + Backspace -> Pipe
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    if (record->event.pressed) dprint("P"); else dprint("R");
    if (record->tap.interrupted) dprint("i");

#   define MODS_SHIFT_MASK   (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
    static uint8_t shift_mod;

    switch (id) {
        // Shift + Esc -> ~
        case SHIFT_ESC:
            shift_mod = get_mods()&MODS_SHIFT_MASK;
            if (record->event.pressed) {
                if (shift_mod) {
                    add_key(KC_GRV);
                    send_keyboard_report(); // send grave with shift for tilde result
                } else {
                    del_mods(shift_mod); // remove shift
                    add_key(KC_ESC);
                    send_keyboard_report(); // send escape
                    add_mods(shift_mod); // return shift but not sent
                }
            } else {
                if (shift_mod) {
                    del_key(KC_GRV);
                    send_keyboard_report();
                } else {
                    del_key(KC_ESC);
                    send_keyboard_report();
                }
            }
            break;
        case SHIFT_BSPC:
            shift_mod = get_mods()&MODS_SHIFT_MASK;
            if (record->event.pressed) {
                if (shift_mod) {
                    add_key(KC_BSLS);
                    send_keyboard_report(); // send backslash with shift for pipe result
                } else {
                    del_mods(shift_mod); // remove shift
                    add_key(KC_BSPC);
                    send_keyboard_report(); // send backspace
                    add_mods(shift_mod); // return shift but not sent
                }
            } else {
                if (shift_mod) {
                    del_key(KC_BSLS);
                    del_key(KC_BSPC);
                    send_keyboard_report();
                } else {
                    del_key(KC_BSPC);
                    send_keyboard_report();
                }
            }
            break;
    }
}

/*
 * Macros
 */
enum macro_id {
    SIG,
    FULLNAME,
    EMAIL,
    TWITTER,
    IRC,
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch (id) {
        case SIG:
            return (record->event.pressed ?
                    MACRO( \
		      \
                      I(0), \
		      \
                      D(LSHIFT), T(E), U(LSHIFT), T(T), T(H), T(A), T(N), \
                      T(SPACE), \
                      D(LSHIFT), T(S), U(LSHIFT), T(C), T(H), T(O), T(O), T(N), T(O), T(V), T(E), T(R), \
		      \
                      D(LSHIFT), T(ENT), U(LSHIFT), \
		      \
                      T(M), T(A), T(I), T(L), D(LSHIFT), T(SCLN), U(LSHIFT), T(SPACE), \
                      T(E), T(S), \
                      D(LSHIFT), T(2), U(LSHIFT), \
                      T(E), T(T), T(H), T(A), T(N), \
                      T(S), T(C), T(H), T(O), T(O), T(N), T(O), T(V), T(E), T(R), T(DOT), T(C), T(O), T(M), \
		      \
                      D(LSHIFT), T(ENT), U(LSHIFT), \
		      \
                      T(T), T(W), T(I), T(T), T(T), T(E), T(R), D(LSHIFT), T(SCLN), U(LSHIFT), T(SPACE), \
                      D(LSHIFT), T(2), U(LSHIFT), \
                      T(E), T(T), T(H), T(A), T(N), \
                      T(S), T(C), T(H), T(O), T(O), T(N), T(O), T(V), T(E), T(R), \
		      \
                      D(LSHIFT), T(ENT), U(LSHIFT), \
		      \
                      T(G), T(I), T(T), T(H), T(U), T(B), D(LSHIFT), T(EQL), U(LSHIFT), T(I), T(R), T(C), D(LSHIFT), T(SCLN), U(LSHIFT), T(SPACE), \
                      T(A), T(L), T(T), T(E), T(R), T(C), T(A), T(T), T(I), T(O), T(N), \
		      \
                      D(LSHIFT), T(ENT), U(LSHIFT), \
		      \
		      \
                      END ) :
                    MACRO_NONE );
        case FULLNAME:
            return (record->event.pressed ?
                    MACRO( \
                      I(0), \
                      D(LSHIFT), T(E), U(LSHIFT), T(T), T(H), T(A), T(N), \
                      T(SPACE), \
                      D(LSHIFT), T(S), U(LSHIFT), T(C), T(H), T(O), T(O), T(N), T(O), T(V), T(E), T(R), \
                      END ) :
                    MACRO_NONE );
        case EMAIL:
            return (record->event.pressed ?
                    MACRO( \
                      I(0), \
                      T(E), T(S), \
                      D(LSHIFT), T(2), U(LSHIFT), \
                      T(E), T(T), T(H), T(A), T(N), \
                      T(S), T(C), T(H), T(O), T(O), T(N), T(O), T(V), T(E), T(R), T(DOT), T(C), T(O), T(M), \
                      END ) :
                    MACRO_NONE );
        case TWITTER:
            return (record->event.pressed ?
                    MACRO( \
                      I(0), \
                      D(LSHIFT), T(2), U(LSHIFT), \
                      T(E), T(T), T(H), T(A), T(N), T(S), T(C), T(H), T(O), T(O), T(N), T(O), T(V), T(E), T(R), \
                      END ) :
                    MACRO_NONE );
        case IRC:
            return (record->event.pressed ?
                    MACRO( \
                      I(0), \
                      D(LSHIFT), T(2), U(LSHIFT), \
                      T(A), T(L), T(T), T(E), T(R), T(C), T(A), T(T), T(I), T(O), T(N), \
                      END ) :
                    MACRO_NONE );
        default:
            ACTION_DEFAULT_LAYER_SET(0);
    }
    return MACRO_NONE;
}

const uint16_t PROGMEM fn_actions[] = {

    [0] = ACTION_LAYER_TAP_KEY(1, KC_SPACE),
    [1] = ACTION_LAYER_MOMENTARY(2),
    [2] = ACTION_FUNCTION(SHIFT_ESC),
    [3] = ACTION_FUNCTION(SHIFT_BSPC),
    [4] = ACTION_KEY(KC_BSLS),

    [21] = ACTION_MACRO(EMAIL),
    [22] = ACTION_MACRO(FULLNAME),
    [23] = ACTION_MACRO(TWITTER),
    [24] = ACTION_MACRO(IRC),
    [25] = ACTION_MACRO(SIG),

};

