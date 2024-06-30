BIN_EXECUTABLES := $(patsubst bin/%.c, bin/%, $(wildcard bin/*.c))

all: main $(BIN_EXECUTABLES)

main: main.c
	gcc -o main main.c

bin/%: bin/%.c
	gcc -o $@ $<

clean:
	rm -rf main $(BIN_EXECUTABLES)