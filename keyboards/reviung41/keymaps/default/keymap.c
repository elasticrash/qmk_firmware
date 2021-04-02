/* Copyright 2020 gtips
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

enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST
};

#define LOWER  MO(_LOWER)
#define RAISE  MO(_RAISE)
#define ADJUST MO(_ADJUST)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_reviung41(
    KC_ESC,               KC_Q,     KC_W,     KC_E,      KC_R,      KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
    LT(_ADJUST, KC_TAB),  KC_A,     KC_S,     KC_D,      KC_F,      KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    KC_LCTL,              KC_Z,     KC_X,     KC_C,      KC_V,      KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_T(KC_ENT),
                                              KC_LALT,   RAISE,     SFT_T(KC_SPC),   LOWER,    KC_RSFT
  ),

  [_LOWER] = LAYOUT_reviung41(
    _______,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,    KC_PERC,            KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  KC_DEL,
    _______,  KC_UNDS,  KC_PLUS,  KC_LCBR,  KC_RCBR,   KC_PIPE,            KC_LEFT,  KC_MINS,  KC_EQL,   KC_UNDS,  KC_PERC,  KC_PLUS,
    _______,  KC_ESC,   KC_LGUI,  KC_LALT,  KC_CAPS,   KC_DQUO,            KC_HOME,  KC_END,   KC_COMM,  KC_DOT,   KC_NUBS, KC_MINS,
                                            _______,   _______,  KC_ENT,   _______,  _______
  ),

  [_RAISE] = LAYOUT_reviung41(
    _______,  KC_1,     KC_2,     KC_3,     KC_4,      KC_5,               KC_6,     KC_7,     KC_8,     KC_9,      KC_0,     KC_DEL,
    _______,  KC_MINS,  KC_EQL,   KC_LBRC,  KC_RBRC,   KC_BSLS,            KC_F1,    KC_F2,    KC_EQL,   KC_UNDS,   KC_PERC,  KC_PLUS,
    _______,  KC_ESC,   KC_RGUI,  KC_RALT,  KC_CAPS,   KC_QUOT,            KC_F7,    KC_F8,    KC_LBRC,  KC_RBRC,   KC_F11,   KC_F12,
                                            _______,   _______,  KC_BSPC,  _______,  _______
  ),

  [_ADJUST] = LAYOUT_reviung41(
    RGB_VAI,   RGB_SAI, RGB_HUI,  KC_7,  KC_8,   KC_9,            XXXXXXX,  XXXXXXX,  KC_UP,    XXXXXXX,  XXXXXXX,  KC_DEL,
    RGB_VAD,   RGB_SAD, RGB_HUD,  KC_4,  KC_5,   KC_6,            XXXXXXX,  KC_LEFT,  KC_DOWN,  KC_RGHT,  XXXXXXX,  XXXXXXX,
    XXXXXXX,   XXXXXXX, XXXXXXX,  KC_1,  KC_2,   KC_3,            XXXXXXX,  XXXXXXX,  KC_LCBR,  KC_RCBR,  XXXXXXX,  XXXXXXX,
                                                 KC_0,  _______,  XXXXXXX,  _______,  _______
  ),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_T(KC_SPC):
            return TAPPING_TERM + 50;
        default:
            return TAPPING_TERM;
    }
}
