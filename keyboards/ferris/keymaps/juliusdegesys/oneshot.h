// Oneshot modifiers without tap-hold timing, based on callum's implementation:
// https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum
#pragma once

#include "quantum.h"

// The four states a oneshot key can be in.
typedef enum {
    os_up_unqueued, // Key is up and not queued -- default state
    os_down_unused, // Key is down and not used
    os_up_queued,   // Key is up and queued
    os_down_used,   // Key is down and used
} oneshot_state;

// Implemented in keymap.c. Keys that cancel any active oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// Implemented in keymap.c. Keys that don't count as "using" a oneshot mod.
// Listing the mods and layer keys here lets mods stack and carry across layers.
bool is_oneshot_ignored_key(uint16_t keycode);

// Feed every key event through this, once per (mod, trigger) pair. If the mod
// is used while its trigger is held it is released on keyup as normal;
// otherwise it is queued and released after the next non-ignored keyup.
// *queued_at records when the mod was queued, for oneshot_expire().
void update_oneshot(oneshot_state *state, uint16_t mod, uint16_t trigger, uint16_t keycode, keyrecord_t *record, uint32_t *queued_at);

// Release a queued mod that has gone unused for longer than timeout_ms.
// Call from matrix_scan_user().
void oneshot_expire(oneshot_state *state, uint16_t mod, uint32_t queued_at, uint32_t timeout_ms);
