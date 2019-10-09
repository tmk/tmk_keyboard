/*
Basic global debounce algorithm.
When no state changes have occured for DEBOUNCE milliseconds, we push the state.
*/

#include "debounce.h"
#include "matrix.h"
#include "timer.h"

#ifndef DEBOUNCE
#define DEBOUNCE 5
#endif

//Default implementation - no debouncing
static matrix_row_t matrix_debounced[MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time;

void matrix_debounce(void) {    
    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        matrix_row_t raw = matrix_get_row(r);
        if (raw != matrix_debounced[r])
        {
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int i = 0; i < MATRIX_ROWS; i++) {
            matrix_debounced[i] = matrix_get_row(i);
        }
        debouncing = false;
    }
}

matrix_row_t matrix_debounce_get_row(uint8_t row)
{
    return matrix_debounced[row];
}
