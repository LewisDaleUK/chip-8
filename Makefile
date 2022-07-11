.PHONY: build

build:
	gcc -I./includes -I/opt/homebrew/Cellar/sdl2/2.0.22/include/ -L/opt/homebrew/Cellar/sdl2/2.0.22/lib/ -lSDL2 source/*.c -o build/chip-8