#include "keyboard.h"
#include "action.h"
#include "unimap.h"
#include "print.h"
#if defined(__AVR__)
#   include <avr/pgmspace.h>
#endif


/* Keymapping with 16bit action codes */
extern const action_t actionmaps[][UNIMAP_ROWS][UNIMAP_COLS];

// table translates matrix to unimap position
extern const uint8_t unimap_trans[MATRIX_ROWS][MATRIX_COLS];



// translates raw matrix to unimap position code
uint8_t unimap_translate(keypos_t key)
{
    if (key.row >= MATRIX_ROWS || key.col >= MATRIX_COLS) {
        return UNIMAP_NO;
    }

#if defined(__AVR__)
    return pgm_read_byte(&unimap_trans[key.row][key.col]);
#else
    return unimap_trans[key.row][key.col];
#endif
}

/* Converts key to action */
__attribute__ ((weak))
action_t action_for_key(uint8_t layer, keypos_t key)
{
    uint8_t ucode = unimap_translate(key);
    if (ucode == UNIMAP_NO) {
        return (action_t)ACTION_NO;
    }

#if defined(__AVR__)
    return (action_t)pgm_read_word(&actionmaps[(layer)][UNIMAP_ROW(ucode)][UNIMAP_COL(ucode)]);
#else
    return actionmaps[(layer)][UNIMAP_ROW(ucode)][UNIMAP_COL(ucode)];
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
