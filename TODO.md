# TODO

## Blocking
- [ ] Investigate USB mode not working (keys unresponsive in wired mode)

## Layout
- [ ] Iterate on layout
- [ ] Add DEL_WORD_FWD macro (Option+Right then Option+Backspace) for consistent word-forward delete
- [ ] Fix KC_A on left Ctrl position (layer 0) - VIA export artifact?

## Maintenance
- [ ] Backup original firmware (download from Epomaker site)
- [ ] Set up keymap-drawer for SVG visualization
  - `pip install keymap-drawer`
  - `qmk c2json -kb epomaker/th40 -km zip753 -o keymap.json`
  - `keymap parse -q keymap.json > keymap.yaml`
  - `keymap draw keymap.yaml > keymap.svg`
  - No layer limit unlike VIA, good for 5+ layer setups

## Known Issues & Workarounds

### DF(3) Breaks MO() Layers - BLOCKING COLEMAK
- Setting DF(3) for Colemak base layer works (alphas are Colemak)
- BUT MO(1) and MO(2) completely stop working
- No symbols, no numbers, nothing - layers don't activate at all
- This is likely an Epomaker firmware bug
- **Cannot use OS-level Colemak** because TH40 is missing the semicolon/O position
- Need Enter → O remap which requires keyboard-level Colemak
- **Fix requires**: Custom QMK firmware, not VIA

### Layer Default Problem
- Mac/Windows toggle (MW_CH) sets default layer via DF()
- Keyboard remembers this in EEPROM
- Moved MW_CH to accessible spot so can recover if boots to wrong layer
- Use Fn + Right Shift to toggle back to correct mode

### VIA Save Not Working
- Corporate laptop security blocks file downloads
- Web VIA applies changes directly to keyboard EEPROM
- Standalone VIA doesn't detect keyboard on Mac
- Redux store extraction methods don't work
- Workaround: document layout manually (this file)
- TODO: Try exporting from personal PC

### Apostrophe Mid-Word
- `'` is on Layer 1 Enter - requires layer switch mid-word for contractions
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

## Future Considerations

### Colemak Support (BLOCKED in VIA - Use QMK)
- VIA only supports 4 layers (0-3)
- DF(3) for Colemak breaks MO(1)/MO(2) - Epomaker firmware bug
- Problem: TH40 missing semicolon position = missing O in Colemak
- Solution: Remap Enter to O, move Enter to layer or elsewhere
- **Fix**: Use carlosedp's QMK fork

### Home Row Mods (Later)
- Tap = letter, Hold = modifier
- Significant adjustment, save for future

### Split Keyboard Transition
- This layout designed to build muscle memory for Corne/Totem
- One layer per thumb is the pattern
- Third layer via both thumbs (not possible on TH40)
