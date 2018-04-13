/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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
#include "action_layer.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"

/* keymap definition macro
 */
#define KEYMAP( \
    K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, K0D, K0E, \
    K10, K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, K1D, K1E, \
    K20, K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A, K2B, K2C, K2D, K2E, \
    K30, K31, K32, K33, K34, K35, K36, K37, K38, K39, K3A, K3B, K3C, K3D, K3E, \
    K40, K41, K42, K43, K44, K45, K46, K47, K48, K49, K4A, K4B, K4C, K4D, K4E, \
    K50, K51, K52, K53, K54, K55, K56, K57, K58, K59, K5A, K5B, K5C, K5D, K5E, \
    K60, K61, K62, K63, K64, K65, K66, K67, K68, K69, K6A, K6B, K6C, K6D, K6E, \
    K70, K71, K72, K73, K74, K75, K76, K77, K78, K79, K7A, K7B, K7C, K7D, K7E, \
    K80, K81, K82, K83, K84, K85, K86, K87, K88, K89, K8A, K8B, K8C, K8D, K8E, \
    K90, K91, K92, K93, K94, K95, K96, K97, K98, K99, K9A, K9B, K9C, K9D, K9E, \
    KA0, KA1, KA2, KA3, KA4, KA5, KA6, KA7, KA8, KA9, KAA, KAB, KAC, KAD, KAE, \
    KB0, KB1, KB2, KB3, KB4, KB5, KB6, KB7, KB8, KB9, KBA, KBB, KBC, KBD, KBE, \
    KC0, KC1, KC2, KC3, KC4, KC5, KC6, KC7, KC8, KC9, KCA, KCB, KCC, KCD, KCE, \
    KD0, KD1, KD2, KD3, KD4, KD5, KD6, KD7, KD8, KD9, KDA, KDB, KDC, KDD, KDE, \
    KE0, KE1, KE2, KE3, KE4, KE5, KE6, KE7, KE8, KE9, KEA, KEB, KEC, KED, KEE  \
) { \
    { KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07, KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K0C, KC_##K0D, KC_##K0E }, \
    { KC_##K10, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15, KC_##K16, KC_##K17, KC_##K18, KC_##K19, KC_##K1A, KC_##K1B, KC_##K1C, KC_##K1D, KC_##K1E }, \
    { KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27, KC_##K28, KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D, KC_##K2E }, \
    { KC_##K30, KC_##K31, KC_##K32, KC_##K33, KC_##K34, KC_##K35, KC_##K36, KC_##K37, KC_##K38, KC_##K39, KC_##K3A, KC_##K3B, KC_##K3C, KC_##K3D, KC_##K3E }, \
    { KC_##K40, KC_##K41, KC_##K42, KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_##K47, KC_##K48, KC_##K49, KC_##K4A, KC_##K4B, KC_##K4C, KC_##K4D, KC_##K4E }, \
    { KC_##K50, KC_##K51, KC_##K52, KC_##K53, KC_##K54, KC_##K55, KC_##K56, KC_##K57, KC_##K58, KC_##K59, KC_##K5A, KC_##K5B, KC_##K5C, KC_##K5D, KC_##K5E }, \
    { KC_##K60, KC_##K61, KC_##K62, KC_##K63, KC_##K64, KC_##K65, KC_##K66, KC_##K67, KC_##K68, KC_##K69, KC_##K6A, KC_##K6B, KC_##K6C, KC_##K6D, KC_##K6E }, \
    { KC_##K70, KC_##K71, KC_##K72, KC_##K73, KC_##K74, KC_##K75, KC_##K76, KC_##K77, KC_##K78, KC_##K79, KC_##K7A, KC_##K7B, KC_##K7C, KC_##K7D, KC_##K7E }, \
    { KC_##K80, KC_##K81, KC_##K82, KC_##K83, KC_##K84, KC_##K85, KC_##K86, KC_##K87, KC_##K88, KC_##K89, KC_##K8A, KC_##K8B, KC_##K8C, KC_##K8D, KC_##K8E }, \
    { KC_##K90, KC_##K91, KC_##K92, KC_##K93, KC_##K94, KC_##K95, KC_##K96, KC_##K97, KC_##K98, KC_##K99, KC_##K9A, KC_##K9B, KC_##K9C, KC_##K9D, KC_##K9E }, \
    { KC_##KA0, KC_##KA1, KC_##KA2, KC_##KA3, KC_##KA4, KC_##KA5, KC_##KA6, KC_##KA7, KC_##KA8, KC_##KA9, KC_##KAA, KC_##KAB, KC_##KAC, KC_##KAD, KC_##KAE }, \
    { KC_##KB0, KC_##KB1, KC_##KB2, KC_##KB3, KC_##KB4, KC_##KB5, KC_##KB6, KC_##KB7, KC_##KB8, KC_##KB9, KC_##KBA, KC_##KBB, KC_##KBC, KC_##KBD, KC_##KBE }, \
    { KC_##KC0, KC_##KC1, KC_##KC2, KC_##KC3, KC_##KC4, KC_##KC5, KC_##KC6, KC_##KC7, KC_##KC8, KC_##KC9, KC_##KCA, KC_##KCB, KC_##KCC, KC_##KCD, KC_##KCE }, \
    { KC_##KD0, KC_##KD1, KC_##KD2, KC_##KD3, KC_##KD4, KC_##KD5, KC_##KD6, KC_##KD7, KC_##KD8, KC_##KD9, KC_##KDA, KC_##KDB, KC_##KDC, KC_##KDD, KC_##KDE }, \
    { KC_##KE0, KC_##KE1, KC_##KE2, KC_##KE3, KC_##KE4, KC_##KE5, KC_##KE6, KC_##KE7, KC_##KE8, KC_##KE9, KC_##KEA, KC_##KEB, KC_##KEC, KC_##KED, KC_##KEE }  \
}
#endif
