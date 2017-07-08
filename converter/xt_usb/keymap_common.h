/*
Copyright 2011,2012,2013 Jun Wako <wakojun@gmail.com>

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
#ifndef KEYMAP_COMMON_H
#define KEYMAP_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"


/* All keys */
#define KEYMAP_ALL( \
    K00,    K02,K03,K04,K05,K06,K07,K08,K09,K0A,K0B,K0C,K0D, K0E,K0F,K0G,                   \
    K10,K11,K12,K13,K14,K15,K16,K17,K18,K19,K1A,K1B,K1C,K1D, K1E,K1F,K1G,  K1H,K1I,K1J,K1K, \
    K20,K21,K22,K23,K24,K25,K26,K27,K28,K29,K2A,K2B,K2C,K2D, K2E,K2F,K2G,  K2H,K2I,K2J,     \
    K30,K31,K32,K33,K34,K35,K36,K37,K38,K39,K3A,K3B,    K3D,               K3H,K3I,K3J,K3K, \
    K40,K41,K42,K43,K44,K45,K46,K47,K48,K49,K4A,        K4D,     K4F,      K4H,K4I,K4J,     \
    K50,K51,K52,        K55,                K5A,K5B,K5C,K5D, K5E,K5F,K5G,  K5H,    K5J,K5K, \
                                                                                            \
    K60, K61, K62,           /* System Power, Sleep, Wake */                                \
    K70, K71, K72,           /* Mute, Volume Up, Volume Down */                             \
    K80, K81, K82, K83, K84, /* Next, Previous, Stop, Pause, Media Select */                \
    K90, K91, K92,           /* Mail, Calculator, My Computer */                            \
    KA0, KA1, KA2, KA3,      /* WWW Search, Home, Back, Forward */                          \
    KB0, KB1, KB2            /* WWW Stop, Refresh, Favorites */                             \
) { \
    { KC_NO,    KC_##K00, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15, KC_##K16 }, \
    { KC_##K17, KC_##K18, KC_##K19, KC_##K1A, KC_##K1B, KC_##K1C, KC_##K1D, KC_##K20 }, \
    { KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27, KC_##K28 }, \
    { KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_##K3D, KC_##K50, KC_##K31, KC_##K32 }, \
    { KC_##K33, KC_##K34, KC_##K35, KC_##K36, KC_##K37, KC_##K38, KC_##K39, KC_##K3A }, \
    { KC_##K3B, KC_##K10, KC_##K40, KC_##K2D, KC_##K41, KC_##K42, KC_##K43, KC_##K44 }, \
    { KC_##K45, KC_##K46, KC_##K47, KC_##K48, KC_##K49, KC_##K4A, KC_##K4D, KC_##K1J }, \
    { KC_##K52, KC_##K55, KC_##K30, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06 }, \
    { KC_##K07, KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K1H, KC_##K0F, KC_##K2H }, \
    { KC_##K2I, KC_##K2J, KC_##K1K, KC_##K3H, KC_##K3I, KC_##K3J, KC_##K3K, KC_##K4H }, \
    { KC_##K4I, KC_##K4J, KC_##K5H, KC_##K5J, KC_##K5K, KC_##K5D, KC_##K1I, KC_##K0C }, \
    { KC_##K0D, KC_##K5A, KC_##K80, KC_##K51, KC_##K5B, KC_##K5C, KC_##K60, KC_##K61 }, \
    { KC_##K70, KC_##K91, KC_##K83, KC_##K62, KC_##K82, KC_##KA0, KC_##KB2, KC_##KB1 }, \
    { KC_##KB0, KC_##KA3, KC_##KA2, KC_##K92, KC_##K90, KC_##K84, KC_##K81, KC_##K1F }, \
    { KC_##K4F, KC_##K1G, KC_##K72, KC_##K5E, KC_##K71, KC_##K5G, KC_##KA1, KC_##K2F }, \
    { KC_##K5F, KC_##K2G, KC_##K1E, KC_##K2E, KC_##K0E, KC_##K0G, KC_NO   , KC_NO    }  \
}

/* US layout */
#define KEYMAP( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,K78,K07,     KFC,K7E,KFE,                   \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K66, KF0,KEC,KFD,  K77,KCA,K7C,K7B, \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,K5D, KF1,KE9,KFA,  K6C,K75,K7D,     \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,    K5A,               K6B,K73,K74,K79, \
    K12,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,        K59,     KF5,      K69,K72,K7A,     \
    K14,K9F,K11,        K29,                K91,KA7,KAF,K94, KEB,KF2,KF4,  K70,    K71,KDA  \
) \
KEYMAP_ALL( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,K78,K07,     KFC,K7E,KFE,                   \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K66, KF0,KEC,KFD,  K77,KCA,K7C,K7B, \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,K5D, KF1,KE9,KFA,  K6C,K75,K7D,     \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,    K5A,               K6B,K73,K74,K79, \
    K12,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,        K59,     KF5,      K69,K72,K7A,     \
    K14,K9F,K11,        K29,                K91,KA7,KAF,K94, KEB,KF2,KF4,  K70,    K71,KDA, \
                                                                                            \
    SYSTEM_POWER, SYSTEM_SLEEP, SYSTEM_WAKE,                                                \
    AUDIO_MUTE, AUDIO_VOL_UP, AUDIO_VOL_DOWN,                                               \
    MEDIA_NEXT_TRACK, MEDIA_PREV_TRACK, MEDIA_STOP, MEDIA_PLAY_PAUSE, MEDIA_SELECT,         \
    MAIL, CALCULATOR, MY_COMPUTER,                                                          \
    WWW_SEARCH, WWW_HOME, WWW_BACK, WWW_FORWARD,                                            \
    WWW_STOP, WWW_REFRESH, WWW_FAVORITES                                                    \
)

/* ISO layout */
#define KEYMAP_ISO( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,K78,K07,     KFC,K7E,KFE,                   \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K66, KF0,KEC,KFD,  K77,KCA,K7C,K7B, \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,     KF1,KE9,KFA,  K6C,K75,K7D,     \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,K5D,K5A,               K6B,K73,K74,K79, \
    K12,K61,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,    K59,     KF5,      K69,K72,K7A,     \
    K14,K9F,K11,        K29,                K91,KA7,KAF,K94, KEB,KF2,KF4,  K70,    K71,KDA  \
) \
KEYMAP_ALL( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,K78,K07,     KFC,K7E,KFE,                   \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K66, KF0,KEC,KFD,  K77,KCA,K7C,K7B, \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,K5D, KF1,KE9,KFA,  K6C,K75,K7D,     \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,    K5A,               K6B,K73,K74,K79, \
    K12,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,        K59,     KF5,      K69,K72,K7A,     \
    K14,K9F,K11,        K29,                K91,KA7,KAF,K94, KEB,KF2,KF4,  K70,    K71,KDA, \
                                                                                            \
    SYSTEM_POWER, SYSTEM_SLEEP, SYSTEM_WAKE,                                                \
    AUDIO_MUTE, AUDIO_VOL_UP, AUDIO_VOL_DOWN,                                               \
    MEDIA_NEXT_TRACK, MEDIA_PREV_TRACK, MEDIA_STOP, MEDIA_PLAY_PAUSE, MEDIA_SELECT,         \
    MAIL, CALCULATOR, MY_COMPUTER,                                                          \
    WWW_SEARCH, WWW_HOME, WWW_BACK, WWW_FORWARD,                                            \
    WWW_STOP, WWW_REFRESH, WWW_FAVORITES                                                    \
)

/* JIS layout */
#define KEYMAP_JIS( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,K78,K07,         KFC,K7E,KFE,                   \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K6A,K66, KF0,KEC,KFD,  K77,KCA,K7C,K7B, \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,         KF1,KE9,KFA,  K6C,K75,K7D,     \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,K5D,    K5A,               K6B,K73,K74,K79, \
    K12,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,K51,        K59,     KF5,      K69,K72,K7A,     \
    K14,K9F,K11,    K67,K29,K64,K13,            K91,KA7,KAF,K94, KEB,KF2,KF4,  K70,    K71,KDA  \
) \
KEYMAP_ALL( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,K78,K07,     KFC,K7E,KFE,                   \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K66, KF0,KEC,KFD,  K77,KCA,K7C,K7B, \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,K5D, KF1,KE9,KFA,  K6C,K75,K7D,     \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,    K5A,               K6B,K73,K74,K79, \
    K12,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,        K59,     KF5,      K69,K72,K7A,     \
    K14,K9F,K11,        K29,                K91,KA7,KAF,K94, KEB,KF2,KF4,  K70,    K71,KDA, \
                                                                                            \
    SYSTEM_POWER, SYSTEM_SLEEP, SYSTEM_WAKE,                                                \
    AUDIO_MUTE, AUDIO_VOL_UP, AUDIO_VOL_DOWN,                                               \
    MEDIA_NEXT_TRACK, MEDIA_PREV_TRACK, MEDIA_STOP, MEDIA_PLAY_PAUSE, MEDIA_SELECT,         \
    MAIL, CALCULATOR, MY_COMPUTER,                                                          \
    WWW_SEARCH, WWW_HOME, WWW_BACK, WWW_FORWARD,                                            \
    WWW_STOP, WWW_REFRESH, WWW_FAVORITES                                                    \
)

/* XT layout */
#define KEYMAP_XT( \
    K05,K06,    K76, K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45, K4E,K55,K66,   K77,K7E, \
    K04,K0C,    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,  K5A,   K6C,K75,K7D,K7B,     \
    K03,K0B,    K14,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,K0E,         K6B,K73,K74,K79,\
    K83,K0A,    K12,K5D,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,K59,   K7C,  K69,K72,K7A, \
    K01,K09,    K11,                K29,                   K58,              K70,   K71\
) \
KEYMAP_ALL( \
    K76,K05,K06,K04,K0C,K03,K0B,K83,K0A,K01,K09,F11,F12,     PSCR,K7E,BRK,                         \
    K0E,K16,K1E,K26,K25,K2E,K36,K3D,K3E,K46,K45,K4E,K55,K66, INS,HOME,PGUP,    K77,PSLS,K7C,K7B,   \
    K0D,K15,K1D,K24,K2D,K2C,K35,K3C,K43,K44,K4D,K54,K5B,K5D, DEL,END,PGDN,     K6C,K75,K7D,        \
    K58,K1C,K1B,K23,K2B,K34,K33,K3B,K42,K4B,K4C,K52,    K5A,                   K6B,K73,K74,K79,    \
    K12,K1A,K22,K21,K2A,K32,K31,K3A,K41,K49,K4A,        K59,     UP,           K69,K72,K7A,        \
    K14,LGUI,K11,        K29,          RALT,RGUI,APP,RCTL,  LEFT,DOWN,RGHT,    K70,K71,PENT,       \
                                                                                            \
    SYSTEM_POWER, SYSTEM_SLEEP, SYSTEM_WAKE,                                                \
    AUDIO_MUTE, AUDIO_VOL_UP, AUDIO_VOL_DOWN,                                               \
    MEDIA_NEXT_TRACK, MEDIA_PREV_TRACK, MEDIA_STOP, MEDIA_PLAY_PAUSE, MEDIA_SELECT,         \
    MAIL, CALCULATOR, MY_COMPUTER,                                                          \
    WWW_SEARCH, WWW_HOME, WWW_BACK, WWW_FORWARD,                                            \
    WWW_STOP, WWW_REFRESH, WWW_FAVORITES                                                    \
)

#endif
