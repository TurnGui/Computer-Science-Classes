# Makefile — Etapa 3

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -Iinclude
LDFLAGS = -lcunit

SRC = src/carta.c src/baralho.c src/pilha.c \
      src/paciencia_parser.c \
      src/jogo_estado.c \
      src/jogo_saveload.c

SRC_SIMON = src/simon_regras.c src/simon_estado.c

OBJ      = $(SRC:.c=.o)
OBJ_SIMON = $(SRC_SIMON:.c=.o)

all: paciencia

paciencia: $(OBJ) src/main.o
	$(CC) $(CFLAGS) -o $@ $^

test: $(OBJ) $(OBJ_SIMON) tests/test_etapa3.o tests/test_simon.o
	$(CC) $(CFLAGS) -o test_etapa3 $(OBJ) tests/test_etapa3.o $(LDFLAGS)
	$(CC) $(CFLAGS) -o test_simon  $(OBJ) $(OBJ_SIMON) tests/test_simon.o $(LDFLAGS)
	./test_etapa3
	./test_simon

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

docs:
	doxygen Doxyfile

clean:
	rm -f src/*.o tests/*.o paciencia test_etapa3 test_simon

.PHONY: all test docs clean
