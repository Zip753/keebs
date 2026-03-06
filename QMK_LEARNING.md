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
- **Topic sections are the real notes** — after a session, the topic section is updated to reflect what was actually learned (progression, mental models, key insights). The session log stays as a lightweight timeline. Reading a topic section should be enough to rebuild understanding without replaying the conversation.

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

**Depth line for analog electronics:** Internalize pin modes (input/output, driving, reading), pull-ups/pull-downs, voltage/current/resistance at a practical level. Skip semiconductor physics (MOSFET internals, diode junctions) — not needed for QMK work. When a datasheet spec is unclear, ask "does this affect firmware behavior?" — if yes, look up the value; if no, move on.

---

## Part 2: Learning Path — "Life of a Keypress"

The central narrative traces a keypress from physical switch to USB output. Each step branches into a deeper concept area. This gives a natural reading order while allowing arbitrary deep dives.

### 1. Matrix Scanning `[~]`

*Physical switch → software event*

#### Electronics Fundamentals

**Voltage** is maintained by the power supply — 3.3V on VCC, 0V on GND. It exists whether or not current flows, like water level in a tower. **Current** flows when there's a path from high to low voltage; resistance limits how much (Ohm's law: I = V/R).

**GPIO pins** have two modes:
- **Output:** forces a voltage (HIGH = 3.3V, LOW = 0V) with very low impedance. "Driving" a pin.
- **Input:** senses voltage via MOSFET gates. Draws essentially zero current — the gate is insulated, only the electric field matters. A voltage comparator inside reports HIGH or LOW.

**Floating pin problem:** an input pin connected to nothing has no defined voltage. It picks up noise, reads random values, and puts internal transistors in a partial-conduction state that wastes power. **Pull-up/pull-down resistors** fix this by weakly connecting the pin to VCC or GND through a high-value resistor (~10kΩ–50kΩ), giving a default state that any external signal can easily overpower.

**Mental model for pull-up with a button:**
- Button open → pull-up holds pin at VCC → reads HIGH (default)
- Button closed → pin connects to GND through switch → GND wins over weak resistor → reads LOW
- Logic is inverted: pressed = LOW, released = HIGH

#### The Matrix

47 keys on 17 pins: **5 rows × 12 columns** (from `keyboard.json`). Row pins: B0, B3, B4, B5, B6. Column pins: D15, D14, C15, C14, C13, D3, D2, C12, C11, C10, A14, C9.

**Diode direction: ROW2COL** — columns are outputs, rows are inputs with internal pull-ups.

**Scan cycle** (runs ~1000–2000 times/sec in the main loop):
1. Drive one column LOW, all others stay HIGH
2. Read all 5 row pins — if a key connects that row to the LOW column, the row reads LOW (pressed)
3. Drive column HIGH again, move to next column
4. After all 12 columns: debounce the raw matrix → produce final `matrix[5]` array (12 bits per row)

**Why diodes:** all keys in a column share one wire. When multiple keys are pressed, closed switches can create unintended paths between columns through shared row wires, causing ghost keypresses. Each diode (row→col direction) blocks reverse current flow, preventing this.

**`MATRIX_UNSELECT_DRIVE_HIGH`:** when deselecting a column, actively drive it HIGH (near-zero impedance) instead of releasing to a passive pull-up. This charges parasitic capacitance fast, preventing ghost reads on the slow Cortex-M0.

**Debounce: `asym_eager_defer_pk`** — press registers immediately (eager), release waits for stable signal (deferred), tracked per-key. Like a synth envelope with zero attack and longer release.

#### The Main Loop

No OS scheduler, no threads, no event loop. Just `while (true)` running everything sequentially:
- `matrix_scan()` → `rgb_matrix_task()` → `mousekey_task()` → USB housekeeping → repeat
- If matrix unchanged (most iterations): skip keypress processing entirely
- RGB and other features have internal timers — called every loop but self-throttle
- Only concurrency: ChibiOS handles USB interrupts underneath (hardware-driven, pauses main loop briefly)

**Key files:**
- `keyboards/epomaker/th40/keyboard.json` — matrix pin definitions
- `quantum/matrix.c` — default scanning (TH40 uses standard, no custom matrix)
- `quantum/debounce/` — debounce algorithms
- `quantum/main.c` — the `while(true)` loop
- `quantum/keyboard.c` — `keyboard_task()` orchestrates scan + all feature tasks

