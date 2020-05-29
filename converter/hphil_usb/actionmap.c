/* HP 98203C Keyboard Keycodes
 *   https://i.imgur.com/QKqVuQu.jpg
 *
 *   Device ID: E0
 *   Describe Record: 0A(Extended, 2 axes?), 61, 0F(Resolution; 3937cpm=100cpi)
 *   Extended Describe Record: 40(Localization), 00(Reserved. No nationality?)
 *   Poll Record:
 *      Keys:               0x60(Keycode Set 2), keycode
 *      Dial:               0x01, XX
 *      Dial with Shift:    0x02, 00(XX), YY
 *
 *   Keycode Set 2 is device dependent and user  definable. Use the LSB to indicate
 *   the key transition (0 = Down, 1 = Up). 128 keys maximum. (p.E-3):
 *
 *         ,___.___.___.___.___.   ,___.___.   ,___.___.___.         ,___.___.___.___.
 *         `---'---'---'---'---'   `---'---'   `---'---'---'         `---'---'---'---'
 *         ,___.___.___.___.___.   ,___.___.   ,___.___.___.         ,___.___.___.___.
 *         `---'---'---'---'---'   `---'---'   `---'---'---'         `---'---'---'---'
 * ,-------------------------------------------------------. ,-----. ,---------------.
 * |Ctl|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Bsp| |PUASE| | E | ( | ) | ^-|
 * |-------------------------------------------------------| |-----| |---------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| | | RUN | |  7|  8|  9| / |
 * |-----------------------------------------------------' | |-----| |---------------|
 * |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '| Ent| |CONTI| |  4|  5|  6| * |
 * |-------------------------------------------------------| `-----' |---------------|
 * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift |         |  1|  2|  3| - |
 * |-------------------------------------------------------' ,-----. |---------------|
 *              |           Space               |            |EXECU| |  0| . | , | + |
 *              `-------------------------------'            `-----' `---------------'
 *
 * Raw Keycodes:
 *       34 36 38 40 42    44 46    50 52 54      60 62 64 66
 *       3A 3C 3E 48 4A    4C 4E    56 58 5A      68 6A 6C 6E
 *
 * 0C A0 A2 A4 A6 A8 AA AC AE B0 B2 B4 B6 5C  70  98 9A 9C 9E
 * 32 D0 D2 D4 D6 D8 DA DC DE C8 CA B8 BA     5E  90 92 94 96
 * 30 E0 E2 E4 E6 E8 EA EC CC CE BC BE 72     74  88 8A 8C 8E
 * 0A F0 F2 F4 F6 F8 FA EE C0 C2 C4 0A            80 82 84 86
 *                C6                          76  78 7A 7C 7E
 *
 *
 * (Raw Keycodes)>>1:
 *       1A 1B 1C 20 21    22 23    28 29 2A      30 31 32 33
 *       1D 1E 1F 24 25    26 27    2B 2C 2D      34 35 36 37
 * 06 50 51 52 53 54 55 56 57 58 59 5A 5B 2E  38  4C 4D 4E 4F
 * 19 68 69 6A 6B 6C 6D 6E 6F 64 65 5C 5D     2F  48 49 4A 4B
 * 18 70 71 72 73 74 75 76 66 67 5E 5F 39     3A  44 45 46 47
 * 05 78 79 7A 7B 7C 7D 77 60 61 62 05            40 41 42 43
 *                63                          3B  3C 3D 3E 3F
 *
 */
#include "actionmap.h"

