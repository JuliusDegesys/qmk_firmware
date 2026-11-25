#include QMK_KEYBOARD_H
#include "oneshot.h"

// When the layout changes, also update the key labels in heatmap.html and the
// layer descriptions in README.md.

// macOS clipboard shortcuts. Not named KC_* because QMK has KC_UNDO etc.
#define CMD_UNDO LGUI(KC_Z)
#define CMD_CUT LGUI(KC_X)
#define CMD_COPY LGUI(KC_C)
#define CMD_PASTE LGUI(KC_V)

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

// Queued oneshot mods are released after this long unused. Not named
// ONESHOT_TIMEOUT because that is QMK's setting for its own oneshot keys.
#define OSK_TIMEOUT_MS 2000

enum custom_keycodes {
    // Custom oneshot mods. Prefixed OSK_ because QMK defines OS_MEH and
    // OS_HYPR itself (as timed OSM() keycodes).
    OSK_SHFT = SAFE_RANGE,
    OSK_CTRL,
    OSK_ALT,
    OSK_CMD,
    // SAFE_RANGE + 4 was OSK_MEH. Values are kept so keylogs recorded before
    // it was removed still decode correctly.
    OSK_HYPR = SAFE_RANGE + 5,

    CMD_TAB, // Switch to next app                    (cmd-tab)
    CMD_GRV, // Switch to next window of the same app (cmd-grv)
};

enum layers {
    BASE,
    SYM,
    NAV,
    NUM,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Base layer: Dvorak
  [BASE] = LAYOUT(
    KC_ESC , KC_COMM, KC_DOT, KC_P  , KC_Y   ,    KC_F  , KC_G   , KC_C, KC_R, KC_L,
    KC_A   , KC_O   , KC_E  , KC_U  , KC_I   ,    KC_D  , KC_H   , KC_T, KC_N, KC_S,
    KC_SCLN, KC_Q   , KC_J  , KC_K  , KC_X   ,    KC_B  , KC_M   , KC_W, KC_V, KC_Z,
                              LA_NAV, KC_LSFT,    KC_SPC, LA_SYM
  ),
  // Symbols: hold SYM (right outer thumb)
  [SYM] = LAYOUT(
    KC_TILD, KC_QUOT, KC_DQUO, KC_AMPR, KC_HASH,    KC_PLUS, KC_EXLM, KC_QUES, KC_AT  , KC_ASTR,
    KC_PIPE, KC_LCBR, KC_LBRC, KC_LPRN, KC_LT  ,    KC_GT  , KC_RPRN, KC_RBRC, KC_RCBR, XXXXXXX,
    KC_GRV , KC_UNDS, KC_COLN, KC_BSLS, KC_EQL ,    KC_MINS, KC_PERC, KC_CIRC, KC_DLR , KC_SLSH,
                               _______, _______,    _______, _______
  ),
  // Navigation, window switching, oneshot mods: hold NAV (left outer thumb)
  [NAV] = LAYOUT(
    CMD_GRV , CMD_TAB , XXXXXXX , XXXXXXX  , XXXXXXX         ,    KC_PGUP, LCTL(KC_PGUP), KC_UP  , LCTL(KC_PGDN), XXXXXXX,
    OSK_SHFT, OSK_CTRL, OSK_ALT , OSK_CMD  , OSK_HYPR        ,    KC_ENT , KC_LEFT      , KC_DOWN, KC_RGHT      , KC_BSPC,
    CMD_UNDO, CMD_CUT , CMD_COPY, CMD_PASTE, LGUI(LSFT(KC_4)),    KC_PGDN, LSFT(KC_TAB) , KC_TAB , XXXXXXX      , KC_DEL ,
                                  _______  , _______         ,    _______, _______
  ),
  // Function keys and numpad: hold NAV and SYM together
  [NUM] = LAYOUT(
    KC_F1, KC_F2 , KC_F3 , KC_F4 , XXXXXXX,    KC_PLUS, KC_7, KC_8, KC_9, KC_ASTR,
    KC_F5, KC_F6 , KC_F7 , KC_F8 , XXXXXXX,    KC_0   , KC_4, KC_5, KC_6, KC_BSPC,
    KC_F9, KC_F10, KC_F11, KC_F12, XXXXXXX,    KC_MINS, KC_1, KC_2, KC_3, KC_SLSH,
                           _______, _______,    QK_BOOT, _______
  ),
};
// clang-format on

////////////////////////////////////////////////////////////////////////////////
// Oneshot mods (logic in oneshot.c)
////////////////////////////////////////////////////////////////////////////////

static oneshot_state os_shft_state = os_up_unqueued;
static oneshot_state os_ctrl_state = os_up_unqueued;
static oneshot_state os_alt_state  = os_up_unqueued;
static oneshot_state os_cmd_state  = os_up_unqueued;

static uint32_t os_shft_queued_at = 0;
static uint32_t os_ctrl_queued_at = 0;
static uint32_t os_alt_queued_at  = 0;
static uint32_t os_cmd_queued_at  = 0;

bool is_oneshot_cancel_key(uint16_t keycode) {
    return keycode == KC_ESC;
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case KC_ESC:
        case LA_SYM:
        case LA_NAV:
        case KC_LSFT:
        case OSK_SHFT:
        case OSK_CTRL:
        case OSK_ALT:
        case OSK_CMD:
        case OSK_HYPR:
            return true;
        default:
            return false;
    }
}

