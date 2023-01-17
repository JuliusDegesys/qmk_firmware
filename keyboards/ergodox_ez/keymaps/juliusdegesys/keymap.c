#include QMK_KEYBOARD_H
#include "version.h"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)
#define ES_LESS_MAC KC_GRAVE
#define ES_GRTR_MAC LSFT(KC_GRAVE)
#define ES_BSLS_MAC ALGR(KC_6)
#define NO_PIPE_ALT KC_GRAVE
#define NO_BSLS_ALT KC_EQUAL
#define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc)
#define BP_NDSH_MAC ALGR(KC_8)
#define SE_SECT_MAC ALGR(KC_6)

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

enum custom_keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = EZ_SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    OS_MEH,
    OS_HYPR,

    SW_WIN,  // Switch to next window         (cmd-tab)
    SW_LANG, // Switch to next input language (ctl-spc)
};

enum layers {
    DEF,
    SYM,
    NAV,
    NUM,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [DEF] = LAYOUT_ez34(
    KC_QUOT, KC_COMM, KC_DOT, KC_P  , KC_Y   , KC_F  , KC_G   , KC_C, KC_R, KC_L,
    KC_A   , KC_O   , KC_E  , KC_U  , KC_I   , KC_D  , KC_H   , KC_T, KC_N, KC_S,
    KC_SCLN, KC_Q   , KC_J  , KC_K  , KC_X   , KC_B  , KC_M   , KC_W, KC_V, KC_Z,
                              LA_NAV, KC_LSFT, KC_SPC, LA_SYM
  ),
  [SYM] = LAYOUT_ez34(
    KC_TILD, KC_UNDS, KC_PLUS, KC_QUES, KC_PIPE, XXXXXXX, KC_EXLM, KC_AT  , KC_HASH, KC_DLR ,
    XXXXXXX, KC_LCBR, KC_LBRC, KC_LPRN, KC_UNDS, XXXXXXX, KC_RPRN, KC_RBRC, KC_RCBR, KC_ESC ,
    KC_GRV , KC_MINS, KC_EQL , KC_SLSH, KC_BSLS, XXXXXXX, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR,
                               _______, _______, _______, _______
  ),
  [NAV] = LAYOUT_ez34(
    XXXXXXX    , SW_WIN    , LGUI(LSFT(KC_T)), LGUI(KC_T)  , LGUI(KC_SPACE)  , KC_PGUP, LCTL(LSFT(KC_TAB)), KC_UP  , LCTL(KC_TAB), KC_ESC ,
    OS_SHFT    , OS_CTRL   , OS_ALT          , OS_CMD      , OS_HYPR         , KC_ENT , KC_LEFT           , KC_DOWN, KC_RGHT     , KC_BSPC,
    KC_MAC_UNDO, KC_MAC_CUT, KC_MAC_COPY     , KC_MAC_PASTE, LGUI(LSFT(KC_4)), KC_PGDN, LSFT(KC_TAB)      , KC_TAB , KC_GRAVE    , KC_DEL ,
                                                    _______      , _______         , _______                    , _______
  ),
  [NUM] = LAYOUT_ez34(
    KC_F1 , KC_F2 , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,
    KC_1  , KC_2  , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   ,
    KC_F11, KC_F12, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______
  ),
};



extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
}

#define OFF {0, 0, 0}
#define CYAN {0, 222, 238}
#define PURP {204, 29, 197}
#define RED {255, 0, 0}
#define GREEN {66, 255, 0}

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [DEF] = { OFF, OFF, OFF, OFF, OFF, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, CYAN, OFF, OFF, OFF, OFF },
    [NAV] = { OFF, OFF, OFF, OFF, OFF, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, PURP, PURP, PURP, PURP, OFF, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, PURP, OFF, OFF, OFF, OFF },
    [SYM] = { OFF, OFF, OFF, OFF, OFF, OFF, RED, RED, RED, RED, OFF, RED, RED, RED, RED, OFF, RED, RED, RED, RED, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, RED, RED, RED, RED, RED, RED, RED, RED, RED, OFF, RED, RED, RED, RED, RED, OFF, OFF, OFF, OFF },
    [NUM] = { OFF, OFF, OFF, OFF, OFF, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, OFF, OFF, OFF, GREEN, GREEN, OFF, OFF, OFF, OFF },

};

