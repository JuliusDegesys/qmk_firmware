/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/
#pragma once

#undef NKRO_ENABLE

// USD_SUSPEND_WAKEUP_DELAY sets the number of milliseconds to pause after
// sending a wakeup packet.  Disabled by default, you might want to set this to
// 200 (or higher) if the keyboard does not wake up properly after suspending.
#define USB_SUSPEND_WAKEUP_DELAY 0

// Use custom one-shot layer implementation in keymap.c
#define NO_ACTION_ONESHOT

// 8 bits because each layer needs its own bit. So, 8bits=8-layer support
#define LAYER_STATE_8BIT

// Allow plugging into either side.
#define EE_HANDS

// Reduce input lag
#define USB_POLLING_INTERVAL_MS 1

// Faster serial communication between halves
#define SERIAL_USART_SPEED 460800