**Still fuzzy:**
- [ ] Diode physics (semiconductor junctions — not needed for QMK, noted for curiosity)
- [ ] Exact scan timing and what `NOP_FUDGE 0.4` controls

---

### 2. Layers & Keycodes `[~]`

*Which keycode, from which layer*

#### Layer State: Two Bitmasks

Layer state is tracked in two separate bitmasks, combined during lookup:
- **`default_layer_state`** — which base layer is active. Set by `DF()`. Persists after key release. On TH40: `0b0001` (QWERTY) or `0b0010` (Colemak).
- **`layer_state`** — momentary overlays. Set by `MO()`, `LT()`, etc. Cleared on release. E.g. holding left space: `0b0100` (layer 2).

Active layers = `layer_state | default_layer_state`.

#### Lookup: Highest Layer Wins, KC_TRNS Falls Through

On keypress at `(row, col)`, QMK walks active layers **top to bottom**:
1. Check highest active layer → read `keymaps[layer][row][col]`
2. If `KC_TRNS` (transparent) → skip, check next layer down
3. First non-transparent keycode wins
4. If all layers transparent → falls back to layer 0. If that's also `KC_TRNS` → `ACTION_TRANSPARENT` → no-op (dead key).

#### Layer Key Types

- **`MO(n)`** — momentary. Sets bit in `layer_state` while held, clears on release. Our left space = `MO(2)`.
- **`DF(n)`** — default layer. Changes `default_layer_state`. Persists — it's switching the base, not an overlay. Our QWERTY↔Colemak toggle.
- **`LT(n, kc)`** — layer on hold, keycode on tap. `TAPPING_TERM` (default 200ms) decides: release within window = tap (sends `kc`), hold past window = layer (like `MO(n)`). We use `MO(2)` on left space but could use `LT(2, KC_CAPS_WORD)` for tap = caps word.
- **`OSL(n)`** — one-shot layer. Tap to activate layer for the **next** keypress only, then deactivates.

#### Source Layer Cache

Per-key cache (`source_layers_cache[row][col]`) stores which layer a key resolved from at **press time**. On release, the cached layer is used instead of re-resolving.

**Why:** If you hold MO(2), press a key (resolves on layer 2), then release MO(2) before releasing the key — without cache, the release would resolve on layer 0/1 and send the wrong keycode. Cache ensures press and release use the same layer.

**Granularity:** One entry per physical key. Written on press, read on release, overwritten on next press. Never explicitly invalidated.

#### TH40 Layer Stack

```
Layer 3: Numbers + Nav + System     ← MO(3) = hold Fn
Layer 2: Symbols                    ← MO(2) = hold left space
Layer 1: Colemak-DH base            ← DF(1)
Layer 0: QWERTY base                ← DF(0)
```

Only 0 or 1 active as default at a time. 2 and 3 overlay on top when held. If both held simultaneously, layer 3 wins (higher bit) with fallthrough to 2 for transparent positions.

**Key files:**
- `quantum/action_layer.c` — `layer_switch_get_layer()`: the top-down lookup loop
- `quantum/keymap_common.c` — `action_for_key()` and `action_for_keycode()`: keycode → action conversion
- `quantum/keymap_introspection.c` — `keycode_at_keymap_location()`: reads from `keymaps[]` in flash
- `quantum/action.c` — `store_or_get_action()`: cache read/write, bridges matrix → layer → keycode
- `th40/default/keymap.c` — our 4-layer keymap definition

---

### 3. The Callback Chain `[✓]`

*QMK's `_kb`/`_user` override pattern*

#### Weak Functions

`__attribute__((weak))` is a GCC linker feature: "use this implementation unless someone provides a strong (normal) one with the same name." QMK uses this instead of function pointers because the target audience is hobbyists writing first C — just define a function with the right name and it works. Also allows normal compile-time optimizations that function pointers would prevent.

#### The Three-Level Chain

```
QMK core calls _kb()  →  keyboard code (th40.c) calls _user()  →  keymap code (keymap.c)
```

No magic — each level **explicitly** calls the next. If `_kb` forgets to call `_user`, keymap code never runs. Convention, not framework enforcement.

Each `bool` callback can return:
- **`true`** — "I'm done, continue processing"
- **`false`** — "I consumed this, stop" (swallows the keypress)

Pattern in `_kb`: `if (!do_kb_stuff(...)) return false; return process_record_user(keycode, record);`

#### Callback Pairs (~15 total)

