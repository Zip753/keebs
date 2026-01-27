# USB HID Not Working on Mac

**Status**: CONFIRMED BUG in carlosedp fork - USB has NEVER worked on Mac with this port
**Workaround**: Use Bluetooth for typing, USB for power only

## Claude Instructions (READ THIS FIRST)

**Context management is critical.** User runs out of context constantly. Follow these rules:

1. **Use Task tool with Explore agent** for ANY code searching/reading. Never read entire files in main context.
2. **Read only specific line ranges** when you must read directly (e.g., `offset: 200, limit: 30`).
3. **Do ONE thing, update doc, STOP.** Don't chain multiple explorations.
4. **Capture logs via background commands**, grep for specific patterns, never dump raw logs into context.
5. **Update this doc BEFORE doing the next thing.** The doc is the persistent state.
6. **For physical tests**:
   - Present test plan clearly: starting state, actions user will perform, capture duration
   - User communicates via BLE (USB doesn't work), so start state is always BLE mode
   - Account for time needed: switch toggle + plug cable + enumeration = give 20+ seconds
   - Wait for user to say "go"
   - Run ONE atomic command that captures for N seconds and outputs results
   - User performs actions during capture window (no typing needed - USB doesn't work anyway)
   - Command auto-completes, Claude reads results
7. **Flash workflow**: Compile with `qmk compile` (NO FLAGS - uses defaults), flash via iPhone USB (Mac USB doesn't work), record SHA256.
8. **Never assume firmware state** - check the Firmware Version Tracking table below.
9. **USER NEVER RUNS COMMANDS** - Claude runs all bash commands. User only does physical actions (plug cable, press keys, flip toggle).
10. **PAUSE BEFORE TESTS** - Always present test plan and wait for user to say ready before executing capture commands.

## What Works vs What Doesn't

| Scenario | Result |
|----------|--------|
| Original Epomaker firmware + Mac USB | Works |
| Original Epomaker firmware + iPhone USB | Works |
| carlosedp QMK + Mac USB (boot in USB mode) | **FAILS** |
| carlosedp QMK + Mac USB (switch to USB via key) | **FAILS** |
| carlosedp QMK + iPhone USB | **Works** |
| carlosedp QMK + Mac Bluetooth | Works |

**Key insight**: iPhone USB works, Mac USB fails. Same firmware, same keyboard.
This points to Mac's stricter USB enumeration vs iPhone's more tolerant stack.

## Symptoms

- USB mode activates correctly (G LED lights on Fn layer)
- **Hardware confirmed OK**: Original Epomaker firmware had working USB

**Mac kernel logs show** (captured during boot with toggle=USB, 2026-01-28):
```
00:11:03 endpoint 0x00: status 0xe00002ed (transaction error): 0 bytes transferred
00:11:03 device descriptor fragment is invalid
00:11:03 endpoint 0x00 invalid wMaxPacketSize 0x0000
00:11:03 enumeration failed
00:11:03 destroying 0x0000/0000/0000 (IOUSBHostDevice): enumeration failure
... (Mac retries 8 times over 10 seconds, same error each time) ...
00:11:12 persistent enumeration failures
```

**Analysis**:
- VID/PID shows as `0x0000/0000/0000` - device identity never retrieved
- `wMaxPacketSize 0x0000` - EP0 control endpoint not configured
- Failure happens at USB enumeration step 1 (device descriptor read)
- Mac retries ~8 times then gives up with "persistent enumeration failures"
- This happens at BOOT, before any mode-switching code runs

## Deep Code Analysis (2025-01-27)

**Root cause**: `es_restart_usb_driver()` bypasses ChibiOS USB stack initialization

When USB mode is activated via `MD_USB` keycode, `keyboard_common.c:613` calls:
```c
es_restart_usb_driver();  // three_mode.c:207-215
```

This function does bare-metal USB register manipulation:
```c
void es_restart_usb_driver(void) {
    md_rcu_enable_usb(RCU);
    ald_usb_device_components_init();
    USB->TXIER = 0x7F;  // Hardcoded, bypasses ChibiOS
    USB->RXIER = 0x7E;
    USB->IER = 0x2F;
    usb_lld_connect_bus(0);
    ald_usb_int_register();
}
```

**Why this fails on Mac but works on iPhone**:
1. **ChibiOS state machine never initialized** - `USBD1` driver stays in `USB_STOP` state
2. **EP0 config incomplete** - `g_musb_udc` and `usb_ep0_state` not reset to handle control transfers
3. **No timing delays** - proper `usb_lld_start()` has delay loops; this has none
4. **Mac's AppleUSB driver is stricter** - rejects partially-initialized USB during enumeration

**Correct initialization** (in `hal_usb_lld.c:871-897`):
```c
void usb_lld_start(USBDriver *usbp) {
    if (usbp->state == USB_STOP) {
        ald_usb_device_components_init();
        for(i = 0; i < 9999; i++){}  // Timing delay
        ald_usb_int_status_get();     // Clear pending
        ald_usb_int_status_ep_get();
        for(i = 0; i < 9999; i++){}  // More delay
        ald_usb_int_enable(...);      // Via SDK, not hardcoded
        ald_usb_int_enable_ep(ALD_USB_INTEP_ALL);
        ald_usb_int_register();
    }
}
```

**Fix direction**: QMK already has proper `restart_usb_driver(&USBD1)` in `usb_main.c:349-375`:
```c
void restart_usb_driver(USBDriver *usbp) {
    usbDisconnectBus(usbp);
    usbStop(usbp);
    // ... stop/init all endpoints ...
    wait_ms(50);
    usbStart(usbp, &usbcfg);  // Proper ChibiOS init!
    usbConnectBus(usbp);
}
```
The `es_restart_usb_driver()` should call this instead of doing bare-metal register writes.

**Secondary issue**: `kb_board_init()` (keyboard_common.c:517-522) calls `Usb_Disconnect()` when not starting in USB mode, which may leave ChibiOS USB state inconsistent.

## Fix Attempts (2025-01-27) - ALL FAILED

**Attempt 1**: Replace `es_restart_usb_driver()` with `restart_usb_driver(&USBD1)`
- Result: Same error - `wMaxPacketSize 0x0000`

**Attempt 2**: Call `ald_usb_device_components_init()` then `restart_usb_driver(&USBD1)`
- Result: Same error - `wMaxPacketSize 0x0000`

**Key finding**: The `wMaxPacketSize 0x0000` error means EP0 max packet size is not configured.
EP0 should be 64 bytes, but it's returning 0.

**Root cause identified**: `Usb_Disconnect()` calls `md_rcu_enable_usb_reset(RCU)` which **hardware-resets the USB peripheral**, clearing all register state including EP0 configuration. When USB is later re-enabled, the peripheral starts blank and EP0 config set by `usb_lld_init()` during boot is lost.

The `g_musb_udc` structure (which holds EP0 mps=64) is internal to `hal_usb_lld.c` and only gets re-initialized:
1. In `usb_lld_init()` - called once at HAL startup
2. In `usb_lld_reset()` - called when USB reset signal received from host
3. In USB interrupt handler on reset flag

But when switching from BLE→USB mode, none of these paths run before Mac tries to enumerate.

**Why iPhone works**: Likely more tolerant timing - gives device more time to initialize before enumeration, or accepts malformed descriptors.

**Potential fix directions** (not yet tested):
1. Call `usb_lld_init()` explicitly in `es_restart_usb_driver()` to re-init EP0
2. Manually set `g_musb_udc.in_ep[0].ep_mps = 64` (requires exposing internal struct)
3. Don't call `Usb_Disconnect()` at boot - keep USB initialized but disconnected
4. Fix at HAL level: make `usb_lld_start()` always re-init EP0, not just on first start

## Git History & Code Origin Analysis (2026-01-27)

**Code origin**: The `lib/rdmctmzt_common/` library was added in a single commit by carlosedp:
```
commit 00f57128ff8d009544cb7dc97297c6b77c0a5f33
Author: Carlos de Paula <me@carlosedp.com>
Date:   Wed Nov 19 14:41:21 2025 -0300
    Add libs supporting three-way Epomaker and other keyboards based on ES32
```

Copyright in the files mentions "Finalkey" and "LiuLiuQMK" (github.com/LiuLiuQMK).
Per QMK issue #24085, this code appears to be from a third-party firmware provider
that supplies multiple Chinese keyboard vendors (Royal Kludge, Epomaker, etc.) who
have been shipping QMK-based firmware without providing full source code.

**LiuLiuQMK's own fork does NOT have this library** - their `lib/` folder only contains
standard ChibiOS/LUFA/etc. The rdmctmzt_common code was likely reverse-engineered or
obtained from the vendor's proprietary sources.

**Epomaker's official TH40 repo** (github.com/Epomaker/th40) is incomplete - it has
basic keyboard files but no wireless/three-mode functionality.

## Monsgeek vs Epomaker: Different USB Disconnect Approaches

The Monsgeek keyboards in the same fork (`keyboards/monsgeek/wireless/transport.c`)
use a **completely different, working approach**:

**Monsgeek (keyboards/monsgeek/wireless/transport.c:43-71) - WORKS:**
```c
void usb_transport_enable(bool enable) {
    if (enable) {
        usb_power_connect();
        restart_usb_driver(&USBD1);  // Uses standard QMK/ChibiOS
        host_set_driver(&chibios_driver);
    } else {
        usbStop(&USBD1);             // ChibiOS-level stop
        usbDisconnectBus(&USBD1);    // ChibiOS-level disconnect
        usb_power_disconnect();
    }
}
```

**Epomaker (lib/rdmctmzt_common/three_mode.c:222-229) - BROKEN:**
```c
void Usb_Disconnect(void) {
    ald_usb_int_unregister();
    usb_lld_disconnect_bus(0);
    md_rcu_enable_usb_reset(RCU);   // HARDWARE RESET - wipes all state!
    md_rcu_disable_usb_reset(RCU);
    md_rcu_disable_usb(RCU);        // Disables USB clock entirely
}
```

The Epomaker code does a **full hardware reset** of the USB peripheral via RCU
(Reset and Clock Unit), which clears all register state. The Monsgeek code just
uses ChibiOS-level stop/disconnect which keeps the hardware in a recoverable state.

## EP0 Hardware Registers Never Configured

Found in `hal_usb_lld.c:955-972`:
```c
void usb_lld_init_endpoint(USBDriver *usbp, usbep_t ep) {
    if (ep == 0) {
        g_musb_udc.out_ep[0].ep_mps = USB_CTRL_EP_MPS;  // Software only!
        g_musb_udc.in_ep[0].ep_mps = USB_CTRL_EP_MPS;
        return;  // Returns WITHOUT calling ald_usb_dev_ep_config()!
    }
    // ... for other EPs, calls ald_usb_dev_ep_config() which sets
    // hardware TXMAXP/RXMAXP registers ...
}
```

For EP0, only the software `g_musb_udc` structure is set. The hardware registers
(USB->TXMAXP, USB->RXMAXP) are NOT explicitly configured for EP0. The code relies
on:
1. Hardware defaults after reset
2. USB reset interrupt handler re-initializing EP0

After `Usb_Disconnect()` does hardware reset, the USB peripheral starts blank,
and when we try to restart USB, the host may try to enumerate before the USB
reset interrupt fires and configures EP0 properly.

## Fix Attempt 3 (2026-01-27): ChibiOS-level disconnect - FAILED

**Hypothesis**: The hardware reset in `Usb_Disconnect()` wipes USB state. Using ChibiOS-level
disconnect like Monsgeek does should preserve state.

**Changes made**:
```c
void es_restart_usb_driver(void) {
    restart_usb_driver(&USBD1);  // Just use QMK's standard restart
}

void Usb_Disconnect(void) {
    usbStop(&USBD1);             // ChibiOS-level instead of hardware reset
    usbDisconnectBus(&USBD1);
}
```

**Result**: Same error - `wMaxPacketSize 0x0000`

**What this tells us**:
- The problem is NOT just about the hardware reset in `Usb_Disconnect()`
- Even with ChibiOS managing USB state, EP0 still isn't configured properly
- The issue might be in the boot sequence or how USB is first initialized

## Boot Sequence Analysis (2026-01-28)

Traced the full QMK/ChibiOS initialization order:

```
1. ChibiOS startup assembly
   └─► __early_init()                    [chibios.c:127]
       └─► board_init()                  [th40.c:61]
           └─► kb_board_init()           [keyboard_common.c:507]
               ├─► [USB mode]  User_Usb_Init()    ← Does almost nothing!
               └─► [BLE mode]  Usb_Disconnect()   ← Hardware reset

2. main() starts
   └─► platform_setup()                  [platform.c:19]
       └─► halInit()
           └─► hal_lld_init()            [hal_lld.c:102]
               └─► md_rcu_enable_usb(RCU)  ← USB clock enabled HERE

3. main() continues
   └─► protocol_pre_init()               [chibios.c:144]
       └─► init_usb_driver(&USB_DRIVER)  [usb_main.c:326]
           ├─► usbDisconnectBus()
           ├─► usbStop()
           ├─► wait_ms(50)
           ├─► usbStart()
           │   └─► usb_lld_start()       [hal_usb_lld.c:871]
           │       └─► if (state == USB_STOP) { ... }  ← CONDITIONAL!
           └─► usbConnectBus()
```

### Critical Finding: `User_Usb_Init()` vs `es_restart_usb_driver()`

**At boot (USB mode), `User_Usb_Init()` does almost nothing:**
```c
void User_Usb_Init(void) {
    // ONLY clock calibration - NOT USB initialization!
    md_rcu_enable_csu(RCU);
    CSU->CON |= CSU_CON_AUTOEN_MSK;
    CSU->CON |= CSU_CON_CNTEN_MSK;
}
```

**Compare to `es_restart_usb_driver()` (used for mode switching):**
```c
void es_restart_usb_driver(void) {
    md_rcu_enable_usb(RCU);           // Enable USB clock
    ald_usb_device_components_init(); // Full device mode init
    USB->TXIER = 0x7F;                // Enable TX interrupts
    USB->RXIER = 0x7E;                // Enable RX interrupts
    USB->IER = 0x2F;                  // Enable status interrupts
    usb_lld_connect_bus(0);           // Connect to bus
    ald_usb_int_register();           // Register NVIC
}
```

**The boot path relies entirely on QMK's standard `init_usb_driver()` → `usb_lld_start()`.**

### Potential Issue: Conditional Initialization in `usb_lld_start()`

```c
void usb_lld_start(USBDriver *usbp) {
    if (usbp->state == USB_STOP) {    // ← Only runs if state is USB_STOP!
        ald_usb_device_components_init();
        // ... delays and interrupt setup ...
    }
    // If state != USB_STOP, ENTIRE INIT IS SKIPPED!
}
```

If `USBD1.state` is not `USB_STOP` for any reason, the initialization is completely skipped.
This could happen if:
- `usbStop()` didn't properly set state to `USB_STOP`
- Something corrupted the driver state
- ChibiOS driver object wasn't properly initialized

### Why Boot Fails But Mode-Switch Might Work Differently

| Path | USB Clock | Device Init | Interrupts | Bus Connect |
|------|-----------|-------------|------------|-------------|
| Boot (USB mode) | `hal_lld_init()` | `usb_lld_start()` (conditional) | `usb_lld_start()` (conditional) | `usbConnectBus()` |
| Mode switch (BLE→USB) | `es_restart_usb_driver()` | `es_restart_usb_driver()` (always) | `es_restart_usb_driver()` (always) | `es_restart_usb_driver()` |

The mode-switch path uses bare-metal init that **always runs**, while the boot path
uses ChibiOS which **may skip init** based on driver state.

### Hypothesis

The bug at boot (Scenario 2) may be caused by:
1. `usb_lld_start()` skipping initialization due to unexpected driver state, OR
2. ChibiOS USB init working but with incorrect timing/configuration for Mac

The mode-switch path (which also fails) has different issues - it bypasses ChibiOS
entirely and doesn't properly initialize the USB state machine.

## Test Scenarios

**Scenario 1** (tested, FAILS): Toggle=USB, key-switch to BLE, key-switch back to USB
- Physical toggle: USB position (unchanged throughout)
- Boot keyboard while plugged into Mac
- Press BLE key → keyboard works via Bluetooth
- Press USB key → enumeration fails, `wMaxPacketSize 0x0000`

**Scenario 2** (tested, FAILS): Toggle=USB, boot, no mode switching
- Physical toggle: USB position
- Boot keyboard while plugged into Mac
- Don't press any mode keys
- **Result: USB FAILS even at boot. Had to use BLE key to type.**
- **This means the bug is in initial USB init, NOT in mode-switching code.**

**Scenario 3** (tested 2026-01-28, FREEZE): Toggle=BLE, boot unplugged, then plug in USB
- Physical toggle: BLE position
- Boot keyboard **without USB cable**
- Connect via Bluetooth, verify working ✅
- Plug USB cable into Mac
- **RESULT: COMPLETE KEYBOARD FREEZE** - no response, required power cycle
- Did not get to test physical toggle flip
- **Firmware**: UNKNOWN - user flashed something but unclear if stock or fix attempt
- **TODO**: Re-test with confirmed firmware state

**Scenario 4** (untested): Toggle=USB, key-switch to BLE, replug USB cable
- Physical toggle: USB position
- Boot keyboard while plugged into Mac (USB fails as expected)
- Press BLE key → keyboard works via Bluetooth
- **Unplug USB cable, wait 5 seconds, plug it back in**
- **Question**: Does fresh USB connection trigger re-enumeration?

**Open questions** (answered by code analysis):
- ✅ `init_usb_driver()` IS called during boot regardless of mode (in `protocol_pre_init()`)
- ✅ Physical toggle is read by `kb_board_init()` which runs BEFORE `hal_lld_init()`
- ✅ `Usb_Disconnect()` runs BEFORE USB clock is enabled (in `__early_init`)
- ❓ Does `usb_lld_start()` actually execute its init code, or does the state check fail?

## Recommended Fix: Change Usb_Disconnect() to Match Monsgeek

Instead of patching `es_restart_usb_driver()`, change `Usb_Disconnect()` to use
ChibiOS-level operations that keep the USB state machine consistent:

```c
void Usb_Disconnect(void) {
    // Use ChibiOS stack like Monsgeek does:
    usbStop(&USBD1);
    usbDisconnectBus(&USBD1);

    // Remove the hardware reset that wipes USB state:
    // ald_usb_int_unregister();      // Now handled by usbStop()
    // usb_lld_disconnect_bus(0);     // Now handled by usbDisconnectBus()
    // md_rcu_enable_usb_reset(RCU);  // REMOVE - causes the bug
    // md_rcu_disable_usb_reset(RCU); // REMOVE
    // md_rcu_disable_usb(RCU);       // REMOVE - or keep if power saving needed
}
```

**Trade-off**: Slightly higher power consumption in BLE mode since USB peripheral
stays powered (but disconnected). May need to verify this doesn't break BLE
functionality or drain battery excessively.

**Also update es_restart_usb_driver()** to match:
```c
void es_restart_usb_driver(void) {
    // Just use QMK's standard restart:
    restart_usb_driver(&USBD1);
    // Remove all the bare-metal register manipulation
}
```

## Files to Modify

1. `lib/rdmctmzt_common/three_mode.c`:
   - `Usb_Disconnect()` - remove hardware reset, use ChibiOS
   - `es_restart_usb_driver()` - simplify to just `restart_usb_driver(&USBD1)`

2. Possibly `lib/rdmctmzt_common/keyboard_common.c`:
   - Check if `kb_board_init()` boot sequence needs adjustment

## References

- QMK issue #24085: Vendors distributing without source code
  https://github.com/qmk/qmk_firmware/issues/24085
- Monsgeek wireless transport: `keyboards/monsgeek/wireless/transport.c`
- ES32 USB HAL: `lib/chibios-contrib/os/hal/ports/ES32/LLD/USBv1/hal_usb_lld.c`
- QMK USB restart: `tmk_core/protocol/chibios/usb_main.c:349-375`
- Repo status: github.com/carlosedp/qmk_firmware has issues/discussions disabled

## Mac USB Debugging Runbook

**Get USB enumeration logs** (run BEFORE switching to USB mode, then switch):
```bash
# Wait 8s then capture last 15s of kernel USB logs
sleep 8 && /usr/bin/log show --last 15s | grep -i kernel | grep -iE "usb|AppleUSB|enumerat|endpoint|transaction|0x00" | head -40
```

**Key error patterns to look for**:
- `endpoint 0x00: status 0xe00002ed (transaction error)` - EP0 control transfer failed
- `device descriptor fragment is invalid` - Device responded but with bad data
- `wMaxPacketSize 0x0000` - EP0 max packet size not configured (should be 64)
- `destroying 0x0000/0000/0000` - VID/PID never retrieved, device killed

**Check connected USB devices**:
```bash
system_profiler SPUSBDataType | head -80
ioreg -p IOUSB -l | head -100
```

**Check Bluetooth (for comparison)**:
```bash
/usr/bin/log show --last 2m | grep -i usb | grep -i "EPOMAKER"
```

**Note**: Use `/usr/bin/log` not just `log` - zsh may have issues with the bare command.

## Investigation Methodology

**Problem**: Previous debugging went in circles because:
- Tests were not specific enough about toggle positions, key presses, cable state
- Assumed too much about what code paths were being exercised
- Made fixes without fully understanding the root cause
- **ASSUMED firmware state from git instead of asking what's actually flashed**

**Lesson learned (2026-01-28)**: Git repo state ≠ flashed firmware state. ALWAYS ASK.

### Pre-Test Checklist (MANDATORY before any test)

**Claude must confirm these with user BEFORE running any test:**

1. **Firmware state**:
   - [ ] What firmware is currently flashed? (stock / which fix attempt?)
   - [ ] When was it last flashed?
   - [ ] Do we need to flash a specific version first?

2. **Hardware state**:
   - [ ] Physical toggle position? (USB / BLE / 2.4G)
   - [ ] USB cable plugged in? To what? (Mac / iPhone / unplugged)
   - [ ] Keyboard powered on or off?

3. **Test procedure**:
   - [ ] What exact steps will be performed?
   - [ ] What should user observe/report?
   - [ ] What logs to capture (if any)?

4. **Expected outcome**:
   - [ ] What result would confirm hypothesis X?
   - [ ] What result would rule out hypothesis X?

### Test Execution Rules

1. **Be extremely specific** about test conditions:
   - Physical toggle position (USB/BLE)
   - USB cable state (plugged/unplugged, when plugged)
   - Boot sequence (power cycle vs wake)
   - Key presses (which keys, in what order)
   - What firmware version / which fixes applied

2. **Document observable symptoms precisely**:
   - LED states (which LEDs, what colors)
   - Mac logs (capture before/after)
   - Keyboard response (works, fails silently, freezes)

3. **Test one variable at a time**:
   - Don't combine multiple changes
   - Revert to known state between tests
   - Compare with/without specific fixes

4. **Prioritize diagnostic tests over fix attempts**:
   - Understand the failure mode first
   - Collect data that narrows down possibilities
   - Only attempt fixes when root cause is clear

5. **NEVER ASSUME - ALWAYS ASK**:
   - Don't check git status and assume that's what's flashed
   - Don't assume user remembers what was flashed
   - If uncertain, re-flash to establish known baseline

## Test Results (2026-01-28)

### Scenario 3: Toggle=BLE, hot-plug USB (Stock firmware)

**Firmware**: Stock Zip753 (SHA256: d24129e8b2b69b432fdc4567296bb08f33577901f1bf2e0dfcdc51d02cfab2d5)
**Toggle**: BLE
**Procedure**: Boot without USB, connect BLE, plug USB into Mac

**Result**:
- ✅ No freeze (previous freeze was a broken fix attempt, not stock behavior)
- Keyboard auto-switched to USB mode (G LED white on Fn layer)
- USB typing: FAILED (expected)
- Switched back to BLE via key, worked fine

### Scenario 5: Capture USB enumeration logs (Stock firmware)

**Firmware**: Same stock Zip753
**Toggle**: BLE
**Procedure**: BLE connected, unplug USB, run log capture, plug USB into Mac

**Mac kernel logs captured**:
```
00:51:39 endpoint 0x00: status 0xe00002ed (transaction error): 0 bytes transferred
00:51:39 device descriptor fragment is invalid
00:51:39 endpoint 0x00 invalid wMaxPacketSize 0x0000
00:51:39 enumeration failed
00:51:39 destroying 0x0000/0000/0000 (IOUSBHostDevice): enumeration failure
... (Mac retries 8 times over ~10 seconds) ...
00:51:49 persistent enumeration failures
```

**Analysis**:
- Confirms `wMaxPacketSize 0x0000` bug exists in stock carlosedp firmware
- EP0 control endpoint max packet size should be 64, but is 0
- Mac tries 8 times, all fail with same error
- This is the SAME error as documented earlier - stock firmware has this bug

## Firmware Version Tracking

| Version | SHA256 | three_mode.c | Behavior |
|---------|--------|--------------|----------|
| Stock Zip753 | d2412... | Unmodified | USB fails (wMaxPacketSize 0x0000), BLE works, no hot-plug freeze |
| Fix Attempt 3 | - | ChibiOS disconnect | Same failure (was this ever actually flashed?) |
| Fix Attempt 4 | 2dddc... | +usb_lld_reset() in three_mode.c | FAILED - same wMaxPacketSize 0x0000 |
| Fix Attempt 5 | 582eb... | +EP0 hw regs in usb_lld_init_endpoint | FAILED - wrong place (called after enum) |
| Fix Attempt 6 | d2065... | +EP0 hw regs in usb_lld_start | FAILED - test used mode-switch path, not boot |
| Fix Attempt 7 | 4c256... | +EP0 hw regs (TXMAXP/RXMAXP) in both paths | FAILED - wrong registers for EP0? |

**Current flashed firmware**: Fix Attempt 7 (4c25615af37ccc707f06303727e6f72f6505b30a2c22dbddfed7a1983f746e37)

## Fix Attempt 4 (2026-01-28): Call usb_lld_reset() in es_restart_usb_driver()

**Root cause confirmed**: `es_restart_usb_driver()` calls `ald_usb_device_components_init()` but never initializes `g_musb_udc.in_ep[0].ep_mps = 64`. Mac sees `wMaxPacketSize 0x0000`.

**Fix**: Call `usb_lld_reset(&USBD1)` which initializes EP0 state including ep_mps=64:
```c
void es_restart_usb_driver(void) {
    md_rcu_enable_usb(RCU);
    ald_usb_device_components_init();

    // FIX: Initialize EP0 state (was missing, causes wMaxPacketSize 0x0000 on Mac)
    usb_lld_reset(&USBD1);

    USB->TXIER = 0x7F;
    USB->RXIER = 0x7E;
    USB->IER = 0x2F;
    usb_lld_connect_bus(0);
    ald_usb_int_register();
}
```

**SHA256**: `2dddcef180cc656a820bb307fd3995c1f4b3b2f664284fa7f9c4f49780663278`

**Result**: FAILED - same `wMaxPacketSize 0x0000` error

**Mac logs (2026-01-28 01:18:14)**:
```
endpoint 0x00: status 0xe00002ed (transaction error): 0 bytes transferred
device descriptor fragment is invalid
endpoint 0x00 invalid wMaxPacketSize 0x0000
enumeration failed
destroying 0x0000/0000/0000 (IOUSBHostDevice): enumeration failure
```

**Analysis**: `usb_lld_reset()` doesn't set EP0 hardware registers either - it only sets the software `g_musb_udc` struct. The actual USB hardware EP0 FIFO/maxpacket registers are never configured.

## Fix Attempt 5 (2026-01-28): Write EP0 hardware registers in hal_usb_lld.c

**Root cause confirmed**: `usb_lld_init_endpoint()` at line 965-972 sets software struct for EP0 but returns WITHOUT calling `ald_usb_dev_ep_config()` which writes to `USB->TXMAXP` / `USB->RXMAXP`.

For non-EP0 endpoints, the code properly calls `ald_usb_dev_ep_config()`. For EP0, it just sets software state and returns.

**Fix**: Add hardware register writes in `hal_usb_lld.c:usb_lld_init_endpoint()`:
```c
if (ep == 0) {
    // ... existing software struct setup ...

    // FIX: Configure EP0 hardware registers (was missing!)
    uint32_t old_ep_idx = musb_get_active_ep();
    musb_set_active_ep(0);
    USB->TXMAXP = USB_CTRL_EP_MPS;  // 64 bytes
    USB->RXMAXP = USB_CTRL_EP_MPS;  // 64 bytes
    musb_set_active_ep(old_ep_idx);

    return;
}
```

**SHA256**: `582eb953c5a4edcb22d665cd84fa2d1d104d1375560f8f55030a1ccc3c815f97`

**Result**: FAILED - `usb_lld_init_endpoint()` is called during SET_CONFIGURATION (after enumeration), not before.

## Fix Attempt 6 (2026-01-28): EP0 hardware init in usb_lld_start()

**Analysis**: EP0 hardware registers must be configured BEFORE enumeration, not during SET_CONFIGURATION.

`usb_lld_start()` runs before host enumeration attempt, so it's the right place.

**SHA256**: `d2065e0dc21f19ebe855736f1e5353dbb22528d424f5bb0a5acf729236a35ece`

**Result**: FAILED - test used mode-switch path (flip toggle), not boot path

## Fix Attempt 7 (2026-01-28): EP0 hw regs in BOTH paths

Added EP0 hardware register writes to:
1. `usb_lld_start()` in hal_usb_lld.c (boot path)
2. `es_restart_usb_driver()` in three_mode.c (mode-switch path)

**SHA256**: `4c25615af37ccc707f06303727e6f72f6505b30a2c22dbddfed7a1983f746e37`

**Result**: FAILED - still wMaxPacketSize 0x0000

**Conclusion**: TXMAXP/RXMAXP registers may not be the right approach for EP0, or something else is clearing them.

## Investigation Summary (2026-01-28)

**What was tried (Fix Attempts 4-7):**
- Adding `usb_lld_reset()` call - no effect
- Setting EP0 TXMAXP/RXMAXP in `usb_lld_init_endpoint()` - wrong timing (called after enum)
- Setting EP0 TXMAXP/RXMAXP in `usb_lld_start()` - boot path only
- Setting EP0 TXMAXP/RXMAXP in both `usb_lld_start()` AND `es_restart_usb_driver()` - still fails

**Key observation**: All attempts to write `USB->TXMAXP = 64` and `USB->RXMAXP = 64` have no effect on Mac's reported `wMaxPacketSize`. Either:
1. These aren't the correct registers for EP0 on this controller
2. Something clears them after we set them
3. Mac reads EP0 max packet size from somewhere else (device descriptor?)
4. The writes aren't actually executing

**Unexplored lead**: iPhone USB works with the same firmware. What makes iPhone more tolerant?

## Next Steps: Debugging Approach

**Option 1: Add debug logging**
- Log register values after writing to verify they're set
- Capture logs on USB plug-in event
- Challenge: How to capture logs from the keyboard?

**Option 2: Use indicator LEDs for debugging**
- The 3 indicator LEDs (top right: Caps Lock, Scroll Lock, Num Lock) can be controlled freely
- Use them to signal code path execution:
  - LED 1 on = `es_restart_usb_driver()` was called
  - LED 2 on = EP0 registers were written
  - LED 3 on = USB interrupt fired
- This confirms whether code is executing without needing log capture

**Option 3: Compare with working path**
- iPhone USB works - investigate what's different in that enumeration
- Maybe add timing delays to match iPhone's more tolerant behavior

## Current Code State

**Files modified from stock:**
1. `lib/chibios-contrib/os/hal/ports/ES32/LLD/USBv1/hal_usb_lld.c`:
   - Added EP0 hw reg writes in `usb_lld_start()` (lines ~879-882)
   - Added EP0 hw reg writes in `usb_lld_init_endpoint()` (lines ~965-980)

2. `lib/rdmctmzt_common/three_mode.c`:
   - Added EP0 hw reg writes in `es_restart_usb_driver()` (lines ~210-213)

These changes are committed as ongoing investigation - they don't fix the issue but represent the debugging direction.

## Next Steps

1. **Try indicator LED debugging** - verify code paths are executing
2. **Investigate iPhone vs Mac difference** - why does iPhone USB work?
3. **Check device descriptor** - maybe bMaxPacketSize0 is hardcoded wrong somewhere
4. **Consider upstream** - check if carlosedp repo has any related issues/discussions
