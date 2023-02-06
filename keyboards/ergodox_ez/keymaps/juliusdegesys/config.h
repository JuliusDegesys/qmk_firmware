/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/
#pragma once

#undef NKRO_ENABLE

#define USB_SUSPEND_WAKEUP_DELAY 0
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

#define OFF {0, 0, 0}
#define CYAN {0, 222, 238}
#define RED {255, 0, 0}
#define GREEN {66, 255, 0}
#define PURP {204, 29, 197}
#define YELLOW {255, 255, 0}
#define WHITE {255, 255, 255}

//    _______, 28, 27, 26, 25, 24, _______,      _______, 0 , 1 , 2 , 3 , 4 , _______,
//    _______, 33, 32, 31, 30, 29, _______,      _______, 5 , 6 , 7 , 8 , 9 , _______,
//    _______, 38, 37, 36, 35, 34,                        10, 11, 12, 13, 14, _______,
//    _______, 43, 42, 41, 40, 39, _______,      _______, 15, 16, 17, 18, 19, _______,
//    _______, 47, 46, 45, 44,                                20, 21, 22, 23, _______,

#define LAYOUT_LED(                                                \
    K00, K01, K02, K03, K04,               K05, K06, K07, K08, K09, \
    K10, K11, K12, K13, K14,               K15, K16, K17, K18, K19, \
    K20, K21, K22, K23, K24,               K25, K26, K27, K28, K29, \
                   K30,                         K31                 \
)                                                                   \
{OFF, OFF, OFF, OFF, OFF, K05, K06, K07, K08, K09, K15, K16, K17, K18, K19, K25, K26, K27, K28, K29, K31, OFF, OFF, OFF, \
    OFF, OFF, OFF, OFF, OFF, K04, K03, K02, K01, K00, K14, K13, K12, K11, K10, K24, K23, K22, K21, K20, K30, OFF, OFF, OFF}
