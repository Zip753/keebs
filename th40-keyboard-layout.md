# EPOMAKER TH40 Keyboard Layout

## Physical Layout Reference

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│1.5u │1u │1u │1u │1u │1u │1u │1u │1u │1u │1u │1.5u │  Row 1
│ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │Bksp │
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│1.75u │1u │1u │1u │1u │1u │1u │1u │1u │1u │ 2.25u  │  Row 2
│ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Enter  │
├────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬──────┤
│1.25│1u │1u │1u │1u │1u │1u │1u │1u │1u │1u │1.75u │  Row 3
│Shft│/?*│ Z │ X │ C │ V │ B │ N │ M │ , │ . │Shift │
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│1.25│1u │1.25│ 2.25u  │1u │  2.75u   │1.25│1u │1.25│  Row 4
│Ctrl│Opt│ Cmd│ LSpace │Fn │  RSpace  │ Cmd│Opt│Ctrl│
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘

* Note: /? is next to left shift (TH40 quirk)
```

---

## Layer 0: Base Layer (QWERTY)

### Changes from Default

| Key | Default | New |
|-----|---------|-----|
| Caps Lock | Caps Lock | Ctrl |
| Left Space | Space | MO(1) - Symbols layer |
| Middle Fn | Fn | MO(2) - Numbers layer |
| Right Space | Space | Space |
| Right Cmd (Alt key) | Alt | Cmd |
| Right Opt (App key) | App/Menu | Opt |
| Right Ctrl | Ctrl | QWERTY↔Colemak toggle (DF(3)/DF(0)), MW_CH on L1 |

---

## Layer 1: Symbols (Hold Left Space)

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│ Esc │ ! │ @ │ # │ $ │ % │ ^ │ & │ * │ ` │ ~ │  :  │
│ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │Bksp │
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│ Ctrl │ [ │ ] │ | │ _ │   │ + │ = │ ( │ ) │   '    │
│ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │  "(⇧)  │
├────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬──────┤
│Shft│   │   │ ; │   │   │   │ \ │ - │ { │ } │ Shift│
│    │ Z │ X │ C │ V │/? │ B │ N │ M │ , │ . │      │
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│Ctrl│Opt│ Cmd│ (held) │Fn │  Space   │Cmd │Opt│ Sys│
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘
```

---

## Layer 2: Numbers + Navigation (Hold Middle Fn)

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│     │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │     │
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│ Ctrl │   │   │   │   │   │ ← │ ↓ │ ↑ │ → │        │
├────┬───┬───┬───┬───┬───┬───┬───────┬──────┬───────┬──────┬──────┤
│Shft│Mut│V- │V+ │   │   │   │ Opt+← │Opt+⌫ │Opt+Del│Opt+→ │ Shift│
│    │ Z │ X │ C │ V │/? │ B │   N   │  M   │   ,   │  .   │      │
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│Ctrl│Opt│ Cmd│        │(h)│          │Cmd │Opt│    │
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘
```

- Numbers on top row (Q=1 through P=0)
- HJKL = Arrow keys (Vim style)
- Z = Mute, X = Vol Down, C = Vol Up (left hand)
- Word editing cluster (right hand, below arrows):
  - N = `LALT(KC_LEFT)` - word backward
  - M = `LALT(KC_BSPC)` - delete word backward
  - , = `LALT(KC_DEL)` - delete word forward (or `LALT(KC_D)` for terminal)
  - . = `LALT(KC_RGHT)` - word forward
- Keep Fn+Tab = Esc
- System stuff also lives here (BT channels, RGB, battery check, etc.)

---

## Layer 3: Colemak (Right Ctrl toggles between L0 QWERTY ↔ L3 Colemak)

- Colemak alpha layout
- Same layer access: MO(1) for symbols, MO(2) for numbers
- **CURRENTLY BROKEN**: DF(3) breaks MO() layers in Epomaker firmware
- Will work properly after QMK migration

---

## Current Working Setup (As of 2025-01-02)

- Layer 0: QWERTY base (working)
- Layer 1: Symbols (working)
- Layer 2: Numbers + Navigation + Volume + System (BT, RGB, battery)
- Layer 3: Colemak (BROKEN - DF(3) breaks MO() layers, see below)
- Right Ctrl toggles QWERTY↔Colemak (DF(0)/DF(3))
- MW_CH on Right Ctrl in Layer 1 (escape hatch from Mac mode)

