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

// Animation frame defaults
#define MIN_WALK_SPEED 10
#define MIN_RUN_SPEED 40
#define LUNA_SIZE 96 // 96-byte arrays for the little dog
#define LUNA_FRAMES 2
#define LUNA_FRAME_DURATION 200 // Number of ms per frame
#define FORMAT LALT(LSFT(KC_F))
#define TSKMGR LCTL(LSFT(KC_ESC))

uint32_t luna_anim_timer = 0;
uint32_t luna_anim_sleep = 0;
uint8_t luna_current_frame = 0;
static long int luna_oled_timeout = 50000;

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
      KC_ESC,  KC_Q,   KC_W,   KC_E,    KC_R,    KC_T,                                        KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_PIPE,
      LT(3, KC_TAB),   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LCTRL,KC_Z,   KC_X,   KC_C,    KC_V,    KC_B,  KC_ENT, KC_LSFT, KC_SPACE, KC_BSPACE, KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,
                               KC_LALT, KC_TAB,  MO(2), KC_ENT, KC_LSFT, KC_SPACE, KC_BSPACE, MO(1),  KC_LGUI, FORMAT
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
      _______, KC_LCBR, KC_RCBR,  KC_LPRN, KC_RPRN, KC_GRV,                                     KC_MINS, KC_MINS, KC_EQL,  KC_UNDS, KC_PERC, KC_TILD,
      _______, KC_PIPE, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, _______, _______, _______, _______, KC_PLUS, KC_EQL,  KC_COMM, KC_DOT,  KC_NUBS, KC_PIPE,
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
      KC_MPLY, KC_1, 	  KC_2,    KC_3,    KC_4,    KC_5,                                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU,                                     KC_PLUS, KC_MINS, KC_EQL,   KC_UNDS, _______, _______,
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
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                      KC_F6,   KC_F7,   KC_UP,   KC_F9,   KC_F10,  _______,
      _______, RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                                    _______, KC_LEFT, KC_DOWN, KC_RGHT,  KC_F11,  _______,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD,_______, _______, _______, KC_DEL, _______, _______, KC_LCBR, KC_RCBR, KC_F12, _______,
                                 _______, _______, _______, _______, _______, _______, KC_DEL, _______, TSKMGR, KC_DEL
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

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
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
#endif

static void render_luna_sit(void) {
    static const char PROGMEM sit[LUNA_FRAMES][3][LUNA_SIZE/3] = {
        {
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
            0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x68,0x10,0x08,0x04,0x03,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x02,0x06,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
            0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        },
        {
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
            0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0xe0,0x90,0x08,0x18,0x60,0x10,0x08,0x04,0x03,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
            0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        }
    };
    oled_set_cursor(7,5);
    luna_current_frame = (luna_current_frame + 1) % LUNA_FRAMES;
    oled_write_raw_P(sit[abs(1 - luna_current_frame)][0], LUNA_SIZE/3);
    oled_set_cursor(7,6);
    oled_write_raw_P(sit[abs(1 - luna_current_frame)][1], LUNA_SIZE/3);
    oled_set_cursor(7,7);
    oled_write_raw_P(sit[abs(1 - luna_current_frame)][2], LUNA_SIZE/3);
}

