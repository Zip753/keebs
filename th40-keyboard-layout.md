# EPOMAKER TH40 Keyboard Layout

## Quick Reference

```
Repo:     github.com/Zip753/keebs
Keymap:   th40/default/keymap.c (symlinked as zip753 in qmk_firmware)
Compile:  cd qmk_firmware && qmk compile -kb epomaker/th40 -km zip753
Flash:    Hold Esc + plug USB → copy .bin to "NO NAME" drive
Firmware: carlosedp QMK fork (bluetooth, RGB, battery all work)
```

### Current Status (2025-01-04)
- **Working**: Bluetooth (3 channels), RGB, battery, Mac/Win toggle, Colemak layer
- **Broken**: USB wired mode (needs investigation)
- **VIA**: Enabled in firmware, but requires USB to work

### Workflow for Changes
1. Edit `th40/default/keymap.c`
2. `qmk compile -kb epomaker/th40 -km zip753`
3. Flash via bootloader (Esc + plug in → copy .bin)

When USB is fixed: test in VIA first → sync changes back to keymap.c → reflash

---

## Physical Layout

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │Bksp │  Row 1
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│ Ctrl │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Enter  │  Row 2
├────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬──────┤
│Shft│ Z │ X │ C │ V │ B │/? │ N │ M │ , │ . │Shift │  Row 3 (ZXCV shifted)
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│Ctrl│Opt│ Cmd│  MO(1) │MO2│  Space   │ Cmd│Opt│ DF │  Row 4
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘
```

Note: Bottom row has ZXCV shift - slash is between B and N (not next to LShift).

---

## Layer 0: QWERTY Base (Mac)

| Key | Function |
|-----|----------|
| Caps Lock position | Ctrl |
| Left Space | MO(1) - Symbols |
| Middle Fn | MO(2) - Numbers/Nav |
| Right Space | Space |
| Right Ctrl | DF(3) - Toggle to Colemak |

---

## Layer 1: Symbols (Hold Left Space)

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│ Tab │ ! │ @ │ # │ $ │ % │ ^ │ & │ * │ ` │ ~ │  :  │
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│ Ctrl │ [ │ ] │ | │ _ │   │ + │ = │ ( │ ) │   '    │
├────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬──────┤
│Shft│   │   │   │ ; │   │   │ \ │ - │ { │ } │Shift │
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│    │Opt│ Cmd│ (held) │   │  Space   │Cmd │App│MW_CH│
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘
```

- Top row: `! @ # $ % ^ & *` + backtick/tilde (UK ISO: KC_NUBS)
- Home row: brackets `[ ] | _` and `+ = ( )` + apostrophe
- Bottom row: `; \ - { }`
- Right Ctrl: MW_CH (Mac/Windows toggle)

---

## Layer 2: Numbers + Navigation (Hold Fn)

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│ Esc │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │     │
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│ Ctrl │BT1│BT2│BT3│2.4│   │ ← │ ↓ │ ↑ │ → │ RGB    │
├────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬──────┤
│Shft│Mut│V- │V+ │   │   │   │W← │W⌫ │WDel│W→ │MW_CH │
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│EE  │WLO│ Alt│  Bat   │(h)│  Space   │Cmd │Opt│    │
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘
```

- Numbers on top row (numpad keycodes)
- HJKL = Arrow keys (Vim style)
- Volume: Mute/Down/Up on Z/X/C
- Word nav: Opt+Left/Bksp/Del/Right on N/M/,/.
- System: BT channels, RGB toggle, battery check

---

## Layer 3: Colemak-DH Base

- Standard Colemak-DH alpha layout
- Enter position = O (TH40 is missing semicolon position)
- Use Layer 2 for actual Enter
- Left Space = KC_ENT (not MO(1))
- Right Ctrl = DF(0) to return to QWERTY

---

## Custom Keycodes (carlosedp fork)

| Keycode | Function |
|---------|----------|
| MD_BLE1/2/3 | Bluetooth channels |
| MD_24G | 2.4GHz wireless |
| MD_USB | USB wired mode |
| MW_CH | Mac/Windows toggle |
| QK_BAT | Battery status |
| RM_TOGG | RGB toggle |
| EE_CLR | Clear EEPROM |

---

## TODO

- [ ] Investigate USB mode not working
- [ ] Fix KC_A on left Ctrl position (layer 0)
- [ ] Backup original Epomaker firmware
- [ ] Add DEL_WORD_FWD macro

---

## Resources

- [carlosedp QMK fork](https://github.com/carlosedp/qmk_firmware)
- [VIA Web](https://usevia.app)
- [Colemak Mods Symbols](https://colemakmods.github.io/ergonomic-mods/symbols.html)
