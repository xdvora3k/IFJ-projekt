PROJ=proj
BINARY_FILES=$(PROJ) test-scanner
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
FILES=src/str.c src/scanner.c

.PHONY: all
all: $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ) $(FILES) src/main.c

.PHONY: clean
clean:
	@rm -f *.o $(BINARY_FILES)

.PHONY: build-test
build-test:
	$(CC) $(CFLAGS) -o test-scanner $(FILES) tests/test_scanner.c
