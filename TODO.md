# TODO

## Blocking
- [ ] USB HID not working on Mac - see [USB_HID_ISSUE.md](USB_HID_ISSUE.md) for full analysis
  - Symptom: `data_len == 0` at `usbd_ep_start_write()` → ZLP sent instead of descriptor
  - Unknown: Is this root cause or symptom? ChibiOS works on other keyboards.
  - Next step: Trace ChibiOS layer (`get_descriptor_cb()` in hal_usb.c), not more ES32 LLD changes
  - Workaround: Use Bluetooth for typing, USB for power only

## Layout
- [ ] Iterate on layout
- [ ] Add DEL_WORD_FWD macro (Option+Right then Option+Backspace) for consistent word-forward delete
- [ ] Fix KC_A on left Ctrl position (layer 0) - VIA export artifact?
- [ ] Try `LT(2, KC_CAPS_WORD)` on left space — tap for Caps Word, hold for layer 2
  - `CAPS_WORD` auto-disables after one word, no manual toggle needed
  - `TAPPING_TERM` (default 200ms) controls tap vs hold threshold
  - Needs `CAPS_WORD_ENABLE = yes` in `rules.mk`
- [x] Move Colemak to Layer 1 (rearranged layer order)
  - Layers now: 0=QWERTY, 1=Colemak, 2=Symbols, 3=Numbers
  - Toggle: DF(1) on layer 0, DF(0) on layer 1
  - Future: Custom `DF_TOG` keycode for single-key toggle
- [ ] Colemak layer indicator: teal LED (Win Lock position) when Colemak is active
  - Requires `_user` → `_kb` fix in th40.c first (free up keymap-level callbacks)
  - Then add `default_layer_state_set_user()` or `rgb_matrix_indicators_user()` in keymap.c

## Maintenance
- [x] Flashing workflow established
  - Build on Mac: `qmk compile` (after setting defaults, see README)
  - Upload .bin to Google Drive
  - Download on iPhone, copy to "NO NAME" drive via Lightning adapter
  - Corporate security blocks direct Mac → keyboard copy
- [ ] Backup original firmware (download from Epomaker site)
- [ ] Set up keymap-drawer for SVG visualization
  - `pip install keymap-drawer`
  - `qmk c2json -kb epomaker/th40 -km zip753 -o keymap.json`
  - `keymap parse -q keymap.json > keymap.yaml`
  - `keymap draw keymap.yaml > keymap.svg`
  - No layer limit unlike VIA, good for 5+ layer setups

## Known Issues & Workarounds

### DF() + MO() Layer Issue (RESOLVED)
- Was: DF(3) for Colemak broke MO(1)/MO(2) - layers didn't activate
- Root cause: QMK layers must be in increasing order (higher layers override lower)
- Fix: Reordered layers so base layouts (0, 1) are below overlays (2, 3)
- MW_CH also removed - it hijacked layer 1 for Mac/Win mode we don't need

### VIA Not Working
- VIA can't detect keyboard at all - USB HID is broken (see Blocking)
- VIA requires raw HID over USB, which doesn't work
- Previously: corporate security blocked file downloads
- Workaround: document layout in source code, flash via iPhone

### Apostrophe Mid-Word
- `'` is on Layer 2 Enter - requires layer switch mid-word for contractions
- Acceptable tradeoff for now
- Alternative considered: put `'` on Right Alt key (tap)

### Caps Lock Delay (Firmware Issue)
- KC_CAPS and KC_LCAP both have built-in delay in Epomaker firmware
- Other keys (like KC_A) respond instantly on same position
- macOS `hidutil CapsLockDelayOverride` doesn't help - it's firmware-side
- **Fix requires**: QMK firmware (carlosedp fork)
- **Workaround**: Use Karabiner to map a different keycode (e.g., F13) to Caps Lock
- **Current status**: Living without it, Ctrl on Caps position is what matters

### ISO Layout Compatibility
- MacBook has ISO keyboard, using UK layout
- TH40 is ANSI, so backtick (KC_GRV) produces § on UK layout
- **Fix**: Use `KC_NUBS` for backtick, `S(KC_NUBS)` for tilde
- Works correctly with UK ISO input source

### Momentary LED Indicators Persist
- BT channel indicator (MD_BLE1/2/3), battery indicator (QK_BAT), etc. stay lit after releasing layer 3
- Only clears when background LED animation overwrites them
- If animation is slow/static, indicators linger noticeably
- Battery indicator especially persistent
- Likely needs explicit LED clear on layer release (custom code)

## Local qmk_firmware Edits (not tracked in git)

These changes are made to the gitignored `qmk_firmware/` folder and must be reapplied after re-cloning:

### keyboards/epomaker/th40/th40.c
- Changed all `_user` callbacks → `_kb` (matching geonixr2 fix `ece72520fa`)
- Added calls to `_user()` inside each so keymaps can override
- **Reason**: QMK convention — `_kb` for keyboard level, `_user` for keymap level

### th40/default/keymap.c (symlinked as Zip753)
- Added `#include "keyboard_common.h"`
- Added `MD_USB` to layer 3, G key position (for testing USB mode switch)
- Added `keyboard_post_init_user()` with `rgb_matrix_mode_noeeprom(RGB_MATRIX_STARLIGHT_DUAL_SAT)`

## Future Considerations

### Colemak Support (READY)
- Layer order: 0=QWERTY, 1=Colemak, 2=Symbols, 3=Numbers
- DF(1)/DF(0) toggle on right Ctrl position
- TH40 missing semicolon position = Enter remapped to O in Colemak
- Needs testing after flashing

### Home Row Mods (Later)
- Tap = letter, Hold = modifier
- Significant adjustment, save for future

### Split Keyboard Transition
- This layout designed to build muscle memory for Corne/Totem
- One layer per thumb is the pattern
- Third layer via both thumbs (not possible on TH40)
