#include QMK_KEYBOARD_H

enum {
    TG_PRNT,
    TG_GITPL,
    TG_DKUP,
    TG_DKDWN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // 0: Base Layer
    LAYOUT_all(TG_PRNT, TG_GITPL, TG_DKUP, TG_DKDWN),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TG_GITPL:
            if (record->event.pressed) {
                SEND_STRING("git pull");
            } else {
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            break;
        case TG_DKUP:
            if (record->event.pressed) {
                SEND_STRING("docker-compose up --build");
            } else {
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            break;
        case TG_DKDWN:
            if (record->event.pressed) {
                SEND_STRING("docker-compose down");
            } else {
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            break;
        case TG_PRNT:
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
