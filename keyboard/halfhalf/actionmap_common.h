#ifndef ACTIONMAP_COMMON_H
#define ACTIONMAP_COMMON_H

#define ACTIONMAP( \
    K00, K01, K02, K03, K04, K05, K06, \
    K10, K11, K12, K13, K14, K15, K16, \
    K20, K21, K22, K23, K24, K25, K26, \
                        K34, \
   \
    K50, K51, K52, K53, K54, K55, K56, \
    K60, K61, K62, K63, K64, K65, K66, \
    K70, K71, K72, K73, K74, K75, K76, \
              K82 \
) { \
    { AC_##K00, AC_##K01, AC_##K02, AC_##K03, AC_##K04, AC_##K05, AC_##K06 }, \
    { AC_##K10, AC_##K11, AC_##K12, AC_##K13, AC_##K14, AC_##K15, AC_##K16 }, \
    { AC_##K20, AC_##K21, AC_##K22, AC_##K23, AC_##K24, AC_##K25, AC_##K26 }, \
    { KC_NO,    KC_NO,    KC_NO,    KC_NO,    AC_##K34, KC_NO,    KC_NO    }, \
\
    { AC_##K56, AC_##K55, AC_##K54, AC_##K53, AC_##K52, AC_##K51, AC_##K50 }, \
    { AC_##K66, AC_##K65, AC_##K64, AC_##K63, AC_##K62, AC_##K61, AC_##K60 }, \
    { AC_##K76, AC_##K75, AC_##K74, AC_##K73, AC_##K72, AC_##K71, AC_##K70 }, \
    { KC_NO,    KC_NO,    KC_NO,    KC_NO,    AC_##K82, KC_NO,    KC_NO    } \
}
#endif
