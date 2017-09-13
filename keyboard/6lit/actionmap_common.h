#ifndef ACTIONMAP_COMMON_H
#define ACTIONMAP_COMMON_H

#define ACTIONMAP( \
    K00, K01, K02, \
    K10, K11, K12, \
\
    K20, K21, K22, \
    K30, K31, K32 \
) { \
    { AC_##K00, AC_##K01, AC_##K02 }, \
    { AC_##K10, AC_##K11, AC_##K12 }, \
\
    { AC_##K22, AC_##K21, AC_##K20 }, \
    { AC_##K32, AC_##K31, AC_##K30 } \
}
#endif
