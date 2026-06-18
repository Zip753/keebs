#include QMK_KEYBOARD_H
#include "rdmctmzt_common.h"
#include "keyboard_common.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Layer 0: Colemak-DH base
    // - KC_O on Enter position: TH40 missing semicolon key = missing O in Colemak
    // - Enter: LSpace + RSpace (layer 2 right space)
    // - DF(1): switch to QWERTY
    [0] = LAYOUT_tkl_ansi(
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
        KC_LCTL, KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,             KC_O,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_SLSH, KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_RSFT,
        KC_CAPS, KC_LALT, KC_LGUI,                   MO(2),   MO(3),   LSFT_T(KC_SPC),   KC_RGUI, KC_RALT, DF(1)
    ),

    // Layer 1: QWERTY base (Mac)
    // - ZXCV shift: slash moved between B and N (see DECISIONS.md)
    // - KC_A on bottom-left: testing caps lock behavior (TODO: fix)
    // - DF(0): switch back to Colemak layer
    [1] = LAYOUT_tkl_ansi(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,             KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_SLSH, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
        KC_A,    KC_LALT, KC_LGUI,                   MO(2),   MO(3),   KC_SPC,           KC_RGUI, KC_RALT, DF(0)
    ),

    // Layer 2: Symbols (hold left space)
    // - S(KC_*): shifted symbols (! @ # $ % ^ & *)
    // - KC_NUBS: backtick ` (ISO workaround for UK layout)
    // - S(KC_NUBS): tilde ~ (ISO workaround)
    // - S(KC_SCLN): colon : on backspace position
    // - KC_SCLN: semicolon ; on V position
    // - KC_ENT on right space: Enter for Colemak (LSpace + RSpace)
    [2] = LAYOUT_tkl_ansi(
        KC_TAB,  S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), KC_NUBS, S(KC_NUBS), S(KC_SCLN),
        KC_LCTL, KC_LBRC, KC_RBRC, S(KC_BSLS), S(KC_MINS), KC_NO, S(KC_EQL), KC_EQL, S(KC_9), S(KC_0),       KC_QUOT,
        KC_LSFT, KC_NO,   KC_NO,   KC_NO,   KC_SCLN, KC_NO,   KC_NO,   KC_BSLS, KC_MINS, S(KC_LBRC), S(KC_RBRC), KC_RSFT,
        KC_NO,   KC_LALT, KC_LGUI,                   KC_NO,   KC_NO,   KC_ENT,           KC_RGUI, KC_RALT, KC_RCTL
    ),

    // Layer 3: Numbers + Navigation + System
    // - Arrows: HJKL vim-style
    // - Word nav: LALT(KC_LEFT/RGHT) = word backward/forward
    // - Word del: LALT(KC_BSPC/DEL) = delete word backward/forward
    // - MD_BLE1/2/3: Bluetooth channels, MD_24G: 2.4GHz mode, MD_USB: USB mode
    // - QK_BAT: battery check, EE_CLR: clear EEPROM, RM_TOGG: RGB toggle
    // - RGB: RM_VALD/VALU = brightness down/up, RM_NEXT = next animation
    [3] = LAYOUT_tkl_ansi(
        KC_ESC,  KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,   KC_P6,   KC_P7,   KC_P8,   KC_P9,   KC_P0,   KC_NO,
        KC_LCTL, MD_BLE1, MD_BLE2, MD_BLE3, MD_24G,  MD_USB,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,          RM_TOGG,
        KC_LSFT, KC_MUTE, KC_VOLD, KC_VOLU, RM_VALD, RM_VALU, RM_NEXT, LALT(KC_LEFT), LALT(KC_BSPC), LALT(KC_DEL), LALT(KC_RGHT), KC_RSFT,
        EE_CLR,  QK_WLO,  KC_LALT,                   QK_BAT,  KC_NO,   KC_SPC,           KC_RGUI, KC_RALT, KC_NO
    ),

};

void keyboard_post_init_user(void) {
    kb_keyboard_post_init();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_DUAL_HUE);
}
