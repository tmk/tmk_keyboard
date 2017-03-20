/*
Copyright 2015 Jun Wako <wakojun@gmail.com>

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
#ifndef ACTIONMAP_COMMON_H
#define ACTIONMAP_COMMON_H
#include "rgblight.h"

enum function_id {
    RGBLED_TOGGLE,
    RGBLED_STEP_MODE,
    RGBLED_INCREASE_HUE,
    RGBLED_DECREASE_HUE,
    RGBLED_INCREASE_SAT,
    RGBLED_DECREASE_SAT,
    RGBLED_INCREASE_VAL,
    RGBLED_DECREASE_VAL,
};

/* keymap definition macro */

#define ACTIONMAP( \
    K00, K01, K02, K03, \
	K10, K11, K12, K13, \
    K20, K21, K22, K23  \
) { \
    { AC_##K00, AC_##K01, AC_##K02, AC_##K03 }, \
    { AC_##K10, AC_##K11, AC_##K12, AC_##K13 }, \
    { AC_##K20, AC_##K21, AC_##K22, AC_##K23 } \
}
#endif
