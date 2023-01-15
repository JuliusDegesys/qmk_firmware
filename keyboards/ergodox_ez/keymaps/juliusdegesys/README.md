# QMK Config for a 34-key layout on an Ergodox EZ.

(Well, technically, this is as 35-key layout because there is a reset key)

A ton of this is based off of callum's work (see oneshot, swapper, and the
layout in users/callum/)

I've changed up a few items for my purposes.


Here are some initial pointers to get you started:

- Use the documentation at [https://docs.qmk.fm/](https://docs.qmk.fm/) to set
  up your environment for building your firmware.
- Build your layout against
  [https://github.com/zsa/qmk_firmware/](https://github.com/zsa/qmk_firmware/)
which is our QMK fork (instead of qmk/qmk_firmware). This is what Oryx (the
graphical configurator) uses, so it's guaranteed to work.
- Create a folder with a simple name (no spaces!) for your layout inside the
  qmk_firmware/keyboards/ergodox_ez/glow/keymaps/ folder.
- Copy the contents of the \*\_source folder (in the .zip you downloaded from
  Oryx) into this folder.
- Make sure you've set up your environment per the [QMK
  docs](https://docs.qmk.fm/#/newbs_getting_started?id=set-up-your-environment)
so compilation would actually work.
- From your shell, make sure your working directory is qmk*firmware, then enter
  the command `make ergodox_ez/glow:_layout_`, substituting the name of the
folder you created for "_layout_".

Good luck on your journey! And remember, if you get stuck, you can always get
back to your [original
layout](https://configure.zsa.io/ergodox-ez/layouts/P7QPL/xyjeY/0) from Oryx.
