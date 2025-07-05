<p align="center">
 <img src="https://github.com/RocketRobz/TWiLightEmu/blob/main/resources/banner.png"><br>
</p>
(Working title, subject to change)

As this is the first emulator of any kind I've developed, I cannot guarentee that everything will work properly, or even work at all.

## Compilation

### Setting up your environment

To build TWiLight Emu++ from source, you will need to install devkitARM and some libraries. Follow devkitPro's [Getting Started](https://devkitpro.org/wiki/Getting_Started) page to install pacman, then run the following command to install everything you need:
```
(sudo dkp-)pacman -S 3ds-dev
```
(The `sudo dkp-` may not be needed depending on your OS)

You will also need [bannertool](https://github.com/Steveice10/bannertool/releases/latest) and [makerom](https://github.com/profi200/Project_CTR/releases/latest) in your PATH.

### Cloning the repository

To download the source you will need to clone the repository with submodules, this can be done by running:
```
git clone --recursive https://github.com/RocketRobz/TWiLightEmu.git
```

If you've already cloned it, you can use the following command to update all of the submodules:
```
git submodule update --init --recursive
```

### Building

Once you've cloned the repository (with submodules), simply run `make` in the root of the repository.

## Credits
* devkitPro: libctru