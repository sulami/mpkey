# MPKey

A simple XCB-based hotkey manager for MPD

## Installation

Just run `make`.

`libmpdclient-devel` and `xcb-util-keysyms-devel` are required, the names vary
depending on your distro.

## Configuration

Configuration is done by editing `config.h` prior to compiling. Edit the hotkey
list `hks[]` to add the keys and commands you want to use.

