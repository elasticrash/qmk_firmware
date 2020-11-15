#include QMK_KEYBOARD_H

enum {
    DEMO_PRNT,
    DEMO_GIT_PULL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // 0: Base Layer
    LAYOUT_all(DEMO_PRNT, DEMO_GIT_PULL, KC_V, KC_E),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DEMO_GIT_PULL:
            if (record->event.pressed) {
                SEND_STRING("git pull");
            } else {
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            break;
        case DEMO_PRNT:
            if (record->event.pressed) {
                register_code(KC_LGUI);
                register_code(KC_LSFT);
                register_code(KC_S);
            } else {
                unregister_code(KC_LGUI);
                unregister_code(KC_LSFT);
                unregister_code(KC_S);
            }
            break;
    }
    return true;
}

// Loop
void matrix_scan_user(void){
    // Empty
};
