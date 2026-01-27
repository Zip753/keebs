# Design Decisions

## ZXCV Shift

Shifted bottom row left for better ergonomics (matches Colemak DH philosophy):

```
Default:  │/?│ Z │ X │ C │ V │ B │...
Proposed: │ Z │ X │ C │ V │/?│ B │...
```

This puts /? under left index finger (comfortable stretch position).

## Symbol Placement Rationale

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

## Key Design Decisions

### Modifiers That Stay Constant Across All Layers
- Left Shift = Shift
- Right Cmd = Cmd
- Caps Lock = Ctrl
- Left Ctrl/Opt/Cmd = unchanged (unreachable with left thumb on layer key)

### Modifiers Free to Remap on Layers
- Right Shift
- Right bottom row keys

### Layer Order

Layers are ordered with base layouts first, then overlays:
- Layer 0: QWERTY base
- Layer 1: Colemak-DH base
- Layer 2: Symbols
- Layer 3: Numbers + Navigation + System

This follows QMK's recommendation that higher layers take precedence,
so overlay layers (Symbols, Numbers) should be above base layers.

### Layer Access
- Left Space = MO(2) Symbols (left thumb)
- Middle Fn = MO(3) Numbers (left thumb or right index)
- Right Ctrl = QWERTY/Colemak toggle (not momentary):
  - On Layer 0: DF(1) → switch to Colemak
  - On Layer 1: DF(0) → switch back to QWERTY

### Removed: MW_CH (Mac/Windows Toggle)

The firmware's MW_CH key was designed for stock keymaps where:
- Layer 0 = Windows layout (Alt/Win in Windows positions)
- Layer 1 = Mac overlay (swaps Alt/GUI for Mac)

What MW_CH actually does: `layer_on(1)` for Mac mode, `layer_off(1)` for Windows.

**Problem:** Our keymap uses Layer 1 for Colemak, not Mac modifiers. Pressing MW_CH
would permanently enable the Colemak layer, causing a confusing "stuck" state.

**Why we don't need it:**
- Layer 0 already has Mac-correct modifier positions (Cmd where Cmd should be)
- No Windows-specific layout exists or is needed
- Mac-only user, Windows use is rare enough to not warrant a toggle

### Why Right-Hand Heavy Symbols
- Left hand is modifier-heavy (Ctrl+Space for tmux, etc.)
- Right hand already accustomed to symbols from standard layout
- Keeps important symbols accessible with left hand free for modifiers

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
