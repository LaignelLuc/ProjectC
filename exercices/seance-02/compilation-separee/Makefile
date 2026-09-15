CFLAGS = -Iinclude -std=c11 -Wall -Wextra -Wpedantic -g

.PHONY: all run test memcheck memcheck-tests memcheck-run memcheck-scenario clean

all: build/stats

build/stats: src/main.c
	mkdir -p build
	cc $(CFLAGS) src/main.c -o build/stats

run: build/stats
	./build/stats

test: build/stats
	./build/stats | diff -u tests/expected-output.txt -

memcheck: memcheck-tests

memcheck-tests: build/stats
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/stats

memcheck-run: build/stats
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/stats

memcheck-scenario: build/stats
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./build/stats

clean:
	rm -rf build
