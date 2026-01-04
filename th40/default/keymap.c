#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Layer 0: QWERTY base (Mac)
    [0] = LAYOUT_tkl_ansi(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,             KC_ENT,
        KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
        KC_LCTL, KC_LALT, KC_LGUI,                   MO(1),   MO(2),   KC_SPC,           KC_RGUI, KC_RALT, DF(3)
    ),

    // Layer 1: Symbols (hold left space)
    [1] = LAYOUT_tkl_ansi(
        KC_ESC,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_NUBS, S(KC_NUBS), S(KC_SCLN),
        KC_LCTL, KC_LBRC, KC_RBRC, KC_PIPE, KC_UNDS, KC_NO,   KC_PLUS, KC_EQL,  KC_LPRN, KC_RPRN,             KC_QUOT,
        KC_LSFT, KC_NO,   KC_NO,   KC_NO,   KC_SCLN, KC_NO,   KC_NO,   KC_BSLS, KC_MINS, S(KC_LBRC), S(KC_RBRC), KC_RSFT,
        KC_LCTL, KC_LALT, KC_LGUI,                   KC_TRNS, KC_TRNS, KC_SPC,           KC_RGUI, KC_RALT, MW_CH
    ),

    // Layer 2: Numbers + Navigation (hold Fn)
    [2] = LAYOUT_tkl_ansi(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS,
        KC_LCTL, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,          KC_ENT,
        KC_LSFT, KC_NO,   KC_MUTE, KC_VOLD, KC_VOLU, KC_NO,   KC_NO,   LALT(KC_LEFT), LALT(KC_BSPC), LALT(KC_DEL), LALT(KC_RGHT), KC_RSFT,
        KC_LCTL, KC_LALT, KC_LGUI,                   KC_TRNS, KC_TRNS, KC_TRNS,          KC_RGUI, KC_RALT, KC_TRNS
    ),

    // Layer 3: Colemak base (Enter = O, use L2 for actual Enter)
    [3] = LAYOUT_tkl_ansi(
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
        KC_LCTL, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,             KC_O,
        KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
        KC_LCTL, KC_LALT, KC_LGUI,                   MO(1),   MO(2),   KC_SPC,           KC_RGUI, KC_RALT, DF(0)
    ),

};
