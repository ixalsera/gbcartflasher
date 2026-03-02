# GB Cart Flasher

A Qt-based Game Boy cartridge dumper and flasher.

---

# Table of Contents

- [Dependencies](#dependencies)
- [Building](#building)
    - [Linux](#linux)
    - [Windows](#windows)
    - [macOS](#macos)
- [Contributing](#contributing)
- [Maintainers](#maintainers)
- [Thanks](#thanks)
- [Licence](#licence)

---

# Dependencies

- Qt 5 (Core and Widgets)
    - While it has not been tested, Qt 6 may also work
- libftdi (Unix)
- libftd2xx (Windows)

---

# Building

## Linux

Install the required packages using your package manager. Below is an example install for Ubuntu:

```shell
sudo apt install build-essential make qt5-default libftdi-dev
```

From the project root, simply run `make` to configure and build the project.

> __Note:__<br>
> Currently, the project doesn't support calling `make` with the parallel runners flag (`-j`).
> It will, however, run the actual build step using `-j$(nproc)` for a faster compile.

In order to use the cart flasher over USB, you will need to install the provided `udev` rules file and make sure your
user is in the `plugdev` group.

```shell
sudo cp 90-gbflash.rules /usr/lib/udev/rules.d
sudo udevadm control --reload-rules
sudo udevadm trigger
sudo adduser $USER plugdev
```

You will need to log out and back in again for the group changes to take effect.

## Windows

Instructions coming soon! :)

## macOS

Instructions coming soon! :)

## Cross-compiling

Instructions coming soon! :)

---

# Contributing

Please report any issues using [GitHub Issues](https://github.com/ixalsera/gbcartflasher/issues). PRs are welcome.

---

# Maintainers

Original authors (~2008 - ~2013):

- Kraku - Application, concept and artwork
- Chroost - Device design and manufacture

Current maintainer (2026 - ):

- [Xerat0nin](https://github.com/ixalsera)

---

# Thanks

- Kraku and Chroost for the original device and project concept and all the hard work they put in to the project to get
  it in to our hands
- Javier Rodrigo for creating their own cart flasher (which introduced me to this app)
- The entire Game Boy modding and homebrew scene, without whom none of the information we have today would exist

---

# Licence

GB Cart Flasher is released under the GNU General Public License v2.0. You can find the full licence [here](LICENSE).