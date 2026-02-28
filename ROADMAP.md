# Roadmap

These would be nice to add:

- Validate checksum after flashing (read flash back to temp file and sha1)
    - Alternatively, redo the GB global checksum if that's faster and will help not wear out a chip
- Validate checksum after dumping (read flash back to temp file and sha1)
    - Alternatively, redo the GB global checksum if that's faster and will help not wear out a chip
- Auto set variables on ROM load
- Autodetect variables from cart
- Detect when device removed/changed (poll?)
- Validate ROM can be flashed to cart (see auto-set vars on rom load)
- Validation routines for RAM?
- Grab ROM info/boxart/etc on ROM/cart load from internet/cache
- Migrate to CMake?
- Static libftdi
- macOS support