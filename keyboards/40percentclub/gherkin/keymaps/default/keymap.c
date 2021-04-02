#include QMK_KEYBOARD_H

#define FN_A        LT(3, KC_A)
#define FN_V        LT(1, KC_V)
#define FN_B        LT(2, KC_B)
#define FN_Q        LT(4, KC_Q)
#define CTL_Z       CTL_T(KC_Z)
#define ALT_X       ALT_T(KC_X)
#define ALT_N       ALGR_T(KC_N)
#define CTL_M       RCTL_T(KC_M)
#define SFT_ENT     RSFT_T(KC_ENT)
#define SFT_SP      RSFT_T(KC_SPC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT_ortho_3x10(
    FN_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    FN_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_BSPC,
    CTL_Z,   ALT_X,   KC_C,    FN_V,    SFT_SP,  SFT_SP,  FN_B,    KC_N,    CTL_M,   SFT_ENT
  ),

  [1] = LAYOUT_ortho_3x10(
    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
    _______, _______, _______, _______, KC_DEL,  _______, _______, _______, _______, _______
  ),

  [2] = LAYOUT_ortho_3x10(
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,
    KC_F11,  KC_F12,  _______, _______, _______, _______, _______, _______, _______, KC_QUOT,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),


  [3] = LAYOUT_ortho_3x10(
    KC_ESC,  KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_MINS, KC_EQL,  KC_UP,   KC_RBRC, KC_BSLS,
    KC_TAB,  KC_BSLS, KC_SLSH, KC_SCLN, KC_GRV,  KC_COMM, KC_LEFT, KC_DOWN, KC_RGHT, KC_QUOT,
    _______, _______, _______, _______, _______, _______, _______, KC_COMM, KC_DOT,  _______
  ),

  [4] = LAYOUT_ortho_3x10(
    _______, _______, _______, _______, _______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
    KC_TAB,  _______, _______, _______, _______, KC_LABK, KC_RABK, KC_QUES, KC_COLN, KC_DQUO,
    _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END
  ),
};

void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.

  // Set our LED pins as output
  setPinOutput(D5);
  setPinOutput(B0);
}

void led_set_user(uint8_t usb_led) {
  if (IS_LED_ON(usb_led, USB_LED_NUM_LOCK)) {
    writePinLow(D5);
  } else {
    writePinHigh(D5);
  }

  if (IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) {
    writePinLow(B0);
  } else {
    writePinHigh(B0);
  }
}
