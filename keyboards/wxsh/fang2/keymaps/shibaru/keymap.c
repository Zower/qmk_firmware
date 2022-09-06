/* Copyright 2021 Andreas Karlsvik
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

#include QMK_KEYBOARD_H
#include <stdio.h>

#ifdef ENCODER_ENABLE
    #include "encoder.c"
#endif

enum layer {
    _COLE,
    _SPEC,
    _HRDIV,
    _SEC,
    _GAMING
};

enum custom_keycodes {
    KC_SHCT = SAFE_RANGE,
    KC_AE, // Æ, only with no dead keys english keyboard
    KC_OE, // Ø
    KC_AA, // Å
};

#define KC_TGLS LT(_SPEC, KC_ENTER)
#define KC_TGLSE LT(_SEC, KC_LGUI)
#define KC_TGLHR LT(_HRDIV, KC_SPACE)
#define KC_LSSHC LSFT_T(KC_SHCT)
#define KC_SHSPC MT(MOD_)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLE] = LAYOUT(
                 KC_ESC,   KC_1,    KC_2,    KC_3,     KC_4,     KC_5,     XXXXXXX,                                  XXXXXXX, KC_6,    KC_7,     KC_8,    KC_9,    KC_0,     TG(_COLE),
                 KC_TAB,   KC_Q,    KC_W,    KC_F,     KC_P,     KC_B,     XXXXXXX,                                  XXXXXXX, KC_J,    KC_L,     KC_U,    KC_Y,    KC_SCLN,  XXXXXXX,
        	     KC_BSPC,  KC_A,    KC_R,    KC_S,     KC_T,     KC_G,                                                        KC_M,    KC_N,     KC_E,    KC_I,    KC_O,     KC_ESC,
                 KC_LSFT,  KC_Z,    KC_X,    KC_C,     KC_D,     KC_V,     KC_MUTE, XXXXXXX,               XXXXXXX, KC_MUTE,  KC_K,    KC_H,     KC_COMM, KC_DOT,  KC_SLASH, TG(_GAMING),
                                             KC_LGUI,  KC_LALT,  KC_TGLHR, KC_LCTRL, XXXXXXX,              XXXXXXX, XXXXXXX,  KC_TGLS, KC_TGLSE, XXXXXXX 
    ),
    [_SPEC] = LAYOUT(
                 _______, KC_F1,      KC_F2,    KC_F3,    KC_F4,     KC_F5,       KC_F11, 		                     KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  _______,
                 _______, KC_EXCLAIM, KC_AT,    KC_HASH,  KC_DOLLAR, KC_PERCENT,  _______,                           _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_TILDE, KC_GRV,  XXXXXXX,
        	     _______, KC_LBRC,    KC_RBRC,  KC_LPRN,  KC_RPRN,   XXXXXXX,                                                 XXXXXXX, KC_LCBR, KC_RCBR, KC_COLN,  KC_SCLN, _______,
                 _______, XXXXXXX,    XXXXXXX,  KC_BSLS,  KC_PLUS,   XXXXXXX,     _______, _______,         _______, _______, XXXXXXX, KC_QUES, KC_PIPE, XXXXXXX,  _______, _______,
                                                 _______, _______,   _______,     _______, _______,         _______, _______, XXXXXXX, _______, _______
    ),
    [_HRDIV] = LAYOUT(
                 _______, _______,    _______,  _______,  _______, _______, _______,   		                        _______, _______, _______,  _______, _______,  _______,  _______,
                 _______, _______,    _______,  _______,  _______, _______, _______,                                _______, _______, _______,  _______, _______,  _______,  _______,
        	     _______, KC_EXCLAIM, KC_EQUAL, KC_QUOTE, KC_DQUO, XXXXXXX,                                                 XXXXXXX,  KC_MINUS, KC_UNDS, KC_LABK,  KC_RABK,  _______,
                 _______, _______,    _______,  _______, _______,  _______, _______, _______,             _______, _______, _______,  _______,  _______, _______,  _______,  _______,
                                                _______, _______,  _______, _______, _______,             _______, _______, XXXXXXX,  _______,   _______
    ),
    [_SEC] = LAYOUT(
                 _______, _______, _______, _______,  _______, _______,  _______, 		                       _______, _______, _______,  _______, _______,  _______,  _______,
                 _______, _______, _______, _______,  _______, _______,  _______,                              _______, KC_AE,   KC_OE,   KC_AA, _______,  _______,  _______,
                 _______, _______, _______, _______,  _______, _______,                                        _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______,
                 _______, _______, _______, _______,  _______, _______,  _______, _______,            _______, _______, _______, _______, _______, _______, _______,  _______,
                                            _______,  _______, _______,  _______, _______,            _______, _______, XXXXXXX, _______ ,_______
    ),
    [_GAMING] = LAYOUT(
                 KC_ESC,   KC_1,    KC_2,    KC_3,     KC_4,     KC_5,  KC_GRV,                                       KC_C,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
                 KC_TAB,   KC_Q,    KC_W,    KC_E,     KC_R,     KC_T,  KC_B,                                         KC_D,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    XXXXXXX,
        	     KC_LSFT,  KC_A,    KC_S,    KC_D,     KC_F,     KC_G,                                                         KC_H,    KC_J,    KC_K,    KC_L,    XXXXXXX, XXXXXXX,
                 KC_LCTRL, KC_Z,    KC_X,    KC_C,     KC_V,     KC_B,  KC_MUTE,  KC_LGUI,                   KC_INS,  _______, KC_N,    KC_M,    KC_COMM, KC_DOT,  XXXXXXX, _______,
                                             KC_LALT,  KC_SPACE, KC_M,  KC_LALT,  XXXXXXX,                   XXXXXXX, KC_ENT,  XXXXXXX, KC_DEL,  KC_PSCR
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_SHCT):
            if (record->tap.count && record->event.pressed) {
                set_oneshot_mods(MOD_BIT(KC_LCTRL));
                return false; 
            }
            break;
        case KC_AE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL | MOD_LALT));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL | MOD_LALT));
                unregister_code(KC_Z);
            }
            return false;
            break;
        case KC_OE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL | MOD_LALT));
                register_code(KC_L);
            } else {
                unregister_mods(mod_config(MOD_LCTL | MOD_LALT));
                unregister_code(KC_L);
            }
            return false;
            break;
        case KC_AA:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL | MOD_LALT));
                register_code(KC_W);
            } else {
                unregister_mods(mod_config(MOD_LCTL | MOD_LALT));
                unregister_code(KC_W);
            }
            return false;
            break;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
            break;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
            break;
    }
    return true;
}
