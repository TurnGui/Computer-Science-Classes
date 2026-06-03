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
# Card Patience Engine — C

A terminal-based implementation of a **card patience game engine**, written in C across 3 progressive stages.

---

## About

This project implements various card solitaire games (Golf, Simple Simon, and more) with a generic engine that can handle any patience game via configuration files. Built as part of the *Programming Laboratories* course.

---

## Stages

| Stage | Game | Goal |
|-------|------|------|
| 1 | Golf Solitaire | Single-game implementation |
| 2 | Simple Simon | Add second game, refactor logic |
| 3 | Generic Engine | Any game via DSL (no code changes) |

Each stage builds on the previous one — code is reused and extended.

---

## Requirements (All Stages)

- **Language:** C (ISO C99)
- **Server:** lambda.di.uminho.pt (SSH access)
- **Cyclomatic complexity:** <= 10 per function
- **Statements:** <= 15 per function
- **No global variables**
- **No `goto`, `break`, `continue`**
- **Modular design:** Logic separated from interface
- **Tests:** CUnit for all logic functions
- **Documentation:** Doxygen on all headers

---

## Project Structure

```
etapa3/
├── include/              # Header files
│   ├── carta.h          # Card (stage 1)
│   ├── baralho.h        # Deck (stage 1)
│   ├── pilha.h          # Stack (stage 1)
│   ├── simon_*.h        # Simon logic (stage 2)
│   ├── paciencia.h      # DSL structures (stage 3)
│   ├── paciencia_parser.h
│   ├── jogo_estado.h
│   └── jogo_saveload.h
├── src/                 # Implementation
├── paciencias/          # Game configurations (.paciencia files)
├── tests/               # CUnit tests
├── Makefile
└── Doxyfile
```

---

## Build & Run

```bash
# Compile
make

# Run the game
./paciencia

# Run tests
make test

# Generate documentation
doxygen Doxyfile

# Clean build
make clean
```

---

## Commands

```
m <from> <to> <n>    Move n cards from pile to pile
u                    Undo last move
s <file>             Save game state
l <file>             Load game state
q                    Quit
```

---

## Stage 3 — Generic Engine

The big feature: games are now **configuration files**, not code.

A game is described in ~15 lines:

```
JOGO SimpleSimon
BARALHOS 1
TIPO TAB =
TIPO FUND =
INIT TAB 8
INIT FUND 0
MOV TAB TAB <
MOV TAB TAB +[m<
AUTO TAB FUND +[mKaV
WIN TAB 0
```

Want a new game? Create a new `.paciencia` file. No C code changes needed.

---

## Test Results

Stage 3:
- 12 CUnit tests, 12 passed
- All functions: complexity <= 10, statements <= 15

```bash
make test
```

---

## Key Concepts

**Bitmasks for flags:** Game rules can have multiple constraints (descending, same suit, etc). We use bitwise operations instead of complex structures.

**Lookup table parser:** The DSL parser uses a lookup table instead of a giant switch statement. Reduces complexity from 22 to 2.

**Snapshots for undo:** Game state is copied before each move. Undo just restores a previous copy.

---

## Files to Know

| File | Purpose |
|------|---------|
| `paciencia.h` | Game rule structures |
| `paciencia_parser.c` | Reads .paciencia files |
| `jogo_estado.c` | Game engine (validates moves, checks win) |
| `main.c` | Terminal UI |

---

## Code Quality

Verified with `pmccabe`:

- All functions have cyclomatic complexity <= 10
- All functions have <= 15 statements
- No global mutable variables
- Clean logic/interface separation
- Full Doxygen documentation

---

## Authors

Group LCCPL1G01
