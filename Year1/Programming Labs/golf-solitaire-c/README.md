# Golf Solitaire — C

A terminal-based implementation of the **Golf Solitaire** card game, written in C as part of the *Programming Laboratories* course.

---

## The Game

Golf Solitaire is a single-player card game. The goal is to clear all 7 columns of cards from the table by playing cards that are one rank above or below the current discard pile. If no moves are available, you can draw from the stock. The game ends when you clear the table (win) or the stock runs out with no valid moves left (loss).

---

## Project Structure

```
golf/
├── include/
│   ├── carta.h          # Card type and functions
│   ├── baralho.h        # Deck type and functions
│   ├── golf.h           # Game rules
│   └── golf_estado.h    # Game state
├── src/
│   ├── carta.c          # Card logic
│   ├── baralho.c        # Deck logic
│   ├── golf.c           # Rule logic
│   ├── golf_estado.c    # State management
│   └── main.c           # Terminal interface
└── tests/
    └── test_logica.c    # CUnit tests for all logic functions
```

---

## Build & Run

```bash
# Compile
gcc -Wall -Wextra -I include src/*.c -o golf

# Run
./golf
```

---

## Controls

| Command | Action |
|---------|--------|
| `j <0-6>` | Play card from column (e.g. `j 3`) |
| `p` | Draw from stock |
| `q` | Quit |

---

## Running Tests

Requires [CUnit](https://cunit.sourceforge.net).

```bash
# Install CUnit (Ubuntu/Debian)
sudo apt-get install libcunit1 libcunit1-dev

# Compile tests
gcc -Wall -Wextra -I include \
    src/carta.c src/baralho.c src/golf.c src/golf_estado.c \
    tests/test_logica.c \
    -lcunit -o tests/test_logica

# Run
./tests/test_logica
```

Expected output:
```
Suite: carta         4/4  passed
Suite: baralho       5/5  passed
Suite: golf_regras   2/2  passed
Suite: golf_estado  13/13 passed

Run Summary: 24 tests, 24 passed, 0 failed
```

---

## Code Quality

Verified with `pmccabe` as requested by the professor :

- **Cyclomatic complexity** ≤ 10 on all functions
- **Statements** ≤ 15 on all functions
- No global variables
- No `goto`, `break` or `continue`
- Clean separation between logic (`carta`, `baralho`, `golf`, `golf_estado`) and interface (`main`)

---
