// copied from QMK...

/* Copyright 2018 fuge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEYMAP_HUNGARIAN
#define KEYMAP_HUNGARIAN

#include "keycodes.h"

// basic letters
#define HU_Z KC_Y
#define HU_Y KC_Z

#define HU_A KC_A
#define HU_B KC_B
#define HU_C KC_C
#define HU_D KC_D
#define HU_E KC_E
#define HU_F KC_F
#define HU_G KC_G
#define HU_H KC_H
#define HU_I KC_I
#define HU_J KC_J
#define HU_K KC_K
#define HU_L KC_L
#define HU_M KC_M
#define HU_N KC_N
#define HU_O KC_O
#define HU_P KC_P
#define HU_Q KC_Q
#define HU_R KC_R
#define HU_S KC_S
#define HU_T KC_T
#define HU_U KC_U
#define HU_V KC_V
#define HU_W KC_W
#define HU_X KC_X

// num row
#define HU_0 KC_GRV
#define HU_1 KC_1
#define HU_2 KC_2
#define HU_3 KC_3
#define HU_4 KC_4
#define HU_5 KC_5
#define HU_6 KC_6
#define HU_7 KC_7
#define HU_8 KC_8
#define HU_9 KC_9
#define HU_OE KC_0

#define HU_UE KC_MINS
#define HU_OO KC_EQL

// q row
#define HU_OEE KC_LBRC
#define HU_UU KC_RBRC

// a row
#define HU_EE KC_SCLN
#define HU_AA KC_QUOT
#define HU_UEE KC_NUHS

#define HU_MINS KC_SLSH // - 

#define HU_DOT KC_DOT
#define HU_COMM KC_COMM

// shifted characters
// num row
#define HU_PARA HU_0 // §
#define HU_PARA_MODS MOD_LSHIFT
#define HU_QUOT HU_1 // '
#define HU_QUOT_MODS MOD_LSHIFT
#define HU_DQOT HU_2 // "
#define HU_DQOT_MODS MOD_LSHIFT
#define HU_PLUS HU_3 // + 
#define HU_PLUS_MODS MOD_LSHIFT
#define HU_EXLM HU_4 // !
#define HU_EXLM_MODS MOD_LSHIFT
#define HU_PERC HU_5 // %
#define HU_PERC_MODS MOD_LSHIFT
#define HU_SLSH HU_6 // /
#define HU_SLSH_MODS MOD_LSHIFT
#define HU_EQL  HU_7 // =
#define HU_EQL_MODS MOD_LSHIFT
#define HU_LPRN HU_8 // (
#define HU_LPRN_MODS MOD_LSHIFT
#define HU_RPRN HU_9 // )
#define HU_RPRN_MODS MOD_LSHIFT

// í,y row
#define HU_II	KC_NUBS
#define HU_QST  HU_COMM // ?
#define HU_QST_MODS MOD_LSHIFT
#define HU_COLN HU_DOT // :
#define HU_COLN_MODS MOD_LSHIFT 
#define HU_UNDS HU_MINS // _
#define HU_UNDS_MODS MOD_LSHIFT 

// Alt Gr'd characters
// num row
#define HU_TILD HU_1 // ~
#define HU_TILD_MODS MOD_RALT
//#define HU_?? HU_2 // ˇ (proper name?)
//#define HU_??_MODS MOD_RALT
#define HU_CIRC HU_3 // ^
#define HU_CIRC_MODS MOD_RALT
#define HU_BRV HU_4 // ˘
#define HU_BRV_MODS MOD_RALT
#define HU_RING HU_5 // °
#define HU_RING_MODS MOD_RALT
//#define HU_?? HU_6 // ˛ (proper name?)
//#define HU_??_MODS MOD_RALT
#define HU_GRV HU_7 // `
#define HU_GRV_MODS MOD_RALT
//#define HU_?? HU_8 // ˙ (proper name?)
//#define HU_??_MODS MOD_RALT
#define HU_ACUT HU_9 // ´ 
#define HU_ACUT_MODS MOD_RALT

// q row
#define HU_BSLS HU_Q // \ backslash 
#define HU_BSLS_MODS MOD_RALT
#define HU_PIPE HU_W // |
#define HU_PIPE_MODS MOD_RALT
#define HU_DIV HU_OEE // ÷
#define HU_DIV_MODS MOD_RALT
#define HU_CRSS HU_UU // ×
#define HU_CRSS_MODS MOD_RALT
#define HU_EURO HU_U // €
#define HU_EURO_MODS MOD_RALT

// a row
#define HU_LBRC HU_F // [
#define HU_LBRC_MODS MOD_RALT
#define HU_RBRC HU_G // ]
#define HU_RBRC_MODS MOD_RALT
#define HU_DLR  HU_EE // $
#define HU_DLR_MODS MOD_RALT
#define HU_SS HU_AA // ß
#define HU_SS_MODS MOD_RALT

// í,y row
#define HU_LESS KC_NUBS // < 
#define HU_LESS_MODS MOD_RALT
#define HU_MORE HU_Y // >
#define HU_MORE_MODS MOD_RALT
#define HU_HASH HU_X // # 
#define HU_HASH_MODS MOD_RALT
#define HU_AMPR HU_C // &
#define HU_AMPR_MODS MOD_RALT
#define HU_AT  HU_V // @
#define HU_AT_MODS MOD_RALT
#define HU_LCBR HU_B// {
#define HU_LCBR_MODS MOD_RALT
#define HU_RCBR HU_N // }
#define HU_RCBR_MODS MOD_RALT
#define HU_SCLN HU_COMM // ;
#define HU_SCLN_MODS MOD_RALT
#define HU_ASTR HU_MINS // *
#define HU_ASTR_MODS MOD_RALT


#include "../common/String2CodeMap.h"

static const absolem::String2CodeMap string2code = {
    // just a little stub for the battery reporting
    {'0', {0, HU_0}},
    {'1', {0, HU_1}},
    {'2', {0, HU_2}},
    {'3', {0, HU_3}},
    {'4', {0, HU_4}},
    {'5', {0, HU_5}},
    {'6', {0, HU_6}},
    {'7', {0, HU_7}},
    {'8', {0, HU_8}},
    {'9', {0, HU_9}},
    {'%', {HU_PERC_MODS, HU_PERC}}
};

#endif