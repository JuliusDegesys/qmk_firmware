/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/
#pragma once

//#define ORYX_CONFIGURATOR
#define IGNORE_MOD_TAP_INTERRUPT
#undef TAPPING_TERM
#define TAPPING_TERM 365

#define USB_SUSPEND_WAKEUP_DELAY 0
#define FIRMWARE_VERSION u8"P7QPL/xyjeY"
#define RAW_USAGE_PAGE 0xFF60
#define RAW_USAGE_ID 0x61
#define LAYER_STATE_8BIT

#define RGB_MATRIX_STARTUP_SPD 60

// The below makes specifying a 34 key layout (excluding the reboot key) easier
// to specify
#define LAYOUT_ez34(                                                \
    K00, K01, K02, K03, K04,               K05, K06, K07, K08, K09, \
    K10, K11, K12, K13, K14,               K15, K16, K17, K18, K19, \
    K20, K21, K22, K23, K24,               K25, K26, K27, K28, K29, \
                   K30, K31,               K32, K33                 \
)                                                                   \
LAYOUT_ergodox_pretty(                                              \
    _______, _______, _______, _______, _______ , _______, QK_BOOT,      _______, _______, _______, _______, _______, _______, _______, \
    _______, K00    , K01    , K02    , K03     , K04    , _______,      _______, K05    , K06    , K07    , K08    , K09    , _______, \
    _______, K10    , K11    , K12    , K13     , K14    ,                        K15    , K16    , K17    , K18    , K19    , _______, \
    _______, K20    , K21    , K22    , K23     , K24    , _______,      _______, K25    , K26    , K27    , K28    , K29    , _______, \
    _______, _______, _______, _______, K30     ,                                          K33    , _______, _______, _______, _______, \
                                                  _______, _______,      _______, _______,                                              \
                                                           _______,      _______,                                                       \
                                        K31     , _______, _______,      _______, _______, K32                                          \
)