void set_layer_color(int layer) {
  for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
    RGB rgb = {
      .r = pgm_read_byte(&ledmap[layer][i][0]),
      .g = pgm_read_byte(&ledmap[layer][i][1]),
      .b = pgm_read_byte(&ledmap[layer][i][2]),
    };
    // Global brightness setting.
    float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
    rgb_matrix_set_color(i, f*rgb.r, f*rgb.g, f*rgb.b);
  }
}

bool rgb_matrix_indicators_user(void) {
  if (keyboard_config.disable_layer_led) { return false; }
  switch (biton32(layer_state)) {
    case DEF:
    case SYM:
    case NAV:
    case NUM:
      set_layer_color(biton32(layer_state));
      break;
   default:
    if (rgb_matrix_get_flags() == LED_FLAG_NONE)
      rgb_matrix_set_color_all(0, 0, 0);
    break;
  }
  return true;
}


////////////////////////////////////////////////////////////////////////////////
// Oneshot mod implementation
////////////////////////////////////////////////////////////////////////////////

// Represents the four states a oneshot key can be in
typedef enum {
    os_down_unused, // Key is down and not used
    os_down_used, // Key is down and used
    os_up_unqueued, // Key is up and not queued
    os_up_queued,  // Key is up and queued
} oneshot_state;

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
    if (keycode == trigger) {
        if (key_down) {
            // Trigger keydown
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
        } else {
            // Trigger keyup
            switch (*state) {
            case os_down_unused:
                // If we didn't use the mod while trigger was held, queue it.
                *state = os_up_queued;
                break;
            case os_down_used:
                // If we did use the mod while trigger was held, unregister it.
                reset_oneshot(state, mod);
                break;
            default:
                break;
            }
        }
    } else {
        if (key_down) {
            if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
                // Cancel oneshot on designated cancel keydown.
                reset_oneshot(state, mod);
            }
        } else {
            if (!is_oneshot_ignored_key(keycode)) {
                // On non-ignored keyup, consider the oneshot used.
                switch (*state) {
                case os_down_unused:
                    *state = os_down_used;
                    break;
                case os_up_queued:
                    reset_oneshot(state, mod);
                    break;
                default:
                    break;
                }
            }
        }
    }
}



bool sw_win_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_hypr_state = os_up_unqueued;

void update_swapper(
    bool *active,
    uint16_t cmdish,
    uint16_t tabish,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            if (!*active) {
                *active = true;
                register_code(cmdish);
            }
            register_code(tabish);
        } else {
            unregister_code(tabish);
            // Don't unregister cmdish until some other key is hit or released.
        }
    } else if (*active) {
        unregister_code(cmdish);
        *active = false;
    }
}

uint16_t last_keycode = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // TODO: Toggle some home-row lights based on the state.
    update_swapper(&sw_win_active, KC_LGUI, KC_TAB, SW_WIN, keycode, record);

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
    state = update_tri_layer_state(state, SYM, NAV, NUM);

    uint8_t layer = biton(state);
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    ergodox_right_led_1_set(15);
    ergodox_right_led_2_set(15);
    ergodox_right_led_3_set(15);

    switch (layer) {
        case DEF:
            // Blue
            // Don't display anything for now...
            ergodox_right_led_3_on();
            break;
        case SYM:
            // Red
            ergodox_right_led_1_on();
            break;
        case NAV:
            // Purple
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        case NUM:
            // Green
            ergodox_right_led_2_on();
            break;
        default:
            break;
    }
    return state;
}
