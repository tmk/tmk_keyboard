#include "keyboard.h"
#include "action.h"
#include "unimap.h"
#include "print.h"
#if defined(__AVR__)
#   include <avr/pgmspace.h>
#endif


/* Keymapping with 16bit action codes */
extern const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS];

// table translates matrix to universal keymap
extern const uint8_t unimap_trans[MATRIX_ROWS][MATRIX_COLS];



// translates raw matrix to universal map
keypos_t unimap_translate(keypos_t key)
{
    uint8_t unimap_pos = 
#if defined(__AVR__)
        pgm_read_byte(&unimap_trans[key.row][key.col]);
#else
        unimap_trans[key.row][key.col];
#endif
    return (keypos_t) {
        .row = ((unimap_pos & 0xf0) >> 4),
        .col = (unimap_pos & 0x0f)
    };
}

/* Converts key to action */
__attribute__ ((weak))
action_t action_for_key(uint8_t layer, keypos_t key)
{
    keypos_t uni = unimap_translate(key);
    if ((uni.row << 4 | uni.col) == UNIMAP_NO) {
        return (action_t)ACTION_NO;
    }
#if defined(__AVR__)
    return (action_t)pgm_read_word(&actionmaps[(layer)][(uni.row & 0x7)][(uni.col)]);
#else
    return actionmaps[(layer)][(uni.row & 0x7)][(uni.col)];
#endif
}

/* Macro */
__attribute__ ((weak))
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    return MACRO_NONE;
}

/* Function */
__attribute__ ((weak))
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
}
