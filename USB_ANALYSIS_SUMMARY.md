# USB Analysis Summary (2026-01-28)

## Background Agent Findings

### MUSB EP0 FIFO Analysis

**Critical finding**: EP0 FIFO address is NEVER configured.

- `TXMAXP`/`RXMAXP` are set to 64 bytes ✓
- `TXFIFO1`/`TXFIFO2` (address registers) - **NEVER SET for EP0**
- `RXFIFO1`/`RXFIFO2` (address registers) - **NEVER SET for EP0**

The hardware doesn't know where EP0's FIFO memory is located.

**Registers needed:**
- `USB->TXFIFO1`: Lower 8 bits of address (address >> 3)
- `USB->TXFIFO2`: bits 5-7 = max packet size, bit 4 = double buffering, bits 0-3 = upper address
- Same for `RXFIFO1/2`

**Location gap**: `usb_lld_init_endpoint()` at line 972 handles EP1-6 FIFO config but **returns early for EP0** without setting FIFO address.

### Monsgeek vs Epomaker Comparison

| Aspect | Epomaker (broken) | Monsgeek (working) |
|--------|-------------------|---------------------|
| USB restart | Raw register writes | `restart_usb_driver(&USBD1)` |
| Disconnect | RCU hardware reset (destroys state) | ChibiOS `usbStop()` (preserves) |
| EP0 config | Manual, incomplete | ChibiOS HAL handles |
| Timing | No delays | 50ms between stop/start |

**Why Monsgeek works**: Uses ChibiOS abstraction layer which properly initializes EP0 including FIFO.

**Why Epomaker fails**:
1. RCU reset clears all USB registers
2. Re-init doesn't set EP0 FIFO address
3. Mac sees `wMaxPacketSize 0x0000` because EP0 can't respond

## Potential Fix

Add EP0 FIFO address configuration in `usb_lld_start()` or `es_restart_usb_driver()`:

```c
// After setting TXMAXP/RXMAXP for EP0
USB->INDEX = 0;
USB->TXMAXP = 64;
USB->RXMAXP = 64;

// ADD: Configure EP0 FIFO address (at address 0, size=64, no double buffering)
USB->TXFIFO1 = 0;          // Address low bits = 0
USB->TXFIFO2 = (3 << 5);   // Size = 64 bytes (3 = log2(64/8)), no DPB, addr high = 0
USB->RXFIFO1 = 0;
USB->RXFIFO2 = (3 << 5);
```

**Note**: Size encoding for FIFO registers uses `log2(size/8)`:
- 8 bytes = 0
- 16 bytes = 1
- 32 bytes = 2
- 64 bytes = 3
- 128 bytes = 4

## Next Steps

1. Verify USB interrupts fire (Debug 9 LED test)
2. If interrupts don't fire → FIFO config is likely the fix
3. If interrupts fire but fail → need to trace deeper
