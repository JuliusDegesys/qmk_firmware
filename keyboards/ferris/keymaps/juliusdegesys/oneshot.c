#include "oneshot.h"

static void reset_oneshot(oneshot_state *state, uint16_t mod) {
    *state = os_up_unqueued;
    unregister_code(mod);
}

void update_oneshot(oneshot_state *state, uint16_t mod, uint16_t trigger, uint16_t keycode, keyrecord_t *record, uint32_t *queued_at) {
    if (record->event.pressed) {
        if (keycode == trigger) {
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
        } else if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
            // Cancel oneshot on designated cancel keydown.
            reset_oneshot(state, mod);
        }
    } else { // Key up
        if (keycode == trigger) {
            if (*state == os_down_unused) {
                // If we didn't use the mod while trigger was held, queue it.
                *state     = os_up_queued;
                *queued_at = timer_read32();
            } else if (*state == os_down_used) {
                // If we did use the mod while trigger was held, unregister it.
                reset_oneshot(state, mod);
            }
        } else if (!is_oneshot_ignored_key(keycode)) {
            // On non-ignored keyup, consider the oneshot used.
            if (*state == os_down_unused) {
                *state = os_down_used;
            } else if (*state == os_up_queued) {
                reset_oneshot(state, mod);
            }
        }
    }
}

void oneshot_expire(oneshot_state *state, uint16_t mod, uint32_t queued_at, uint32_t timeout_ms) {
    if (*state == os_up_queued && timer_elapsed32(queued_at) > timeout_ms) {
        reset_oneshot(state, mod);
    }
}
