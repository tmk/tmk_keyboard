/*
Copyright 2013 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ACTION_MACRO_H
#define ACTION_MACRO_H
#include <stdint.h>
#include "progmem.h"


#define MACRO_NONE      0
#define MACRO(...)      ({ static const macro_t __m[] PROGMEM = { __VA_ARGS__ }; &__m[0]; })
#define MACRO_GET(p)    pgm_read_byte(p)

typedef uint8_t macro_t;


#ifndef NO_ACTION_MACRO
void action_macro_play(const macro_t *macro_p);
#else
#define action_macro_play(macro)
#endif



/* Macro commands
 *   code(0x04-73)                      // key down(1byte)
 *   code(0x04-73) | 0x80               // key up(1byte)
 *   { KEY_DOWN, code(0x04-0xff) }      // key down(2bytes)
 *   { KEY_UP,   code(0x04-0xff) }      // key up(2bytes)
 *   WAIT                               // wait milli-seconds
 *   INTERVAL                           // set interval between macro commands
 *   END                                // stop macro execution
 *
 * Ideas(Not implemented):
 *   modifiers
 *   system usage
 *   consumer usage
 *   unicode usage
 *   function call
 *   conditionals
 *   loop
 */
enum macro_command_id{
    /* 0x00 - 0x03 */
    END                 = 0x00,
    KEY_DOWN,
    KEY_UP,

    /* 0x04 - 0x73 (reserved for keycode down) */

    /* 0x74 - 0x83 */
    WAIT                = 0x74,
    INTERVAL,
    MOD_STORE,
    MOD_RESTORE,
    MOD_CLEAR,

    /* 0x84 - 0xf3 (reserved for keycode up) */

    /* 0xf4 - 0xff */
};


/* TODO: keycode:0x04-0x73 can be handled by 1byte command  else 2bytes are needed
 * if keycode between 0x04 and 0x73
 *      keycode / (keycode|0x80)
 * else
 *      {KEY_DOWN, keycode} / {KEY_UP, keycode}
*/
#define DOWN(key)       KEY_DOWN, (key)
#define UP(key)         KEY_UP, (key)
#define TYPE(key)       DOWN(key), UP(key)
#define WAIT(ms)        WAIT, (ms)
#define INTERVAL(ms)    INTERVAL, (ms)
#define STORE()         MOD_STORE
#define RESTORE()       MOD_RESTORE
#define CLEAR()         MOD_CLEAR

/* key down */
#define D(key)          DOWN(KC_##key)
/* key up */
#define U(key)          UP(KC_##key)
/* key type */
#define T(key)          TYPE(KC_##key)
/* wait */
#define W(ms)           WAIT(ms)
/* interval */
#define I(ms)           INTERVAL(ms)
/* store modifier(s) */
#define SM()            STORE()
/* restore modifier(s) */
#define RM()            RESTORE()
/* clear modifier(s) */
#define CM()            CLEAR()
/* key shift-type */
#define ST(key)         D(LSFT),    T(key),   U(LSFT)
/* modifier utility macros */
/*      Example: Ctrl+Alt+[Shift+]<key1,key2> is CTL_( ALT_( [S]T(key1), [S]T(key2) )) . */
#define SFT_(...)       D(LSFT), __VA_ARGS__, U(LSFT)
#define CTL_(...)       D(LCTL), __VA_ARGS__, U(LCTL)
#define ALT_(...)       D(LALT), __VA_ARGS__, U(LALT)
#define AGR_(...)       D(RALT), __VA_ARGS__, U(RALT)
/* Unicode macros for XOrg/MacOs/Win. */
/*      Example: UNIW_(0,1,4,9) enters a bullet glyph on Windows using Alt+KeyPad(####). */
#define UNIX_(d1,d2,d3,d4)  CTL_(ST(U)),    T(d1), T(d2), T(d3), T(d4)      , T(ENT)
#define UNIM_(d1,d2,d3,d4)  ALT_(           T(d1), T(d2), T(d3), T(d4)      )
#define UNIW_(d1,d2,d3,d4)  ALT_( T(P##d1), T(P##d2), T(P##d3), T(P##d4)    )

/* for backward comaptibility */
#define MD(key)         DOWN(KC_##key)
#define MU(key)         UP(KC_##key)


#endif /* ACTION_MACRO_H */
