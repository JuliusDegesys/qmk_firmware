#pragma once

// The oneshot mods are implemented in oneshot.c, so turn off QMK's own.
#define NO_ACTION_ONESHOT

// Only four layers are used, so an 8-bit layer state is enough.
#define LAYER_STATE_8BIT

// Each half stores whether it is left or right, so USB can go into either
// side. See CLAUDE.md for how to set it when flashing.
#define EE_HANDS

// Reduce input lag
#define USB_POLLING_INTERVAL_MS 1

// Faster serial communication between halves
#define SERIAL_USART_SPEED 460800

// Reboot a half that thinks it's the slave if it hears nothing from the
// master. Recovers when the dock powers the keyboard before the host is
// connected and both halves end up as slaves.
#define SPLIT_WATCHDOG_ENABLE
