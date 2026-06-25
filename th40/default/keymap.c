#include QMK_KEYBOARD_H
#include "rdmctmzt_common.h"
#include "keyboard_common.h"

const key_override_t lspace_key_override = ko_make_basic(MOD_MASK_SHIFT, MO(2), KC_SPC);

const key_override_t *key_overrides[] = {
	&lspace_key_override
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT_tkl_ansi(
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_COLN, KC_BSPC,
        KC_LCTL, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,             KC_O,
        KC_LALT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_NO,   KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH,
        KC_CAPS, KC_LALT, KC_LGUI,                   MO(2),   MO(3),   LSFT_T(KC_SPC),   KC_RGUI, KC_RALT, DF(1)
    ),

    [1] = LAYOUT_tkl_ansi(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
        KC_LSFT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_SLSH, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT,
        KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, DF(0)
    ),

    [2] = LAYOUT_tkl_ansi(
        KC_ESC,  S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), KC_GRV,  KC_TILD, KC_SCLN,
        KC_TRNS, KC_LBRC, KC_RBRC, KC_PIPE, KC_UNDS, KC_NO,   KC_PLUS, KC_EQL,  S(KC_9), S(KC_0),          KC_QUOT,
        KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_BSLS, KC_MINS, KC_LCBR, KC_RCBR, KC_DQUO,
        KC_TRNS, KC_TRNS, KC_TRNS,                   KC_NO,   KC_NO,   KC_ENT,           KC_TRNS, KC_TRNS, KC_TRNS
    ),

    [3] = LAYOUT_tkl_ansi(
        KC_ESC,  KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,   KC_P6,   KC_P7,   KC_P8,   KC_P9,   KC_P0,   KC_TRNS,
        KC_TRNS, MD_BLE1, MD_BLE2, MD_BLE3, MD_24G,  MD_USB,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,          RM_TOGG,
        KC_TRNS, KC_MUTE, KC_VOLD, KC_VOLU, RM_VALD, RM_VALU, RM_NEXT, LALT(KC_LEFT), LALT(KC_BSPC), LALT(KC_DEL), LALT(KC_RGHT), KC_TRNS,
        EE_CLR,  QK_WLO,  KC_TRNS,                   QK_BAT,  KC_NO,   KC_SPC,           KC_TRNS, KC_TRNS, KC_TRNS
    ),

};

void keyboard_post_init_user(void) {
    kb_keyboard_post_init();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_DUAL_HUE);
}
