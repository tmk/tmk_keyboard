#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdbool.h>
#include <stdint.h>
#include "matrix.h"

#ifdef __cplusplus
extern "C" {
#endif
/* call this every keyboard_task to debounce the matrix*/
void matrix_debounce(void);

/* matrix state on row */
matrix_row_t matrix_debounce_get_row(uint8_t row);

#ifdef __cplusplus
}
#endif

#endif
