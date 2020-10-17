PROJ=proj
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
FILES=main.c

.PHONY: all
all: $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ) $(FILES)

.PHONY: clean
clean:
	rm -f *.o $(PROJ)

.PHONY: test
test:
	@echo "TODO"
