BIN_EXECUTABLES := $(patsubst bin/%.c, bin/%, $(wildcard bin/*.c))

all: vensh $(BIN_EXECUTABLES)

vensh: vensh.c
	gcc -o vensh vensh.c

bin/%: bin/%.c
	gcc -o $@ $<

clean:
	rm -rf vensh $(BIN_EXECUTABLES)