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
