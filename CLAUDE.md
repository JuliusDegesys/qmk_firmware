# Notes for Claude

Personal fork of QMK. The keymaps that matter are
`keyboards/ferris/keymaps/juliusdegesys` (Ferris Sweep, KB2040 controllers)
and `keyboards/ergodox_ez/keymaps/juliusdegesys`. Everything else is upstream.

## Ferris Sweep handedness (EE_HANDS)

Each half stores whether it is left or right in its EEPROM (`EE_HANDS` in
`config.h`). A half reads its own keys in mirrored order depending on that
setting, and the half on USB places the other half's keys on the opposite side.

**Symptom of a wrong setting:** one half types mirrored. For example, the left
home row types `I U E O A` from pinky to index instead of `A O E U I`, and the
thumb keys are swapped (Shift on the outer thumb, NAV on the inner one). The
keymap is not the problem; the left half has stored itself as right.

**Cause:** an EEPROM reset (`eeconfig_init`, e.g. after a QMK update changes
the eeconfig layout) clears the handedness, and a cleared half defaults to
right.

**Fix:** flash each half on its own, with the TRRS cable disconnected, using the
handedness bootloader target. These builds rewrite the handedness on every boot,
so a later EEPROM reset cannot flip it again.

```bash
# Left half only on USB, then double-tap reset (or hold BOOT while plugging in)
qmk flash -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040 -bl uf2-split-left

# Right half only on USB, then double-tap reset
qmk flash -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040 -bl uf2-split-right
```

- Always flash this way. A plain `qmk flash` (no `-bl`) keeps whatever
  handedness is stored and will not fix a mirrored half.
- Use the reset/BOOT button, not `QK_BOOT`: with the halves separated, NAV is on
  the other half, so the NUM layer can't be reached.
- Never connect or disconnect the TRRS cable while either half has power.
- The user normally plugs USB into the right half via a CalDigit TS4 dock.
  `SPLIT_WATCHDOG_ENABLE` is set so the board recovers when the dock powers it
  without a host attached.
