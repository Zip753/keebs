# QMK Learning

A structured guide for learning embedded development through the EPOMAKER TH40 keyboard's QMK firmware.

---

## Part 1: Meta

### Why — Motivation & Goals

- Learn embedded development through a real project, not contrived exercises
- Understand QMK deeply enough to fix bugs and contribute upstream
- Practice AI-assisted development in an unfamiliar domain — see how far prompting can take you
- Eventually: contribute to carlosedp's fork, maybe upstream QMK
- Long-term curiosity about digital electronics and embedded systems

### Where I'm Starting — Background

**What I know:**
- Software engineer, no embedded/firmware background
- Keymap structure: layers, keycodes, `keymap.c` layout macros
- Layer mechanics: `MO()`, `DF()`, `LT()`, transparent keys
- Build & flash workflow: `qmk compile`, upload via iPhone USB
- Have done practical debugging: USB HID register tracing, LED indicator hacking, `_kb`/`_user` callback chain fixes

**What I don't yet understand:**
- Hardware abstraction layers (ChibiOS HAL, LLD)
- MCU specifics (ES32F0654 — the chip in TH40)
- USB protocol internals (descriptors, endpoints, control transfers)
- SPI communication and wireless protocols (BLE, 2.4GHz)
- Memory constraints and optimization in embedded C
- How QMK's build system actually works (makefiles, platform selection)
- Interrupt handling and real-time considerations

### How — Session Format

- **Topic-driven**: pick a concept area, explore it with Claude's guidance
- **Mix of approaches**: explanation → code reading (QMK source + TH40) → questions to test understanding → hands-on modifications
- **Claude can quiz**: ask questions to check understanding, Socratic style
- **Grounded in real code**: every concept cross-referenced with TH40 implementation
- **Confidence levels per topic**: `[?]` unexplored → `[~]` surface understanding → `[✓]` solid grasp
- **Sessions logged** at the bottom — what was covered, what clicked, what needs revisit

### What — Scope & Resources

