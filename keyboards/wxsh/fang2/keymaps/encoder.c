
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    } else if (index == 1) {
        if(get_mods()) {
            if (clockwise) {
                tap_code(KC_TAB);
            } else {
                tap_code16(S(KC_TAB));
            }
        } else{
            if (clockwise) {
                tap_code(KC_PGDOWN);
            } else {
                tap_code(KC_PGUP);
            }
        }
    }
    return true;
}
