#include QMK_KEYBOARD_H

#define KC_UNDO LGUI(KC_Z)
#define KC_CUT LGUI(KC_X)
#define KC_COPY LGUI(KC_C)
#define KC_PASTE LGUI(KC_V)

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

enum custom_keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    OS_MEH,
    OS_HYPR,

    CMD_TAB, // Switch to next window         (cmd-tab)
    CMD_GRV, // Switch to next window of the same app (cmd-grv)
    CTRL_TAB, // Switch to next tab
    CTRL_SHIFT_TAB, // Switch to prev tab
};

enum layers {
    DEF,
    SYM,
    NAV,
    NUM,
};

// Represents the four states a oneshot key can be in
typedef enum {
    os_up_unqueued, // Key is up and not queued -- default state
    os_down_unused, // Key is down and not used
    os_up_queued,   // Key is up and queued
    os_down_used,   // Key is down and used
} oneshot_state;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [DEF] = LAYOUT(
    KC_ESC , KC_COMM, KC_DOT, KC_P  , KC_Y   ,    KC_F  , KC_G   , KC_C, KC_R, KC_L,
    KC_A   , KC_O   , KC_E  , KC_U  , KC_I   ,    KC_D  , KC_H   , KC_T, KC_N, KC_S,
    KC_SCLN, KC_Q   , KC_J  , KC_K  , KC_X   ,    KC_B  , KC_M   , KC_W, KC_V, KC_Z,
                              LA_NAV, KC_LSFT,    KC_SPC, LA_SYM
  ),
  [SYM] = LAYOUT(
    KC_GRV , KC_QUOT, KC_DQUO, KC_AMPR, KC_HASH,    KC_PLUS, KC_EXLM, KC_QUES, KC_AT  , KC_ASTR,
    KC_PIPE, KC_LCBR, KC_LBRC, KC_LPRN, KC_LT,      KC_GT  , KC_RPRN, KC_RBRC, KC_RCBR, _______,
    KC_TILD, KC_UNDS, KC_COLN, KC_BSLS, KC_EQL ,    KC_MINS, KC_PERC, KC_CIRC, KC_DLR , KC_SLSH,
                               _______, _______,    _______, _______
  ),
  [NAV] = LAYOUT(
    CMD_GRV, CMD_TAB, LGUI(LSFT(KC_T)), LGUI(KC_T), LGUI(KC_SPACE)  ,    KC_PGUP, CTRL_SHIFT_TAB, KC_UP  , CTRL_TAB, _______,
    OS_SHFT, OS_CTRL, OS_ALT          , OS_CMD    , OS_HYPR         ,    KC_ENT , KC_LEFT       , KC_DOWN, KC_RGHT , KC_BSPC,
    KC_UNDO,  KC_CUT, KC_COPY         , KC_PASTE  , LGUI(LSFT(KC_4)),    KC_PGDN, LSFT(KC_TAB)  , KC_TAB , _______ , KC_DEL ,
                                        _______   , _______         ,    _______, _______
  ),
  [NUM] = LAYOUT(
    KC_F8  , KC_F7 , KC_F6 , KC_F5, _______,    KC_PLUS, KC_7, KC_8, KC_9, KC_ASTR,
    KC_F4  , KC_F3 , KC_F2 , KC_F1, _______,    KC_0   , KC_4, KC_5, KC_6, KC_BSPC,
    KC_F12,  KC_F11, KC_F10, KC_F9, _______,    KC_MINS, KC_1, KC_2, KC_3, KC_SLSH,
                           _______, _______,    QK_BOOT, _______
  ),
};


bool cmd_tab_active = false;
bool ctrl_tab_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_hypr_state = os_up_unqueued;

////////////////////////////////////////////////////////////////////////////////
// Oneshot mod implementation
////////////////////////////////////////////////////////////////////////////////


// To be implemented by the consumer. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case KC_ESC:
        return true;
    case LA_SYM:
    case LA_NAV:
    default:
        return false;
    }
}

// To be implemented by the consumer. Defines keys to ignore when determining
// whether a oneshot mod has been used. Setting this to modifiers and layer
// change keys allows stacking multiple oneshot modifiers, and carrying them
// between layers.
bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case KC_ESC:
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
    case OS_HYPR:
        return true;
    default:
        return false;
    }
}

void reset_oneshot(oneshot_state *state, uint16_t mod) {
    *state = os_up_unqueued;
    unregister_code(mod);
}

// Custom oneshot mod implementation that doesn't rely on timers. If a mod is
// used while it is held it will be unregistered on keyup as normal, otherwise
// it will be queued and only released after the next non-mod keyup.
void update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    bool key_down = record->event.pressed;
    if (key_down) {
        if (keycode == trigger) {
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
        } else if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
            // Cancel oneshot on designated cancel keydown.
            reset_oneshot(state, mod);
        }
    } else { // Key Up
        if (keycode == trigger) {
            if (*state == os_down_unused) {
                // If we didn't use the mod while trigger was held, queue it.
                *state = os_up_queued;
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

uint16_t last_keycode = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == CMD_TAB) {
        if (record->event.pressed) {
            if (!cmd_tab_active) {
                cmd_tab_active = true;
                register_code(KC_LGUI);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
    } else if (keycode == CMD_GRV) {
        if (record->event.pressed) {
            if (!cmd_tab_active) {
                cmd_tab_active = true;
                register_code(KC_LGUI);
            }
            register_code(KC_GRV);
        } else {
            unregister_code(KC_GRV);
        }
    } else if (keycode == LA_NAV) {
       if (cmd_tab_active) {
            if (!record->event.pressed) {
                cmd_tab_active = false;
                unregister_code(KC_LGUI);
            }
        }
    }

    if (keycode == CTRL_TAB) {
        if (record->event.pressed) {
            if (!ctrl_tab_active) {
                ctrl_tab_active = true;
                register_code(KC_LCTL);
            }
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
    } else if (keycode == CTRL_SHIFT_TAB) {
        if (record->event.pressed) {
            if (!ctrl_tab_active) {
                ctrl_tab_active = true;
                register_code(KC_LCTL);
            }
            register_code(KC_LSFT);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
            unregister_code(KC_LSFT);
        }
    } else if (keycode == LA_NAV) {
       if (ctrl_tab_active) {
            if (!record->event.pressed) {
                ctrl_tab_active = false;
                unregister_code(KC_LCTL);
            }
        }
    }

    update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
    update_oneshot(&os_shft_state, KC_LSFT, OS_HYPR, keycode, record);
    update_oneshot(&os_shft_state, KC_LSFT, OS_MEH, keycode, record);

    update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_HYPR, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_MEH, keycode, record);

    update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, OS_HYPR, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, OS_MEH, keycode, record);

    update_oneshot(&os_cmd_state, KC_LCMD, OS_CMD, keycode, record);
    update_oneshot(&os_cmd_state, KC_LCMD, OS_HYPR, keycode, record);

    if (keycode == LA_NAV && record->event.pressed && last_keycode == LA_NAV) {
        reset_oneshot(&os_shft_state, KC_LSFT);
        reset_oneshot(&os_ctrl_state, KC_LCTL);
        reset_oneshot(&os_alt_state, KC_LALT);
        reset_oneshot(&os_cmd_state, KC_LCMD);
    }

    if (record->event.pressed) {
        last_keycode = keycode;
    }

    return true;
}


layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
