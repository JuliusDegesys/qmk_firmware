# Build with: qmk compile -kb ferris/sweep -km juliusdegesys -e CONVERT_TO=kb2040

SRC += oneshot.c

LTO_ENABLE = yes

# Keypress logging for the heatmap. Set to no to remove the logging overhead.
CONSOLE_ENABLE = yes

# Features this keymap doesn't use
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
TAP_DANCE_ENABLE = no

# Start running without waiting for the host to finish USB setup
NO_USB_STARTUP_CHECK = yes
