# Chip8 Emulator

This is a simple (and deeply flawed) implementation of a Chip8 emulator, using the functionality as described [here](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) and using [Cookerly's Chip8 Emulator](https://github.com/cookerlyk/Chip8/) as inspiration.

## Dependencies

* [SDL2](https://www.libsdl.org/) installed and available to the compiler
* gcc C compiler installed

## Compiling the emulator

The makefile requires two environment variables, `SDLINC` and `SDLLIB`. `SDLINC` is the path to the include directory for SDL2, and `SDLLIB` the path to the lib directory.

Once these are assigned, simply run `make`, which will output a file at `build/chip-8`.

## Running the emulator

Run the binary file with the path to a Chip8 ROM file, for example:

`./build/chip-8 ~/Downloads/IBM.ch8`

You can terminate execution by closing the emulator window.