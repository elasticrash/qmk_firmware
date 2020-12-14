/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

char wpm_str[10];
//char cckey_str[20];
//char cckey_buffer[10];
int position = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-----------------------------------------.                              ,-------------------------------------------.
 * |   ESC|   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  | \   |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  mo3 |   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | Ctrl |   Z  |   X  |   C  |   V  |   B  |      |      |  |      |      |   N  |   M  | ,  < | . >  | /  ? |  sfent |
 * `--------------------+------+------+------+      +      |  |      +      +------+------+------+----------------------'
 *                      | Alt  | Tab  | mo2  |LShift| Space|  | Space|RShift| mo1  | lguit| bsp  |
 *                      |      |      |      |      |      |  |      |      |      | (bsp)|      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      KC_ESC,  KC_Q,   KC_W,   KC_E,    KC_R,    KC_T,                                          KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_PIPE,
      MO(3),   KC_A,   KC_S,   KC_D,    KC_F,    KC_G,                                          KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LCTRL,KC_Z,   KC_X,   KC_C,    KC_V,    KC_B,  KC_LSFT, KC_SPACE, KC_SPACE, KC_RSFT, KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,
                               KC_LALT, KC_TAB,  MO(2), KC_LSFT, KC_SPACE, KC_SPACE, KC_RSFT, MO(1),  LGUI_T(KC_BSPACE), KC_BSPACE
    ),
/*
 * Lower Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  @   |  #   |  $   |  %   |                              |  ^   |   &  |  *   |  (   |  )   |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   {  |   }  |  (   |  )   |  `   |                              |   +  |  -   |  =   |  _   |  %   |   +    |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  |   |  ^   |  [   |  ]   |  ~   |      |      |  |      |      |   &  |  =   |  ,   |  .   |  \   | - _    |
 * `----------------------+------+------+------+      +      |  |      +      +------+------+------+----------------------'
 *                        |      |      |      |  ;   |  =   |  |  =   |  ;   |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_LOWER] = LAYOUT(
      _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR, KC_PERC,                                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSLS,
      _______, KC_LCBR, KC_RCBR,  KC_LPRN, KC_RPRN, KC_GRV,                                     KC_PLUS, KC_MINS, KC_EQL,  KC_UNDS, KC_PERC, KC_PLUS,
      _______, KC_PIPE, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, _______, _______, _______, _______, KC_AMPR, KC_EQL,  KC_COMM, KC_DOT,  KC_NUBS, KC_MINS,
                                 _______, _______, _______, KC_SCLN, KC_EQL,  KC_EQL,  KC_SCLN, _______, _______, _______
    ),
/*
 * Raise Layer: Number keys, media, navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |play/stp|   1  |  2   |  3   |  4   |  5   |                              |  6   |  7   |  8   |  9   |  0   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next | VolUp|                              | Left | Down | Up   | Right|      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      | Mute | VolDn|      |      |  |      |      | MLeft| Mdown|  [   |  ]   |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RAISE] = LAYOUT(
      KC_MPLY, KC_1, 	  KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
      _______, _______, _______, _______, KC_MUTE, KC_VOLD, _______, _______, _______, _______, KC_MS_L, KC_MS_D, KC_LBRC, KC_RBRC, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |                              | F6   | F7   |  up  | F9   | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | TOG  | SAI  | HUI  | VAI  | MOD  |                              |      |  lf  |  dn  | rgt  | F12  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  | RMOD |      |      |  |      |      |      |      |  {   |  }   |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_UP,   KC_F9,   KC_F10,  _______,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                     _______, KC_LEFT, KC_DOWN, KC_RGHT,  KC_F12,  _______,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, KC_DEL, _______, _______, KC_LCBR, KC_RCBR, _______, _______,
                                 _______, _______, _______, _______, _______, _______, KC_DEL, _______, _______, KC_DEL
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

// layer_state_t layer_state_set_user(layer_state_t state) {
//     return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
// }

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_el_logo(void) {
      oled_write_P(PSTR("[elasticrash ~(^_^)~]\n"), false);
}

static void render_layer_left(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("ESC Q W E R T\n"), false);
            oled_write_P(PSTR("LR3 A S D F G\n"), false);
            oled_write_P(PSTR("CTR Z X C V B\n"), false);
            oled_write_P(PSTR("    ALT TAB LR2 SH SP\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("__  1 2 3 4 5\n"), false);
            oled_write_P(PSTR("__  _________\n"), false);
            oled_write_P(PSTR("__  _________\n"), false);
            oled_write_P(PSTR("    _________\n\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("__  ! @ # $ PER\n"), false);
            oled_write_P(PSTR("__  { } ( ) `\n"), false);
            oled_write_P(PSTR("__  | ^ [ ] ~\n"), false);
            oled_write_P(PSTR("    _____ ; = \n\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("RGB\n"), false);
            oled_write_P(PSTR("F Keys\n"), false);
            oled_write_P(PSTR("Arrows\n\n\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }
}

static void render_layer_right(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("    Y U I O P |\n"), false);
            oled_write_P(PSTR("    H J K L ; '\n"), false);
            oled_write_P(PSTR("    N M , . / EN\n"), false);
            oled_write_P(PSTR("SP SH LR1 GU BS\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("    6 7 8 9 0\n"), false);
            oled_write_P(PSTR("    _________\n"), false);
            oled_write_P(PSTR("    _________\n"), false);
            oled_write_P(PSTR("_____________\n\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("    ^ & * (  )  |\n"), false);
            oled_write_P(PSTR("    + - = _ PER +\n"), false);
            oled_write_P(PSTR("    & = , . SLS -\n"), false);
            oled_write_P(PSTR("= ; _______\n\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("RGB\n"), false);
            oled_write_P(PSTR("F Keys\n"), false);
            oled_write_P(PSTR("Arrows\n\n\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_el_logo();
        render_layer_left();
    } else {
        render_el_logo();
        render_layer_right();
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if(get_highest_layer(layer_state)==_ADJUST){
        if (clockwise) {
            tap_code16(C(KC_Y));
        } else {
            tap_code16(C(KC_Z));
        }
    } else {
    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    }
}
#endif
