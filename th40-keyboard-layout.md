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

### ZXCV Shift

Shifted bottom row left for better ergonomics (matches Colemak DH philosophy):

```
Default:  │/?│ Z │ X │ C │ V │ B │...
Proposed: │ Z │ X │ C │ V │/?│ B │...
```

This puts /? under left index finger (comfortable stretch position).

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

### Symbol Placement Rationale

**Right Hand Home Row (Power Positions):**
- J: `=` (most frequent in code, prime position)
- K: `(`
- L: `)`
- H: `+` (collocation with `=`)
- Enter: `'` (Shift+Enter = `"`)
- Backspace: `:` (Shift+Backspace = `;`)

**Left Hand Home Row:**
- A: `[` (close to Ctrl for Ctrl+])
- S: `]`
- D: `|` (pipe)
- F: `_` (underscore)
- G: empty

**Top Row (Mirrors Number Row Positions):**
- Q W E R T Y U I: `! @ # $ % ^ & *`
- O: `` ` `` (backtick)
- P: `~` (tilde)

**Bottom Row:**
- V position (old C): `;` (semicolon)
- N: `\` (backslash)
- M: `-` (minus - frequent)
- ,: `{` (matches `<` position)
- .: `}` (matches `>` position)
- B: empty (cursed key - most awkward)

**Big Keys:**
- Tab: Esc
- Backspace: `:` (colon) - semicolon on separate key (V position), VIA can't do custom shift
- Enter: `'` / `"` (shift variant)

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

## Key Design Decisions

### Modifiers That Stay Constant Across All Layers
- Left Shift = Shift
- Right Cmd = Cmd
- Caps Lock = Ctrl
- Left Ctrl/Opt/Cmd = unchanged (unreachable with left thumb on layer key)

### Modifiers Free to Remap on Layers
- Right Shift
- Right bottom row keys

### Layer Access
- Left Space = MO(1) Symbols (left thumb)
- Middle Fn = MO(2) Numbers (left thumb or right index)
- Right Ctrl = QWERTY/Colemak toggle (not momentary):
  - On Layer 0: DF(3) → switch to Colemak
  - On Layer 1: MW_CH → Mac/Windows toggle (escape from Mac mode)
  - On Layer 2: (transparent)
  - On Layer 3: DF(0) → switch back to QWERTY

### Why Right-Hand Heavy Symbols
- Left hand is modifier-heavy (Ctrl+Space for tmux, etc.)
- Right hand already accustomed to symbols from standard layout
- Keeps important symbols accessible with left hand free for modifiers

---

## Symbol Frequency Tiers (For Reference)

**S Tier (Must be ergonomic):**
- `:` - Vim command mode, typing
- `;` - repeat f/t motion
- `'` `"` - quotes everywhere
- `/` - search (on base layer)

**A Tier (High frequency):**
- `$` `^` - Vim line navigation
- `%` - bracket matching
- `-` - line navigation, typing
- `( )` `[ ]` `{ }` - brackets

**B Tier (Moderate):**
- `_ = +` - typing, operators
- `*` - search word
- `` ` `` - markdown, template strings
- `|` - tmux, pipes

**C Tier (Low):**
- `!` - command line, end of sentences
- `@` - macros
- `& ~ #` - occasional use

---

## Current Working Setup (As of 2025-01-02)

- Layer 0: QWERTY base (working)
- Layer 1: Symbols (working)
- Layer 2: Numbers + Navigation + Volume + System (BT, RGB, battery)
- Layer 3: Colemak (BROKEN - DF(3) breaks MO() layers, see below)
- Right Ctrl toggles QWERTY↔Colemak (DF(0)/DF(3))
- MW_CH on Right Ctrl in Layer 1 (escape hatch from Mac mode)

