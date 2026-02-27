# TH40 Keyboard Setup

## Quick Reference

```
Keymap:   th40/default/keymap.c (symlinked as Zip753 in qmk_firmware)
Compile:  qmk compile
Flash:    Enter bootloader → copy .bin to "NO NAME" drive
```

## File Structure

```
keebs/
├── th40/                    # Keyboard-specific folder
│   └── default/             # Layout preset name
│       └── keymap.c         # Actual keymap source
├── qmk_firmware/            # carlosedp's QMK fork (gitignored)
│   └── keyboards/
│       └── epomaker/
│           └── th40/
│               └── keymaps/
│                   ├── default/     # Stock keymap from fork
│                   └── Zip753/      # Symlink → ../../../../../th40/default
└── README.md
```

QMK expects keymaps at `keyboards/<vendor>/<model>/keymaps/<name>/`. We keep our source in `th40/default/` and symlink it into QMK as `Zip753`.

## Local Setup

```bash
# Install QMK CLI
brew install qmk/qmk/qmk

# Clone carlosedp's fork (has TH40 wireless support)
git clone https://github.com/carlosedp/qmk_firmware.git

# Install QMK dependencies
qmk setup -H qmk_firmware

# Symlink your keymap into the QMK tree
ln -s ../../../../../th40/default qmk_firmware/keyboards/epomaker/th40/keymaps/Zip753

# Set default keyboard and keymap (so you can just run `qmk compile`)
qmk config user.keyboard=epomaker/th40
qmk config user.keymap=Zip753

# Verify
ls -la qmk_firmware/keyboards/epomaker/th40/keymaps/Zip753
```

## Building

```bash
qmk compile
# Output: qmk_firmware/epomaker_th40_Zip753.bin
```

## Flashing

Enter bootloader mode:
1. **Bootmagic**: Hold top-left key (Tab position) while plugging in USB
2. **Physical button**: Press reset button on back of PCB

Keyboard mounts as "NO NAME" drive. Copy the `.bin` file to flash.

## Firmware

Based on [carlosedp's QMK fork](https://github.com/carlosedp/qmk_firmware) which has full TH40 wireless support (Bluetooth, 2.4GHz, USB).

## Resources

- [VIA Web](https://usevia.app)
- [EPOMAKER TH40 Product Page](https://epomaker.com/products/epomaker-th40)
- [carlosedp QMK fork](https://github.com/carlosedp/qmk_firmware)
