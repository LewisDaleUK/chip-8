.PHONY: build

build:
	gcc -I./includes -I${SDLINC} -L${SDLLIB} -lSDL2 source/*.c -o build/chip-8