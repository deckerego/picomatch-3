# PicoSystem Match 3

A match 3 game for the Pimoroni PicoSystem.

## Copying Match3 to your PicoSystem

Download [the latest release from the GitHub releases page](https://github.com/deckerego/picomatch-3/releases), then connect your PicoSystem to your computer using a USB Type-C cable.

From a power-off state, hold down X (the top face button) and press Power (the button at the top left, next to the USB Type-C port).

Your PicoSystem should mount as "RPI-RP2". On macOS this might be `/Volumes/RPI-RP2`:

```
cp match3.app.uf2 /Volumes/RPI-RP2
```

The file should copy over, and your PicoSystem should automatically reboot into your game.

## Building

### Compilers & Libraries with macOS

You'll need a compiler and a few other dependencies to get started building C++
for PicoSystem. With macOS that will include installing xcode (even if you
don't plan on using xcode). After that the process will be:

1. Adding the xcode build tools: `xcode-select --install`
1. Adding OSS build libraries: `brew install cmake`
1. Installing 32blit tools: `pip3 install 32blit`
1. Installing the ARM cross-compile toolchain from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads (this could be done from brew, but I prefer a system-wide install)
1. Add the ARM tools directory to your current PATH. As an example, you can add `~/.zprofile` and add `export PATH="$PATH:/System/Volumes/Data/Applications/ARM/bin"` at the end for the default install.
1. Clone 32blit SDK: https://github.com/raspberrypi/pico-sdk.git
1. Change into the cloned directory and update submodules as needed: `git submodule update --init`
1. Clone Pico SDK: git clone -b master https://github.com/raspberrypi/pico-sdk.git
1. Change into the cloned directory and update submodules as needed: `git submodule update --init`
1. Clone Pico Extras: git clone -b master https://github.com/raspberrypi/pico-extras.git
1. Change into the cloned directory and update submodules as needed: `git submodule update --init`
1. Move all the SDKs into `/opt` (again, I'm a fan of system-wide installs)

### Building with SDL

To test on your local machine before copying to the PicoSystem, build an
SDL port of your app with:

```
mkdir build.sdl
cd build.sdl
cmake ..
make
```

### Building for the PicoSystem

If you've got local copies of the Pico SDK, Pico Extras and 32blit SDK alongside your project,
then you can configure and build your .uf2 like so:

```
mkdir build.pico
cd build.pico
cmake .. -D32BLIT_DIR=/opt/32blit-sdk -DPICO_SDK_PATH=/opt/pico-sdk -DCMAKE_TOOLCHAIN_FILE=/opt/32blit-sdk/pico.toolchain -DPICO_BOARD=pimoroni_picosystem
make
```
