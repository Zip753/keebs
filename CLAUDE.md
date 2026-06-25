# Repo Context

Custom QMK firmware setup for EPOMAKER TH40 keyboard.

## Key Files

- `README.md` - setup instructions, build commands
- `TODO.md` - outstanding tasks, known issues
- `th40/default/keymap.c` - the actual keymap source (with inline comments)
- `qmk_firmware/` - carlosedp's QMK fork (gitignored, clone separately)
- `qmk_firmware/keyboards/epomaker/th40/readme.md` - hardware info, bootloader docs

## ASCII Layout Template

Use this format for visualizing TH40 layers. Top line shows key sizes, bottom shows key labels.

```
┌─────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬─────┐
│1.5u │1u │1u │1u │1u │1u │1u │1u │1u │1u │1u │1.5u │  Row 1
│ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │Bksp │
├──────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────────┤
│1.75u │1u │1u │1u │1u │1u │1u │1u │1u │1u │ 2.25u  │  Row 2
│ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ Enter  │
├────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬──────┤
│1.25│1u │1u │1u │1u │1u │1u │1u │1u │1u │1u │1.75u │  Row 3
│Shft│ Z │ X │ C │ V │ B │/? │ N │ M │ , │ . │Shift │
├────┬───┬────┬────────┬───┬──────────┬────┬───┬────┤
│1.25│1u │1.25│ 2.25u  │1u │  2.75u   │1.25│1u │1.25│  Row 4
│Ctrl│Opt│ Cmd│ LSpace │Fn │  RSpace  │ Cmd│Opt│Ctrl│
└────┴───┴────┴────────┴───┴──────────┴────┴───┴────┘
```

Row 3 uses ZXCV-shifted layout: /? keycap moved between B and N (from TH40's quirk position next to left shift).
