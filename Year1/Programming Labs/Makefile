CC     = gcc
CFLAGS = -Wall -Wextra -Iinclude

LOGICA = src/carta.c src/baralho.c src/pilha.c src/simon_regras.c src/simon_estado.c

all: simon

simon: $(LOGICA) src/main.c
	$(CC) $(CFLAGS) -o simon $(LOGICA) src/main.c

test: $(LOGICA) tests/test_simon.c
	$(CC) $(CFLAGS) -o tests/test_logica $(LOGICA) tests/test_simon.c -lcunit
	./tests/test_logica

clean:
	rm -f simon tests/test_logica

.PHONY: all test clean