// Each mod is driven by its own key and by Hyper, which applies all four.
static void update_oneshots(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(&os_shft_state, KC_LSFT, OSK_SHFT, keycode, record, &os_shft_queued_at);
    update_oneshot(&os_shft_state, KC_LSFT, OSK_HYPR, keycode, record, &os_shft_queued_at);

    update_oneshot(&os_ctrl_state, KC_LCTL, OSK_CTRL, keycode, record, &os_ctrl_queued_at);
    update_oneshot(&os_ctrl_state, KC_LCTL, OSK_HYPR, keycode, record, &os_ctrl_queued_at);

    update_oneshot(&os_alt_state, KC_LALT, OSK_ALT, keycode, record, &os_alt_queued_at);
    update_oneshot(&os_alt_state, KC_LALT, OSK_HYPR, keycode, record, &os_alt_queued_at);

    update_oneshot(&os_cmd_state, KC_LCMD, OSK_CMD, keycode, record, &os_cmd_queued_at);
    update_oneshot(&os_cmd_state, KC_LCMD, OSK_HYPR, keycode, record, &os_cmd_queued_at);
}

void matrix_scan_user(void) {
    oneshot_expire(&os_shft_state, KC_LSFT, os_shft_queued_at, OSK_TIMEOUT_MS);
    oneshot_expire(&os_ctrl_state, KC_LCTL, os_ctrl_queued_at, OSK_TIMEOUT_MS);
    oneshot_expire(&os_alt_state, KC_LALT, os_alt_queued_at, OSK_TIMEOUT_MS);
    oneshot_expire(&os_cmd_state, KC_LCMD, os_cmd_queued_at, OSK_TIMEOUT_MS);
}

////////////////////////////////////////////////////////////////////////////////
// Cmd+Tab / Cmd+` window switching: Cmd stays held until NAV is released, so
// repeated taps keep cycling.
////////////////////////////////////////////////////////////////////////////////

static bool cmd_tab_active = false;
static bool cmd_grv_active = false;

static void hold_cmd_and_tap(uint16_t key, bool *active, keyrecord_t *record) {
    if (record->event.pressed) {
        if (!cmd_tab_active && !cmd_grv_active) {
            *active = true;
            register_code(KC_LGUI);
        }
        register_code(key);
    } else {
        unregister_code(key);
    }
}

static void update_window_switcher(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CMD_TAB:
            hold_cmd_and_tap(KC_TAB, &cmd_tab_active, record);
            break;
        case CMD_GRV:
            hold_cmd_and_tap(KC_GRV, &cmd_grv_active, record);
            break;
        case LA_NAV:
            if (!record->event.pressed && (cmd_tab_active || cmd_grv_active)) {
                cmd_tab_active = false;
                cmd_grv_active = false;
                unregister_code(KC_LGUI);
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Console logging for the heatmap (see README.md for the log format)
////////////////////////////////////////////////////////////////////////////////

#ifdef CONSOLE_ENABLE
void keyboard_post_init_user(void) {
    // Print custom keycode mappings so the analysis script can decode them.
    uprintf("MAP:0x%04X:OS_SHFT\n", OSK_SHFT);
    uprintf("MAP:0x%04X:OS_CTRL\n", OSK_CTRL);
    uprintf("MAP:0x%04X:OS_ALT\n", OSK_ALT);
    uprintf("MAP:0x%04X:OS_CMD\n", OSK_CMD);
    uprintf("MAP:0x%04X:OS_HYPR\n", OSK_HYPR);
    uprintf("MAP:0x%04X:CMD_TAB\n", CMD_TAB);
    uprintf("MAP:0x%04X:CMD_GRV\n", CMD_GRV);
    uprintf("MAP:0x%04X:CTRL_PGUP\n", LCTL(KC_PGUP));
    uprintf("MAP:0x%04X:CTRL_PGDN\n", LCTL(KC_PGDN));
    uprintf("MAP:0x%04X:BOOT\n", QK_BOOT);
    uprintf("MAP:0x%04X:MO_NAV\n", LA_NAV);
    uprintf("MAP:0x%04X:MO_SYM\n", LA_SYM);
    uprintf("MAP:0x%04X:Cmd-Z\n", CMD_UNDO);
    uprintf("MAP:0x%04X:Cmd-X\n", CMD_CUT);
    uprintf("MAP:0x%04X:Cmd-C\n", CMD_COPY);
    uprintf("MAP:0x%04X:Cmd-V\n", CMD_PASTE);
}

static void log_key_event(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uprintf("KD:%u:%u:%u:0x%04X:0x%02X:%lu\n", get_highest_layer(layer_state), record->event.key.row, record->event.key.col, keycode, get_mods(), (unsigned long)timer_read32());
    } else {
        uprintf("KU:%u:%u:%u:0x%04X:%lu\n", get_highest_layer(layer_state), record->event.key.row, record->event.key.col, keycode, (unsigned long)timer_read32());
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
// QMK callbacks
////////////////////////////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    log_key_event(keycode, record);
#endif
    update_window_switcher(keycode, record);
    update_oneshots(keycode, record);
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, SYM, NAV, NUM);
#ifdef CONSOLE_ENABLE
    uprintf("LL:%u\n", get_highest_layer(state));
#endif
    return state;
}
