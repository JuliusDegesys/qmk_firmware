# Ferris Sweep Keymap — juliusdegesys

A 34-key Dvorak layout for the Ferris Sweep with two Adafruit KB2040 controllers.

Based on [callum's](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum) oneshot mod approach — custom oneshot implementation with no timers.

## Layers

- **BASE** — Dvorak base layer
- **NAV** — Navigation, window/tab switching, oneshot modifiers, clipboard
- **SYM** — Symbols
- **NUM** — Number pad and function keys (tri-layer: hold SYM + NAV)

## Features

- Custom oneshot modifiers (Shift, Ctrl, Alt, Cmd, Meh, Hyper) without timers
- Cmd-Tab / Cmd-Grave window switching (Cmd stays held while NAV layer is active)
- Ctrl-Tab / Ctrl-Shift-Tab tab switching (same hold behavior)
- Double-tap NAV to cancel all queued oneshot mods
- `QK_BOOT` on the NUM layer to enter bootloader without double-tapping reset
- Keypress and layer-change logging via QMK console (when `CONSOLE_ENABLE = yes`)
- Real-time heatmap visualizer showing all 4 layers simultaneously

## Build & Flash

Requires QMK CLI. The Ferris Sweep defaults to Pro Micro, so we use `CONVERT_TO=kb2040` for the RP2040-based KB2040 controllers.

### First time setup (EE_HANDS)

Flash each half separately so the controller remembers which side it is:

```bash
# Left half — plug in left side, enter bootloader
qmk flash -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040 -bl uf2-split-left

# Right half — plug in right side, enter bootloader
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

### Flashing with picotool

If macOS won't mount the RPI-RP2 volume (common on recent macOS versions), use `picotool` instead:

```bash
brew install picotool

# Enter bootloader (QK_BOOT on keyboard or hold BOOT + tap RESET on the KB2040)
# Then:
picotool load ferris_sweep_juliusdegesys_kb2040.uf2 && picotool reboot
```

Repeat for the other half.

## Heatmap Visualizer

The firmware logs every keypress and layer change to the QMK console. A web-based heatmap visualizer shows usage across all 4 layers in real time, with the active layer highlighted as you type.

### Setup

```bash
# Terminal 1 — capture keylog from the keyboard:
qmk console | tee ~/keylog.txt

# Terminal 2 — start the visualizer (opens browser automatically):
python3 serve_heatmap.py
```

The visualizer at `http://localhost:8989` streams keypresses via Server-Sent Events and updates per-keypress. Each layer card shows the physical Sweep layout with keys colored by usage frequency.

### CLI analysis

For a text-based summary instead of the web visualizer:

```bash
python3 analyze_usage.py ~/keylog.txt
```

This prints per-layer breakdowns with keycode names and hit counts.

### Disabling logging

To remove the logging overhead, set `CONSOLE_ENABLE = no` in `rules.mk` and reflash.

## Files

| File | Purpose |
|---|---|
| `keymap.c` | Keymap, oneshot mods, swapper, console logging |
| `config.h` | EE_HANDS, USB polling, serial speed, oneshot config |
| `rules.mk` | Build flags (console, LTO, disabled features) |
| `heatmap.html` | Real-time 4-layer heatmap visualizer |
| `serve_heatmap.py` | Local HTTP + SSE server for live heatmap streaming |
| `analyze_usage.py` | CLI keylog analysis script |