All follow the same `_kb` → `_user` pattern:
- **Init:** `keyboard_pre_init`, `keyboard_post_init`
- **Keypress:** `pre_process_record`, `process_record`, `post_process_record`
- **Layer:** `layer_state_set`, `default_layer_state_set`
- **LED/RGB:** `led_update`, `rgb_matrix_indicators`, `rgb_matrix_indicators_advanced`
- **Housekeeping:** `housekeeping_task`
- **Other:** `shutdown`, `encoder_update`, `connection_host_changed`

#### TH40 Bug (Fixed Locally)

`th40.c` shipped with `_user` callbacks where `_kb` should be — blocking keymap-level overrides. Our fix: rename to `_kb`, add explicit `_user()` calls inside. Same fix applied to Geonixr2 in commit `ece72520fa`.

**Key files:**
- `quantum/quantum.c` — weak default `_kb`/`_user` declarations
- `quantum/action.c` — `process_record()` entry point that starts the chain
- `keyboards/epomaker/th40/th40.c` — keyboard-level callbacks (fixed locally)
- `th40/default/keymap.c` — `keyboard_post_init_user()` sets RGB mode

---

### 4. Custom Keycodes & `process_record` `[~]`

*The processing gauntlet and how keycodes become actions*

#### The `&&` Chain in `process_record_quantum`

After the callback chain (`process_record_kb/user`) returns `true`, the keycode passes through ~30 feature processors in a short-circuit `&&` chain. Each returns `true` ("not mine") or `false` ("consumed, stop"). Order matters — `process_record_kb` runs before tap dance, caps word, auto shift, etc. If your `_user` function returns `false`, none of those later handlers see the keycode.

#### How Different Keycodes Resolve

**Simple key (`KC_A`):** Nobody in the chain claims it. Falls through to `process_action()` → `register_code(KC_A)` → `add_key()` sets bit in keyboard report → `host_keyboard_send()` sends USB HID report.

**Modifier + key (`S(KC_1)` = `!`):** Adds shift as a **weak mod** (temporary), sends report with shift, registers `KC_1`, then on release clears the weak mod. Strong mods (physical shift key) tracked separately in `real_mods` — they're OR'd together: `report.mods = real_mods | weak_mods | oneshot_mods`. No conflict if both active.

**Layer key (`MO(2)`):** `layer_on(2)` / `layer_off(2)`. Nothing sent to host — layers are internal state.

**Tap-hold (`LT(2, KC_SPC)`):** QMK can't decide immediately, so it **buffers** all subsequent key events. Decision: released within `TAPPING_TERM` (200ms) → tap (sends `KC_SPC`); term expires while held → hold (`layer_on(2)`). Buffered events flushed synchronously all at once after decision. Tuning: `PERMISSIVE_HOLD`, `HOLD_ON_OTHER_KEY_PRESS`, `CHORDAL_HOLD` change when the hold decision is made earlier for fast typists.

**TH40 wireless (`MD_BLE1`):** Intercepted in `process_record_kb` → `kb_process_record_common()`. Sends SPI command to wireless MCU, persists to EEPROM. Returns `true` (should arguably return `false`, but harmless since no later processor recognizes the keycode).

#### Key Functions

- **`register_code(kc)`** — press and hold: set bit in HID report, send report. Key stays "down" until unregister.
- **`unregister_code(kc)`** — release: clear bit, send updated report.
- **`tap_code(kc)`** — register + short delay + unregister. One keypress.
- **`SEND_STRING("text")`** — calls `tap_code()` per character.

#### USB HID Reports

Reports are **state snapshots**, not events: "these keys and mods are currently held." Sent every polling interval (~1ms USB). Host compares consecutive reports to detect changes. Self-healing — a lost report doesn't cause stuck keys.

Report struct: modifier bitmask + up to 6 keycodes (6KRO) or a full bitmap (NKRO).

**Key files:**
- `quantum/quantum.c` — `process_record_quantum()`: the `&&` chain of ~30 processors
- `quantum/action.c` — `process_action()`: executes the resolved action, `register_code()`/`unregister_code()`
- `quantum/action_util.c` — `send_keyboard_report()`: builds and sends HID report
- `quantum/action_tapping.c` — tap-hold state machine for LT/MT
- `lib/rdmctmzt_common/keyboard_common.c` — TH40 wireless keycodes (`kb_process_record_common`)
- `lib/rdmctmzt_common/rdmctmzt_common.h` — custom keycode enum (`MD_BLE1`, `MD_USB`, etc.)

