#include <stdint.h>
#include "keyboard.h"
#include "action.h"
#include "util.h"
#include "action_layer.h"
#include "hook.h"

#ifdef DEBUG_ACTION
#include "debug.h"
#else
#include "nodebug.h"
#endif


/* 
 * Default Layer State
 */
uint32_t default_layer_state = 0;

static void default_layer_state_set(uint32_t state)
{
    debug("default_layer_state: ");
    default_layer_debug(); debug(" to ");
    default_layer_state = state;
    hook_default_layer_change(default_layer_state);
    default_layer_debug(); debug("\n");
#ifdef NO_TRACK_KEY_PRESS
    clear_keyboard_but_mods(); // To avoid stuck keys
#endif
}

void default_layer_debug(void)
{
    dprintf("%08lX(%u)", default_layer_state, biton32(default_layer_state));
}

void default_layer_set(uint32_t state)
{
    default_layer_state_set(state);
}

#ifndef NO_ACTION_LAYER
void default_layer_or(uint32_t state)
{
    default_layer_state_set(default_layer_state | state);
}
void default_layer_and(uint32_t state)
{
    default_layer_state_set(default_layer_state & state);
}
void default_layer_xor(uint32_t state)
{
    default_layer_state_set(default_layer_state ^ state);
}
#endif


#ifndef NO_ACTION_LAYER
/* 
 * Keymap Layer State
 */
uint32_t layer_state = 0;

static void layer_state_set(uint32_t state)
{
    dprint("layer_state: ");
    layer_debug(); dprint(" to ");
    layer_state = state;
    hook_layer_change(layer_state);
    layer_debug(); dprintln();
#ifdef NO_TRACK_KEY_PRESS
    clear_keyboard_but_mods(); // To avoid stuck keys
#endif
}

void layer_clear(void)
{
    layer_state_set(0);
}

void layer_move(uint8_t layer)
{
    layer_state_set(1UL<<layer);
}

void layer_on(uint8_t layer)
{
    layer_state_set(layer_state | (1UL<<layer));
}

void layer_off(uint8_t layer)
{
    layer_state_set(layer_state & ~(1UL<<layer));
}

void layer_invert(uint8_t layer)
{
    layer_state_set(layer_state ^ (1UL<<layer));
}

void layer_or(uint32_t state)
{
    layer_state_set(layer_state | state);
}
void layer_and(uint32_t state)
{
    layer_state_set(layer_state & state);
}
void layer_xor(uint32_t state)
{
    layer_state_set(layer_state ^ state);
}

void layer_debug(void)
{
    dprintf("%08lX(%u)", layer_state, biton32(layer_state));
}
#endif



/* return layer effective for key at this time */
static uint8_t current_layer_for_key(keypos_t key)
{
    action_t action = ACTION_TRANSPARENT;

#ifndef NO_ACTION_LAYER
    uint32_t layers = layer_state | default_layer_state;
    /* check top layer first */
    for (int8_t i = 31; i >= 0; i--) {
        if (layers & (1UL<<i)) {
            action = action_for_key(i, key);
            if (action.code != (action_t)ACTION_TRANSPARENT.code) {
                return i;
            }
        }
    }
    /* fall back to layer 0 */
    return 0;
#else
    return biton32(default_layer_state);
#endif
}


#ifndef NO_TRACK_KEY_PRESS
/* record layer on where key is pressed */
static uint8_t layer_pressed[MATRIX_ROWS][MATRIX_COLS] = {};
#endif
action_t layer_switch_get_action(keyevent_t event)
{
    uint8_t layer = 0;
#ifndef NO_TRACK_KEY_PRESS
    if (event.pressed) {
        layer = current_layer_for_key(event.key);
        layer_pressed[event.key.row][event.key.col] = layer;
    } else {
        layer = layer_pressed[event.key.row][event.key.col];
    }
#else
    layer = current_layer_for_key(event.key);
#endif
    return action_for_key(layer, event.key);
}