**Primary codebase:** `qmk_firmware/` (carlosedp's fork with wireless support)

**Key directories:**
| Path | Contents |
|------|----------|
| `keyboards/epomaker/th40/` | Keyboard-specific: `th40.c`, `config.h`, `rules.mk`, `keyboard.json` |
| `keyboards/epomaker/th40/keymaps/` | Keymap definitions (our code lives here as Zip753) |
| `lib/rdmctmzt_common/` | Wireless/three-mode library (non-standard, third-party origin) |
| `tmk_core/protocol/chibios/` | USB HID protocol layer (`usb_main.c`, `chibios.c`) |
| `lib/chibios-contrib/os/hal/ports/ES32/` | Hardware abstraction for ES32 MCU |
| `quantum/` | QMK core — keycodes, layers, RGB, process_record chain |

**External references:**
- QMK documentation: docs.qmk.fm
- The keymap (`th40/default/keymap.c`) as the familiar entry point
- ChibiOS documentation for RTOS and HAL concepts

**Constraints:** No time pressure — depth over breadth, follow curiosity.

---

## Part 2: Learning Path — "Life of a Keypress"

The central narrative traces a keypress from physical switch to USB output. Each step branches into a deeper concept area. This gives a natural reading order while allowing arbitrary deep dives.

### 1. Matrix Scanning `[?]`

*Physical switch → software event*

How does pressing a physical key turn into a digital signal? The keyboard's switches are arranged in a matrix (rows × columns) to reduce the number of GPIO pins needed. The firmware scans this matrix continuously.

**Key concepts:** GPIO pins, row/column scanning, debouncing, scan rate, ghosting/diodes

**Key files:**
- `keyboards/epomaker/th40/keyboard.json` — matrix pin definitions
- `quantum/matrix.c` — default matrix scanning implementation
- `quantum/debounce/` — debounce algorithms

**TH40 cross-reference:** How many rows/columns? Custom matrix or default? Scan rate?

**Understanding questions:**
- [ ] Why arrange switches in a matrix instead of one pin per key?
- [ ] What problem does debouncing solve? What happens without it?
- [ ] How does the scan rate affect typing feel?

---

### 2. Layers & Keycodes `[~]`

*Which keycode, from which layer*

Once a key position is identified, QMK determines which keycode to use based on the active layer stack. Layers are checked top-down; the first non-transparent keycode wins.

**Key concepts:** Layer stack, `KC_TRNS`, default layer vs active layers, `MO()`, `LT()`, `DF()`, basic vs quantum keycodes

**Key files:**
- `quantum/action_layer.c` — layer state management
- `quantum/keycode.h` — keycode definitions
- `quantum/quantum_keycodes.h` — quantum keycode extensions
- `th40/default/keymap.c` — our layer definitions

**TH40 cross-reference:** 4 layers (QWERTY, Colemak, Symbols, Numbers+Nav). How does `DF()` toggle between base layers?

**Understanding questions:**
- [ ] What's the difference between `MO()` and `LT()`? When would you use each?
- [ ] How does `KC_TRNS` affect the layer lookup? What if ALL layers are transparent for a position?
- [ ] What happens when two momentary layers are active simultaneously?

---

### 3. The Callback Chain `[~]`

*QMK's `_kb`/`_user` override pattern*

QMK uses a layered callback system: core → keyboard (`_kb`) → keymap (`_user`). This allows each level to override or extend behavior without modifying the level above.

**Key concepts:** Weak functions, `_kb` vs `_user` convention, `process_record` return values, initialization callbacks

**Key files:**
- `quantum/quantum.c` — core callback dispatch
- `keyboards/epomaker/th40/th40.c` — keyboard-level callbacks (has the `_user`→`_kb` bug)
- `th40/default/keymap.c` — our keymap-level callbacks

**TH40 cross-reference:** The TH40 firmware ships with `_user` callbacks in `th40.c` where `_kb` should be, blocking keymap-level overrides. We've fixed this locally. See `TODO.md` → "Local qmk_firmware Edits".

**Understanding questions:**
- [ ] Why does QMK use `__attribute__((weak))` functions?
- [ ] What happens if `process_record_kb` returns `false`?
- [ ] What's the full list of callbacks QMK exposes? (init, matrix scan, layer change, etc.)

---

### 4. Custom Keycodes & `process_record` `[~]`

*Extending the processing chain*

When a keycode is processed, it flows through `process_record_*` functions. Custom keycodes let you define entirely new behaviors — macros, mode switches, custom actions.

**Key concepts:** `process_record_user()`, custom keycode enum, tap vs hold, `SEND_STRING`, `register_code`/`unregister_code`

**Key files:**
- `quantum/process_keycode/` — built-in keycode processors
- `th40/default/keymap.c` — our custom keycodes (if any)
- `lib/rdmctmzt_common/keyboard_common.c` — custom keycodes for wireless (`MD_BLE1`, `MD_USB`, etc.)

**TH40 cross-reference:** The wireless mode keycodes (`MD_BLE1`, `MD_BLE2`, `MD_USB`, etc.) are custom keycodes processed in `keyboard_common.c`, not standard QMK.

**Understanding questions:**
- [ ] How does `process_record` decide the order of processing?
- [ ] What's the difference between `tap_code()` and `register_code()`/`unregister_code()`?
- [ ] How do TH40's wireless mode keycodes hook into the processing chain?

---

### 5. RGB Matrix `[~]`

*LED animations and per-key indicators*

QMK's RGB Matrix system drives per-key LEDs with animations. The TH40 uses this for both aesthetic effects and functional indicators (layer state, battery, BT channel).

**Key concepts:** RGB matrix vs RGB lighting, animation effects, `rgb_matrix_indicators_*`, HSV color model, EEPROM persistence

**Key files:**
- `quantum/rgb_matrix/` — RGB matrix core
- `quantum/rgb_matrix/animations/` — built-in effects
- `keyboards/epomaker/th40/th40.c` — indicator LED logic (currently in `_user`, should be `_kb`)
- `lib/rdmctmzt_common/user_led_custom.c` — custom LED control for wireless indicators

**TH40 cross-reference:** LED persistence bug — momentary indicators (BT channel, battery) stay lit after releasing layer 3. See `TODO.md` → "Momentary LED Indicators Persist".

**Understanding questions:**
- [ ] What's the difference between `rgb_matrix_indicators_kb` and `rgb_matrix_indicators_user`?
- [ ] How does `rgb_matrix_set_color()` interact with the current animation?
- [ ] Why do indicator LEDs persist? What controls when they're cleared?

---

### 6. USB HID `[~]`

*Reports, descriptors, the protocol layer*

The keyboard communicates with the host via USB Human Interface Device protocol. This involves device descriptors (telling the host what the device is), endpoints (data channels), and HID reports (the actual keystroke data).

**Key concepts:** USB enumeration, device/config/interface/HID/endpoint descriptors, EP0 control transfers, HID reports (6KRO/NKRO), boot protocol

**Key files:**
- `tmk_core/protocol/chibios/usb_main.c` — QMK's USB driver (init, restart, report sending)
- `tmk_core/protocol/usb_descriptor.h` — HID report descriptors
- `lib/chibios-contrib/os/hal/ports/ES32/LLD/USBv1/hal_usb_lld.c` — low-level USB driver for ES32

**TH40 cross-reference:** USB HID is broken on Mac — see `USB_HID_ISSUE.md` for the full investigation. The issue is in EP0 control transfers: `data_len == 0` causes a ZLP instead of sending the device descriptor. iPhone USB works (more tolerant stack).

**Understanding questions:**
- [ ] What happens during USB enumeration? What's the sequence of requests?
- [ ] What's the difference between EP0 (control) and other endpoints?
- [ ] Why would `data_len == 0` happen in the descriptor response?
- [ ] What makes Mac's USB stack stricter than iPhone's?

---

### 7. Wireless (BLE / 2.4GHz) `[?]`

*`rdmctmzt_common`, SPI, not standard QMK*

The TH40's wireless support is NOT part of standard QMK — it comes from the `rdmctmzt_common` library, likely reverse-engineered from vendor firmware. This handles Bluetooth LE, 2.4GHz dongle, and three-mode switching.

**Key concepts:** BLE HID profile, SPI communication with wireless module, three-mode state machine, pairing/bonding, battery reporting

**Key files:**
- `lib/rdmctmzt_common/three_mode.c` — mode switching logic (`es_restart_usb_driver`, `Usb_Disconnect`)
- `lib/rdmctmzt_common/keyboard_common.c` — main integration (init, mode detection, key processing)
- `lib/rdmctmzt_common/user_spi.c` — SPI communication with wireless module
- `lib/rdmctmzt_common/user_battery.c` — battery level monitoring
- `lib/rdmctmzt_common/rdmctmzt_common.c` — wireless common utilities

**TH40 cross-reference:** This library is the source of the USB HID bug — `es_restart_usb_driver()` bypasses ChibiOS, and `Usb_Disconnect()` does a hardware reset that wipes USB state. See `USB_HID_ISSUE.md` → "Monsgeek vs Epomaker" for comparison with a working approach.

**Understanding questions:**
- [ ] How does the keyboard decide which mode to use at boot?
- [ ] What role does SPI play in the wireless architecture?
- [ ] How does the three-mode state machine work? What triggers transitions?
- [ ] Where did this code come from? (QMK issue #24085, Finalkey/LiuLiuQMK)

---

### 8. Build System & Configuration `[?]`

*`config.h`, `rules.mk`, `keyboard.json`, compile flow*

QMK's build system determines which files to compile, what features to enable, and how to target the specific MCU. Configuration cascades from defaults through keyboard-level to keymap-level overrides.

**Key concepts:** `rules.mk` feature flags, `config.h` defines, `keyboard.json` hardware description, `qmk compile` flow, platform detection, MCU targeting

**Key files:**
- `keyboards/epomaker/th40/rules.mk` — feature flags, MCU selection
- `keyboards/epomaker/th40/config.h` — hardware configuration
- `keyboards/epomaker/th40/keyboard.json` — keyboard metadata and matrix
- `builddefs/` — QMK build system internals
- `platforms/chibios/` — ChibiOS platform integration

**TH40 cross-reference:** The TH40 uses the ES32F0654 MCU (not a common QMK target). The build pulls in ChibiOS-contrib for ES32 HAL support. Understanding this helps debug why standard QMK patterns may not work as expected.

**Understanding questions:**
- [ ] What does `qmk compile` actually do under the hood?
- [ ] How does `rules.mk` control which features are compiled in?
- [ ] What's the relationship between `keyboard.json` and `config.h`?
- [ ] How does the build system find the right HAL for the ES32 chip?

---

## Part 3: TH40 Audit

Collected findings on where the TH40 firmware diverges from QMK conventions or has issues.

### `_user`/`_kb` Callback Misuse

**Status:** Fixed locally, not upstreamed

`th40.c` defines callbacks as `_user` instead of `_kb`, which blocks keymap-level overrides. QMK convention: `_kb` for keyboard-level, `_user` for keymap-level. The Geonixr2 keyboard in the same fork had the same issue and was fixed in commit `ece72520fa`.

**Local fix:** Changed all `_user` callbacks to `_kb` in `th40.c`, added calls to `_user()` inside each.

### USB HID Broken on Mac

**Status:** Under investigation — see `USB_HID_ISSUE.md`

USB enumeration fails on Mac with `wMaxPacketSize 0x0000`. Root cause traced to `data_len == 0` in EP0 descriptor response (ZLP sent instead of descriptor data). iPhone USB works with the same firmware.

Key findings:
- `es_restart_usb_driver()` bypasses ChibiOS USB stack
- `Usb_Disconnect()` does hardware reset that wipes USB state
- Monsgeek keyboards in the same fork use ChibiOS-level disconnect and work
- 14 debug/fix iterations narrowed to ChibiOS layer issue
- Next step: trace `get_descriptor_cb()` in ChibiOS `hal_usb.c`

### LED Indicator Persistence Bug

**Status:** Known, not yet investigated

Momentary indicators (BT channel, battery) stay lit after releasing the layer 3 key. Only clears when background RGB animation overwrites them. Likely needs explicit LED clear on layer release.

### Non-Standard Wireless Library

**Status:** Structural concern

`lib/rdmctmzt_common/` is not part of standard QMK. Origin traced to third-party firmware provider (Finalkey/LiuLiuQMK). Code quality and conventions differ from QMK core. This affects:
- USB initialization (bypasses ChibiOS)
- Callback chain (uses `_user` instead of `_kb`)
- Build integration (non-standard library path)

### Comparison with Other Keyboards in Fork

**TODO:** Compare TH40 implementation with:
- Monsgeek keyboards (`keyboards/monsgeek/`) — working USB, cleaner wireless integration
- Geonixr2 — had same `_user`/`_kb` bug, already fixed
- Other Epomaker models in the fork

---

## Part 4: Session Log

*Running notes per session — what was explored, what clicked, open questions.*

<!-- Template:
### Session N — YYYY-MM-DD — Topic
**Covered:**
- ...
**Clicked:**
- ...
**Open questions:**
- ...
**Confidence updates:**
- Topic X: [?] → [~]
-->

*(No sessions yet)*