**Still to explore:**
- [ ] Individual feature processors in depth (caps_word, tap_dance, auto_shift, leader, etc.) — trace when needed
- [ ] NKRO vs 6KRO report formats and when each is used
- [ ] `TAPPING_TERM` tuning in practice (after adding `LT()` to the keymap)

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

### Session 1 — 2026-03-06 — Matrix Scanning & Electronics Fundamentals
**Covered:**
- GPIO pins (input/output modes, driving, reading)
- Voltage vs current mental models (water level analogy)
- Floating pins, pull-up/pull-down resistors
- MOSFET gates — why input pins draw no current
- Full matrix scan cycle with TH40 specifics
- Ghosting and diode function
- `MATRIX_UNSELECT_DRIVE_HIGH` and why the Cortex-M0 needs it
- Debounce algorithm (`asym_eager_defer_pk`)
- QMK main loop structure — no concurrency, just `while(true)`

**Clicked:**
- Voltage as water level, not pressure — it just exists on the rails
- Debounce as synth envelope (zero attack, longer release)
- Pull-up: button pressed = LOW (inverted logic)
- "Pulled" = same word for resistor (weak) and driver (strong)

**Open questions:**
- Diode semiconductor physics (low priority, curiosity)
- `NOP_FUDGE` timing details

**Resources used:**
- [SparkFun: Pull-up Resistors](https://learn.sparkfun.com/tutorials/pull-up-resistors/all)
- [FreeCodeCamp: Pull-down and Pull-up Resistors](https://www.freecodecamp.org/news/a-simple-explanation-of-pull-down-and-pull-up-resistors-660b308f116a/)
- [SparkFun: Logic Levels](https://learn.sparkfun.com/tutorials/logic-levels/all)

**Confidence updates:**
- Matrix Scanning: `[?]` → `[~]`

### Session 1b — 2026-03-06 — Layers & Keycodes
**Covered:**
- Two bitmasks: `default_layer_state` (DF) vs `layer_state` (MO/LT)
- Top-down layer resolution with KC_TRNS fallthrough
- MO vs LT vs DF vs OSL — when to use each
- Source layer cache: per-key, written on press, read on release
- Traced full resolution path: matrix position → layer lookup → keycode → action

**Clicked:**
- DF() is persistent (changes base), MO() is momentary (overlay) — different bitmasks
- LT() = MO() + tap keycode, decided by TAPPING_TERM timeout
- Cache prevents wrong-layer release when MO() deactivates before key release

**Ideas spawned:**
- `LT(2, KC_CAPS_WORD)` on left space — added to TODO.md

**Confidence updates:**
- Layers & Keycodes: `[~]` → `[~]` (was already surface, now solid on mechanics. Full `[✓]` after using LT/OSL in practice.)

### Session 1c — 2026-03-06 — Callback Chain
**Covered:**
- Weak functions (`__attribute__((weak))`) — linker-time override, strong wins
- Three-level chain: core → `_kb` (keyboard) → `_user` (keymap), explicit calls, no magic
- `return false` to swallow keypresses (e.g. wireless keycodes handled at `_kb` level)
- Full list of ~15 callback pairs
- Why the th40.c `_user`/`_kb` bug exists and how our fix works

**Clicked:**
- The chain is convention, not framework — _kb must explicitly call _user
- Weak functions chosen for simplicity (hobbyist audience) and optimization, not because they're the "right" pattern

**Confidence updates:**
- Callback Chain: `[~]` → `[✓]`

### Session 1d — 2026-03-06 — Custom Keycodes & process_record
**Covered:**
- The ~30-processor `&&` chain in `process_record_quantum` — order, short-circuit
- Five keycode paths traced: simple (KC_A), mod+key (S(KC_1)), layer (MO), tap-hold (LT), wireless (MD_BLE1)
- register_code vs tap_code vs SEND_STRING
- Weak mods vs real mods vs oneshot mods — OR'd in report, no conflict
- USB HID reports as state snapshots (self-healing, not events)
- Tap-hold buffering: events held until decision, flushed synchronously
- PERMISSIVE_HOLD / HOLD_ON_OTHER_KEY_PRESS / CHORDAL_HOLD tuning

**Clicked:**
- HID reports are state-based for reliability (like game netcode)
- Tap-hold buffering means keys resolve against the decided layer, not the layer at press time
- The `&&` chain is just a priority list — first processor to return false wins

**Confidence updates:**
- Custom Keycodes & process_record: `[~]` → `[~]` (mechanics clear, but ~30 individual processors are a deep well to explore later)
