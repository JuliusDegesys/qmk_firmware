#!/usr/bin/env python3
"""
Analyze QMK keylog output to show key usage per layer.

Usage:
    # Capture from keyboard (Ctrl+C to stop):
    qmk console | tee keylog.txt

    # Then analyze:
    python3 analyze_usage.py keylog.txt
"""
import sys
from collections import Counter, defaultdict

LAYER_NAMES = {0: "DEF", 1: "SYM", 2: "NAV", 3: "NUM"}

# Standard QMK keycodes (USB HID values)
KEYCODE_NAMES = {
    # Letters
    0x0004: "A", 0x0005: "B", 0x0006: "C", 0x0007: "D", 0x0008: "E",
    0x0009: "F", 0x000A: "G", 0x000B: "H", 0x000C: "I", 0x000D: "J",
    0x000E: "K", 0x000F: "L", 0x0010: "M", 0x0011: "N", 0x0012: "O",
    0x0013: "P", 0x0014: "Q", 0x0015: "R", 0x0016: "S", 0x0017: "T",
    0x0018: "U", 0x0019: "V", 0x001A: "W", 0x001B: "X", 0x001C: "Y",
    0x001D: "Z",
    # Numbers
    0x001E: "1", 0x001F: "2", 0x0020: "3", 0x0021: "4", 0x0022: "5",
    0x0023: "6", 0x0024: "7", 0x0025: "8", 0x0026: "9", 0x0027: "0",
    # Control keys
    0x0028: "ENT", 0x0029: "ESC", 0x002A: "BSPC", 0x002B: "TAB",
    0x002C: "SPC", 0x004C: "DEL",
    # Punctuation
    0x002D: "-", 0x002E: "=", 0x002F: "[", 0x0030: "]", 0x0031: "\\",
    0x0033: ";", 0x0034: "'", 0x0035: "`", 0x0036: ",", 0x0037: ".",
    0x0038: "/",
    # Navigation
    0x004B: "PGUP", 0x004E: "PGDN",
    0x004F: "RIGHT", 0x0050: "LEFT", 0x0051: "DOWN", 0x0052: "UP",
    # Function keys
    0x003A: "F1", 0x003B: "F2", 0x003C: "F3", 0x003D: "F4",
    0x003E: "F5", 0x003F: "F6", 0x0040: "F7", 0x0041: "F8",
    0x0042: "F9", 0x0043: "F10", 0x0044: "F11", 0x0045: "F12",
    # Modifiers
    0x00E0: "LCTL", 0x00E1: "LSFT", 0x00E2: "LALT", 0x00E3: "LGUI",
    # Shifted symbols
    0x021E: "!", 0x021F: "@", 0x0220: "#", 0x0221: "$",
    0x0222: "%", 0x0223: "^", 0x0224: "&", 0x0225: "*",
    0x0226: "(", 0x0227: ")",
    0x022D: "_", 0x022E: "+",
    0x022F: "{", 0x0230: "}", 0x0231: "|",
    0x0233: ":", 0x0234: '"', 0x0235: "~",
    0x0236: "<", 0x0237: ">", 0x0238: "?",
    0x022B: "S(TAB)",
    # GUI combos
    0x0817: "Cmd-T", 0x082C: "Cmd-SPC",
    # GUI+Shift combos
    0x0A17: "Cmd-S-T", 0x0A21: "Cmd-S-4",
}


def keycode_name(kc, custom_map):
    if kc in custom_map:
        return custom_map[kc]
    if kc in KEYCODE_NAMES:
        return KEYCODE_NAMES[kc]
    return f"0x{kc:04X}"


def main():
    if len(sys.argv) > 1:
        f = open(sys.argv[1])
    else:
        f = sys.stdin

    custom_map = {}
    layer_counts = defaultdict(Counter)  # layer -> Counter of keycodes
    pos_counts = defaultdict(Counter)    # layer -> Counter of "row,col"
    total = 0
    has_position_data = False

    for line in f:
        line = line.strip()
        if "MAP:" in line:
            map_idx = line.index("MAP:")
            map_part = line[map_idx:]
            parts = map_part.split(":", 2)
            if len(parts) == 3:
                try:
                    kc = int(parts[1], 16)
                    custom_map[kc] = parts[2]
                except ValueError:
                    pass
        elif "KL:" in line:
            # Strip QMK console prefix (e.g. "DPB:Ferris sweep:1: KL:...")
            kl_idx = line.index("KL:")
            kl_part = line[kl_idx:]
            parts = kl_part.split(":")
            try:
                if len(parts) == 5:
                    # New format: KL:layer:row:col:0xKC
                    layer = int(parts[1])
                    row = int(parts[2])
                    col = int(parts[3])
                    kc = int(parts[4], 16)
                    layer_counts[layer][kc] += 1
                    pos_counts[layer][f"{row},{col}"] += 1
                    has_position_data = True
                    total += 1
                elif len(parts) == 3:
                    # Old format: KL:layer:0xKC
                    layer = int(parts[1])
                    kc = int(parts[2], 16)
                    layer_counts[layer][kc] += 1
                    total += 1
            except ValueError:
                pass

    if f is not sys.stdin:
        f.close()

    if total == 0:
        print("No key events found.")
        print("Make sure the log contains lines like: KL:0:1:2:0x002C")
        return

    print(f"\n{'='*60}")
    print(f"  KEY USAGE REPORT  --  {total:,} total keypresses")
    print(f"{'='*60}\n")

    # Layer summary
    print("LAYER SUMMARY")
    print("-" * 50)
    for layer in sorted(layer_counts):
        name = LAYER_NAMES.get(layer, f"L{layer}")
        count = sum(layer_counts[layer].values())
        pct = count / total * 100
        bar = "#" * int(pct / 2)
        print(f"  {name:>4}  {count:>7,} ({pct:5.1f}%)  {bar}")
    print()

    # Per-layer breakdown by keycode
    for layer in sorted(layer_counts):
        name = LAYER_NAMES.get(layer, f"L{layer}")
        counts = layer_counts[layer]
        layer_total = sum(counts.values())

        print(f"LAYER: {name} ({layer_total:,} keypresses)")
        print("-" * 50)
        for kc, count in counts.most_common():
            pct = count / layer_total * 100
            kname = keycode_name(kc, custom_map)
            bar = "#" * max(1, int(pct / 2))
            print(f"  {kname:<16} {count:>7,} ({pct:5.1f}%)  {bar}")
        print()

    if has_position_data:
        print(f"Position data available. Use the heatmap visualizer:")
        print(f"  open heatmap.html  # then load keylog.txt")


if __name__ == "__main__":
    main()