#define HP98203C( \
        K1A, K1B, K1C, K20, K21,     K22, K23,      K28, K29, K2A,             K30, K31, K32, K33, \
        K1D, K1E, K1F, K24, K25,     K26, K27,      K2B, K2C, K2D,             K34, K35, K36, K37, \
                                                                                                   \
  K06, K50, K51, K52, K53, K54, K55, K56, K57, K58, K59, K5A, K5B, K2E,  K38,  K4C, K4D, K4E, K4F, \
  K19, K68, K69, K6A, K6B, K6C, K6D, K6E, K6F, K64, K65, K5C, K5D,       K2F,  K48, K49, K4A, K4B, \
  K18, K70, K71, K72, K73, K74, K75, K76, K66, K67, K5E, K5F, K39,       K3A,  K44, K45, K46, K47, \
  K05, K78, K79, K7A, K7B, K7C, K7D, K77, K60, K61, K62,                       K40, K41, K42, K43, \
                           K63,                                          K3B,  K3C, K3D, K3E, K3F  \
) { \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_##K05, AC_##K06, AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_##K18, AC_##K19, AC_##K1A, AC_##K1B, AC_##K1C, AC_##K1D, AC_##K1E, AC_##K1F }, \
    { AC_##K20, AC_##K21, AC_##K22, AC_##K23, AC_##K24, AC_##K25, AC_##K26, AC_##K27, AC_##K28, AC_##K29, AC_##K2A, AC_##K2B, AC_##K2C, AC_##K2D, AC_##K2E, AC_##K2F }, \
    { AC_##K30, AC_##K31, AC_##K32, AC_##K33, AC_##K34, AC_##K35, AC_##K36, AC_##K37, AC_##K38, AC_##K39, AC_##K3A, AC_##K3B, AC_##K3C, AC_##K3D, AC_##K3E, AC_##K3F }, \
    { AC_##K40, AC_##K41, AC_##K42, AC_##K43, AC_##K44, AC_##K45, AC_##K46, AC_##K47, AC_##K48, AC_##K49, AC_##K4A, AC_##K4B, AC_##K4C, AC_##K4D, AC_##K4E, AC_##K4F }, \
    { AC_##K50, AC_##K51, AC_##K52, AC_##K53, AC_##K54, AC_##K55, AC_##K56, AC_##K57, AC_##K58, AC_##K59, AC_##K5A, AC_##K5B, AC_##K5C, AC_##K5D, AC_##K5E, AC_##K5F }, \
    { AC_##K60, AC_##K61, AC_##K62, AC_##K63, AC_##K64, AC_##K65, AC_##K66, AC_##K67, AC_##K68, AC_##K69, AC_##K6A, AC_##K6B, AC_##K6C, AC_##K6D, AC_##K6E, AC_##K6F }, \
    { AC_##K70, AC_##K71, AC_##K72, AC_##K73, AC_##K74, AC_##K75, AC_##K76, AC_##K77, AC_##K78, AC_##K79, AC_##K7A, AC_##K7B, AC_##K7C, AC_##K7D, AC_NO,    AC_NO    }  \
}
/*
            K34, K36, K38, K40, K42,      K44, K46,      K50, K52, K54,        K60, K62, K64, K66, \
            K3A, K3C, K3E, K48, K4A,      K4C, K4E,      K56, K58, K5A,        K68, K6A, K6C, K6E, \
                                                                                                   \
  K0C, KA0, KA2, KA4, KA6, KA8, KAA, KAC, KAE, KB0, KB2, KB4, KB6, K5C,  K70,  K98, K9A, K9C, K9E, \
  K32, KD0, KD2, KD4, KD6, KD8, KDA, KDC, KDE, KC8, KCA, KB8, KBA,       K5E,  K90, K92, K94, K96, \
  K30, KE0, KE2, KE4, KE6, KE8, KEA, KEC, KCC, KCE, KBC, KBE, K72,       K74,  K88, K8A, K8C, K8E, \
  K0A, KF0, KF2, KF4, KF6, KF8, KFA, KEE, KC0, KC2, KC4,                       K80, K82, K84, K86, \
                                KC6,                                     K76,  K78, K7A, K7C, K7E  \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_##K0A, AC_##K0C, AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO    }, \
    { AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_NO,    AC_##K30, AC_##K32, AC_##K34, AC_##K36, AC_##K38, AC_##K3A, AC_##K3C, AC_##K3E }, \
    { AC_##K40, AC_##K42, AC_##K44, AC_##K46, AC_##K48, AC_##K4A, AC_##K4C, AC_##K4E, AC_##K50, AC_##K52, AC_##K54, AC_##K56, AC_##K58, AC_##K5A, AC_##K5C, AC_##K5E }, \
    { AC_##K60, AC_##K62, AC_##K64, AC_##K66, AC_##K68, AC_##K6A, AC_##K6C, AC_##K6E, AC_##K70, AC_##K72, AC_##K74, AC_##K76, AC_##K78, AC_##K7A, AC_##K7C, AC_##K7E }, \
    { AC_##K80, AC_##K82, AC_##K84, AC_##K86, AC_##K88, AC_##K8A, AC_##K8C, AC_##K8E, AC_##K90, AC_##K92, AC_##K94, AC_##K96, AC_##K98, AC_##K9A, AC_##K9C, AC_##K9E }, \
    { AC_##KA0, AC_##KA2, AC_##KA4, AC_##KA6, AC_##KA8, AC_##KAA, AC_##KAC, AC_##KAE, AC_##KB0, AC_##KB2, AC_##KB4, AC_##KB6, AC_##KB8, AC_##KBA, AC_##KBC, AC_##KBE }, \
    { AC_##KC0, AC_##KC2, AC_##KC4, AC_##KC6, AC_##KC8, AC_##KCA, AC_##KCC, AC_##KCE, AC_##KD0, AC_##KD2, AC_##KD4, AC_##KD6, AC_##KD8, AC_##KDA, AC_##KDC, AC_##KDE }, \
    { AC_##KE0, AC_##KE2, AC_##KE4, AC_##KE6, AC_##KE8, AC_##KEA, AC_##KEC, AC_##KEE, AC_##KF0, AC_##KF2, AC_##KF4, AC_##KF6, AC_##KF8, AC_##KFA, AC_NO,    AC_NO    }, \
*/


#define AC_L1  ACTION_LAYER_MOMENTARY(1)

#ifdef KEYMAP_SECTION_ENABLE
const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS] __attribute__ ((section (".keymap.keymaps"))) = {
#else
const action_t actionmaps[][MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
#endif
    [0] = HP98203C(
                  F6,  F7,  F8,  F9,  F10,      DOWN,UP,       PGDN,PGUP,HOME,         LGUI,VOLD,VOLU,MUTE,
                  F1,  F2,  F3,  F4,  F5,       LEFT,RGHT,     INS, DEL, END,          F11, F12, PSCR,SLCK,

        ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSPC,  GRV,   NLCK,HENK,MHEN,PEQL,
        TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,       BSLS,  P7,  P8,  P9,  PSLS,
        LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   SCLN,QUOT,ENT,        LALT,  P4,  P5,  P6,  PAST,
        LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, SLSH,                        P1,  P2,  P3,  PMNS,
                                 SPC,                                           L1,    P0,  PDOT,PCMM,PPLS
    ),
    [1] = HP98203C(
                  F16, F17, F18, F19, F20,      PGDN,PGUP,     TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,BTLD,
                  F11, F12, F13, F14, F15,      HOME,END,      TRNS,TRNS,TRNS,         TRNS,TRNS,TRNS,CAPS,

        GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, BSLS,  TRNS,  TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,       LALT,  TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,       LGUI,  TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,                        TRNS,TRNS,TRNS,TRNS,
                                 TRNS,                                          TRNS,  RSFT,TRNS,TRNS,TRNS
    ),
};
