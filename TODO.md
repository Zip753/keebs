# TODO

## Blocking
- [ ] Investigate USB mode not working (keys unresponsive in wired mode)

## Layout
- [ ] Iterate on layout
- [ ] Add DEL_WORD_FWD macro (Option+Right then Option+Backspace) for consistent word-forward delete
- [ ] Fix KC_A on left Ctrl position (layer 0) - VIA export artifact?
- [x] Move Colemak to Layer 1 (rearranged layer order)
  - Layers now: 0=QWERTY, 1=Colemak, 2=Symbols, 3=Numbers
  - Toggle: DF(1) on layer 0, DF(0) on layer 1
  - Future: Custom `DF_TOG` keycode for single-key toggle

## Maintenance
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

### VIA Save Not Working
- Corporate laptop security blocks file downloads
- Web VIA applies changes directly to keyboard EEPROM
- Standalone VIA doesn't detect keyboard on Mac
- Redux store extraction methods don't work
- Workaround: document layout manually (this file)
- TODO: Try exporting from personal PC

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