static void render_luna_walk(void) {
    static const char PROGMEM walk[LUNA_FRAMES][3][LUNA_SIZE/3] = {
        {
            {0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x90,0x90,0x90,0xa0,0xc0,0x80,0x80,
            0x80,0x70,0x08,0x14,0x08,0x90,0x10,0x10,0x08,0xa4,0x78,0x80,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x07,0x08,0xfc,0x01,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
            0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x18,0xea,0x10,0x0f,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x03,
            0x06,0x18,0x20,0x20,0x3c,0x0c,0x12,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        },
        {
            {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x20,0x20,0x40,0x80,0x00,0x00,0x00,
            0x00,0xe0,0x10,0x28,0x10,0x20,0x20,0x20,0x10,0x48,0xf0,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x1f,0x20,0xf8,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
            0x03,0x00,0x00,0x00,0x00,0x01,0x00,0x10,0x30,0xd5,0x20,0x1f,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,
            0x02,0x1c,0x14,0x08,0x10,0x20,0x2c,0x32,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        }
    };
    oled_set_cursor(7,5);
    luna_current_frame = (luna_current_frame + 1) % LUNA_FRAMES;
    oled_write_raw_P(walk[abs(1 - luna_current_frame)][0], LUNA_SIZE/3);
    oled_set_cursor(7,6);
    oled_write_raw_P(walk[abs(1 - luna_current_frame)][1], LUNA_SIZE/3);
    oled_set_cursor(7,7);
    oled_write_raw_P(walk[abs(1 - luna_current_frame)][2], LUNA_SIZE / 3);
}

static void render_luna_run(void) {
    static const char PROGMEM run[LUNA_FRAMES][3][LUNA_SIZE/3] = {
        {
            {0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x08,0xc8,0xb0,0x80,0x80,0x80,0x80,0x80,0x80,
            0x80,0x40,0x40,0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x01,0x02,0xc4,0xa4,0xfc,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc8,0x58,0x28,0x2a,0x10,0x0f,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x0e,0x09,0x04,0x04,0x04,0x04,0x02,0x03,0x02,0x01,0x01,
            0x02,0x02,0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00}
        },
        {
            {0x00,0x00,0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
            0x80,0x80,0x78,0x28,0x08,0x10,0x20,0x30,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x10,0xb0,0x50,0x55,0x20,0x1f,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,
            0x02,0x1e,0x20,0x20,0x18,0x0c,0x14,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        }
    };
    luna_current_frame = (luna_current_frame + 1) % LUNA_FRAMES;
    oled_set_cursor(7,5);
    oled_write_raw_P(run[abs(1 - luna_current_frame)][0], LUNA_SIZE/3);
    oled_set_cursor(7,6);
    oled_write_raw_P(run[abs(1 - luna_current_frame)][1], LUNA_SIZE/3);
    oled_set_cursor(7,7);
    oled_write_raw_P(run[abs(1 - luna_current_frame)][2], LUNA_SIZE / 3);
}

static void render_luna_bark(void) {
    static const char PROGMEM bark[LUNA_FRAMES][3][LUNA_SIZE/3] = {
        {
            {0x00,0xc0,0x20,0x10,0xd0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
            0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x80,0xc8,0x48,0x28,0x2a,0x10,0x0f,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
            0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        },
        {
            {0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
            0x40,0x2c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00},
            {0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0x48,0x28,0x2a,0x10,0x0f,0x20,0x4a,0x09,0x10},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
            0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
        }
    };
    luna_current_frame = (luna_current_frame + 1) % LUNA_FRAMES;
    oled_set_cursor(7,5);
    oled_write_raw_P(bark[abs(1 - luna_current_frame)][0], LUNA_SIZE/3);
    oled_set_cursor(7,6);
    oled_write_raw_P(bark[abs(1 - luna_current_frame)][1], LUNA_SIZE/3);
    oled_set_cursor(7,7);
    oled_write_raw_P(bark[abs(1 - luna_current_frame)][2], LUNA_SIZE / 3);
}

static void render_luna_sneak(void) {
    static const char PROGMEM sneak[LUNA_FRAMES][3][LUNA_SIZE/3] = {
        {
            {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,
            0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x1e,0x21,0xf0,0x04,0x02,0x02,0x02,0x02,0x03,0x02,0x02,0x04,
            0x04,0x04,0x03,0x01,0x00,0x00,0x09,0x01,0x80,0x80,0xab,0x04,0xf8,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x02,0x06,
            0x18,0x20,0x20,0x38,0x08,0x10,0x18,0x04,0x04,0x02,0x02,0x01,0x00,0x00,0x00,0x00}
        },
        {
            {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0xe0,0xa0,0x20,0x40,0x80,0xc0,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x3e,0x41,0xf0,0x04,0x02,0x02,0x02,0x03,0x02,0x02,0x02,0x04,
            0x04,0x02,0x01,0x00,0x00,0x00,0x04,0x00,0x40,0x40,0x55,0x82,0x7c,0x00,0x00,0x00},
            {0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,0x04,
            0x18,0x10,0x08,0x10,0x20,0x28,0x34,0x06,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00}
        }
    };
    luna_current_frame = (luna_current_frame + 1) % LUNA_FRAMES;
    oled_set_cursor(7,5);
    oled_write_raw_P(sneak[abs(1 - luna_current_frame)][0], LUNA_SIZE/3);
    oled_set_cursor(7,6);
    oled_write_raw_P(sneak[abs(1 - luna_current_frame)][1], LUNA_SIZE/3);
    oled_set_cursor(7,7);
    oled_write_raw_P(sneak[abs(1 - luna_current_frame)][2], LUNA_SIZE / 3);
}

void animate_luna(void) {

    void animation_phase(void) {
        if (get_mods() & (MOD_MASK_SHIFT)) { render_luna_bark(); }
        else if (get_mods() & (MOD_MASK_CAG)) { render_luna_sneak(); }
        else if (get_current_wpm() <= MIN_WALK_SPEED) { render_luna_sit(); }
        else if (get_current_wpm() <= MIN_RUN_SPEED) { render_luna_walk(); }
        else { render_luna_run(); }
    }

    // Animate on WPM, turn off OLED on idle
    if (get_current_wpm() != 000 || host_keyboard_led_state().caps_lock || get_mods() & (MOD_MASK_CSAG)) {
        oled_on();
        if (timer_elapsed32(luna_anim_timer) >LUNA_FRAME_DURATION) {
            luna_anim_timer = timer_read32();
            animation_phase();
        }
        luna_anim_sleep = timer_read32();
    } else {
        if (timer_elapsed32(luna_anim_sleep) > luna_oled_timeout) {
            oled_off();
        } else {
            if (timer_elapsed32(luna_anim_timer) >LUNA_FRAME_DURATION) {
                luna_anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }
        }
        animate_luna();
}
#endif
