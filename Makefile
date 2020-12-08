PROJ=proj
BINARY_FILES=$(PROJ) test-scanner test-ilist test-expParser test-expression
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic -pthread
FILES=src/str.c src/scanner.c src/symtable.c src/ilist.c src/stack.c src/expressionParser.c src/expression.c 

.PHONY: all
all: $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ) $(FILES) src/main.c

.PHONY: clean
clean:
	@rm -f *.o $(BINARY_FILES)

.PHONY: build-test-scanner
build-test-scanner:
	$(CC) $(CFLAGS) -o test-scanner tests/test_scanner.c src/scanner.c src/str.c

.PHONY: build-test-ilist
build-test-ilist:
	$(CC) $(CFLAGS) -o test-ilist tests/test_ilist.c src/ilist.c src/symtable.c src/str.c

.PHONY: build-test-expParser
build-test-expParser:
	$(CC) $(CFLAGS) -o test-expParser tests/test_expParser.c src/ilist.c src/symtable.c src/str.c src/expressionParser.c

.PHONY: build-test-expression
build-test-expression:
	$(CC) $(CFLAGS) -o test-expression tests/test_expression.c src/stack.c src/ilist.c src/symtable.c src/str.c src/expressionParser.c src/expression.c
