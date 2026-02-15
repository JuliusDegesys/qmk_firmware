# Ferris Sweep Keymap — juliusdegesys

A 34-key Dvorak layout for the Ferris Sweep with two Adafruit KB2040 controllers.

Based on [callum's](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum) oneshot mod approach — custom oneshot implementation with no timers.

## Layers

- **DEF** — Dvorak base layer
- **SYM** — Symbols
- **NAV** — Navigation, window/tab switching, oneshot modifiers, clipboard
- **NUM** — Number pad and function keys (tri-layer: hold SYM + NAV)

## Features

- Custom oneshot modifiers (Shift, Ctrl, Alt, Cmd, Meh, Hyper) without timers
- Cmd-Tab / Cmd-Grave window switching (Cmd stays held while NAV layer is active)
- Ctrl-Tab / Ctrl-Shift-Tab tab switching (same hold behavior)
- Double-tap NAV to cancel all queued oneshot mods
- `QK_BOOT` on the NUM layer to enter bootloader without double-tapping reset

## Build & Flash

Requires QMK CLI. The Ferris Sweep defaults to Pro Micro, so we use `CONVERT_TO=kb2040` for the RP2040-based KB2040 controllers.

### First time setup (EE_HANDS)

Flash each half separately so the controller remembers which side it is:

```bash
# Left half — plug in left side, double-tap reset to enter bootloader (mounts as RPI-RP2)
qmk flash -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040 -bl uf2-split-left

# Right half — plug in right side, double-tap reset
qmk flash -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040 -bl uf2-split-right
```

This writes handedness to each controller's memory, so USB can be plugged into either side.

### Subsequent flashes

After handedness is set, you can flash both halves with the same command:

```bash
qmk flash -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040
```

### Build only (no flash)

```bash
qmk compile -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040
```
