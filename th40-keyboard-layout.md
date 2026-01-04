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

### ZXCV Shift (Optional - To Try)

Shift bottom row left for better ergonomics (matches Colemak DH philosophy):

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

---

## Future Considerations

### Colemak Support (BLOCKED in VIA - Use QMK)
- VIA only supports 4 layers (0-3)
- DF(3) for Colemak breaks MO(1)/MO(2) - Epomaker firmware bug
- Problem: TH40 missing semicolon position = missing O in Colemak
- Solution: Remap Enter to O, move Enter to layer or elsewhere
- **Fix**: Use carlosedp's QMK fork (see QMK Plan below)

### Home Row Mods (Later)
- Tap = letter, Hold = modifier
- Significant adjustment, save for future

### Split Keyboard Transition
- This layout designed to build muscle memory for Corne/Totem
- One layer per thumb is the pattern
- Third layer via both thumbs (not possible on TH40)

---

## VIA Keycodes Reference

| Function | VIA Code |
|----------|----------|
| Layer 0 momentary | MO(0) |
| Layer 1 momentary | MO(1) |
| Layer 2 momentary | MO(2) |
| Layer 3 momentary | MO(3) |
| Set default layer | DF(n) |
| Transparent | KC_TRNS |
| No action | KC_NO |

---

## Original JSON Location

`/Users/ivan.nikulin/Downloads/EPOMAKER_TH40_QMK_version_JSON.JSON`

---

## QMK Plan (For Colemak + Full Control)

### Good News
- **carlosedp's fork has TH40 with FULL wireless support**
- Bluetooth, 2.4GHz, USB all work
- TH40 is in the "Tested" list - he owns one
- Custom keycodes work: MD_BLE1/2/3, MD_24G, MD_USB, MW_CH, QK_BAT, etc.
- More than 4 layers supported (QMK default is 16)
- No DF() + MO() bugs

### Repository
```
https://github.com/carlosedp/qmk_firmware
```

### Setup Steps

```bash
# 1. Install QMK CLI
brew install qmk/qmk/qmk

# 2. Setup with carlosedp's fork (downloads toolchain ~1GB)
qmk setup carlosedp/qmk_firmware

# 3. Verify TH40 exists
ls ~/qmk_firmware/keyboards/epomaker/th40/

# 4. Create your keymap
mkdir -p ~/qmk_firmware/keyboards/epomaker/th40/keymaps/ivan
cp ~/qmk_firmware/keyboards/epomaker/th40/keymaps/default/keymap.c \
   ~/qmk_firmware/keyboards/epomaker/th40/keymaps/ivan/

# 5. Edit keymap.c with your layout (see template below)

# 6. Compile
qmk compile -kb epomaker/th40 -km ivan

# 7. Flash (hold Esc while plugging in USB, or press reset button on back)
qmk flash -kb epomaker/th40 -km ivan
```

### Bootloader Mode
Two ways to enter:
1. **Bootmagic**: Hold Esc (top-left key) while plugging in USB
2. **Physical button**: Press reset button on back of PCB

### Default Keymap Structure (from carlosedp)
```c
[0] = LAYOUT_tkl_ansi(  // Windows base
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,             KC_ENT,
    KC_LSFT, KC_SLSH, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
    KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,  MO(2),   KC_SPC,           KC_RALT, KC_APP,  KC_RCTL
),
[1] = LAYOUT_tkl_ansi(  // Mac base (swapped GUI/ALT)
    ...
),
[2] = LAYOUT_tkl_ansi(  // Function layer (from layer 0)
    // Has: numbers, BT controls, RGB, arrows, MW_CH, etc.
),
[3] = LAYOUT_tkl_ansi(  // Function layer (from layer 1)
    ...
)
```

### Proposed Layer Architecture for Your Needs
```
Layer 0: QWERTY base (Mac)
Layer 1: Symbols (MO(1) on left space)
Layer 2: Numbers + Arrows + Volume + System (MO(2) on Fn)
Layer 3: Colemak base (DF(3) to switch, DF(0) to return via Right Ctrl)
```

### Matrix Reference (for keymap.c)
```
Row 0: [0,0]=Tab
Row 1: [1,1-11]=Q W E R T Y U I O P Bksp
Row 2: [2,0]=Caps [2,1-9]=A S D F G H J K L [2,10]=Enter
Row 3: [3,0]=LShift [3,1]=/ [3,2-10]=Z X C V B N M , . [3,11]=RShift
Row 4: [4,0]=Ctrl [4,2]=GUI [4,3]=Alt [4,4]=LSpace [4,6]=Fn [4,7]=RSpace [4,8-10]=RAlt App RCtrl
```

### Custom Keycodes Available
| Keycode | Function |
|---------|----------|
| MD_BLE1 | Bluetooth channel 1 |
| MD_BLE2 | Bluetooth channel 2 |
| MD_BLE3 | Bluetooth channel 3 |
| MD_24G  | 2.4GHz wireless mode |
| MD_USB  | USB wired mode |
| MW_CH   | Mac/Windows toggle |
| QK_BAT  | Battery status check |
| QK_WLO  | Windows key lock |
| EE_CLR  | Clear EEPROM |

### VIA Compatibility
The fork includes `VIA_Mapping_TH40.json` - after flashing QMK, you can still use VIA for quick tweaks. But with full QMK control, you might not need it.

### Risks
- Flashing replaces Epomaker firmware entirely
- Keep original firmware backup (download from Epomaker site)
- If something goes wrong, use bootloader mode to reflash
- Wireless behavior might differ slightly (sleep times, battery consumption)

### TODO
- [x] Install QMK CLI
- [x] Clone carlosedp fork
- [x] Create zip753 keymap based on current VIA layout
- [x] Add Colemak layer with Enter→O
- [ ] Add DEL_WORD_FWD macro (Option+Right then Option+Backspace) for consistent word-forward delete
- [x] Test compile
- [ ] Backup original firmware (download from Epomaker site)
- [x] Flash and test
- [ ] Iterate on layout
- [ ] Investigate USB mode not working (keys unresponsive in wired mode)
- [ ] Fix KC_A on left Ctrl position (layer 0) - VIA export artifact?

### QMK Status (2025-01-04)
- **Flashed**: carlosedp QMK fork, keymap `zip753`
- **Working**: Bluetooth (all 3 channels), RGB, battery indicator, Mac/Win toggle, Colemak layer
- **Broken**: USB wired mode - keys unresponsive, only BT works even with switch set to USB
- **Flashing method**: Hold Esc + plug in USB → mounts as "NO NAME" drive → copy .bin file
- **Repo**: `github.com/Zip753/keebs` with symlink `qmk_firmware/keyboards/epomaker/th40/keymaps/zip753` → `th40/default/`
- **Compile**: `qmk compile -kb epomaker/th40 -km zip753`

---

## Resources

- [VIA Web](https://usevia.app)
- [EPOMAKER TH40 Product Page](https://epomaker.com/products/epomaker-th40)
- [NotebookCheck TH40 Layout Guide](https://www.notebookcheck.net/Fixing-the-Epomaker-TH40-110-effort-for-a-40-mechanical-keyboard.916471.0.html)
- [Colemak Mods Symbols Layer](https://colemakmods.github.io/ergonomic-mods/symbols.html)
- [40% Keyboards Wiki](https://40s.wiki/how)
