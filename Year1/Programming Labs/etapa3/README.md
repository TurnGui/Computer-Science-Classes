# Card Patience Engine — C

A terminal-based implementation of a **generic card patience engine**, written in C as part of the *Programming Laboratories* course.

---

## The Idea

Instead of hardcoding the rules of each game, we built a **generic engine** that reads game descriptions from simple text files (DSL). Any patience game is now just a configuration file — no code changes needed.

---

## Project Structure

```
etapa3/
├── include/
│   ├── carta.h              # Card type (reused from stage 1)
│   ├── baralho.h            # Deck type (reused from stage 1)
│   ├── pilha.h              # Stack type (reused from stage 1)
│   ├── paciencia.h          # Game description structures
│   ├── paciencia_parser.h   # DSL parser
│   ├── jogo_estado.h        # Generic game state
│   └── jogo_saveload.h      # Save/load
├── src/
│   ├── carta.c              # Card logic
│   ├── baralho.c            # Deck logic
│   ├── pilha.c              # Stack logic
│   ├── paciencia_parser.c   # Parser (reads .paciencia files)
│   ├── jogo_estado.c        # Game engine (generic)
│   ├── jogo_saveload.c      # Persistence
│   └── main.c               # Terminal interface
├── paciencias/
│   ├── golf.paciencia       # Golf Solitaire rules (DSL)
│   └── simon.paciencia      # Simple Simon rules (DSL)
└── tests/
    └── test_etapa3.c        # CUnit tests (12/12 passed)
```

---

## Build & Run

```bash
# Compile
make

# Run
./paciencia

# Run tests
make test
```

---

## How to Play

```
Choose a patience game (Golf or Simon)
Boards shows:
  [0] 3C KS QS KH 3D
  [1] QH 2S 3S 9H AH
  ...

Commands:
  m <from> <to> <n>    Move n cards from pile 'from' to pile 'to'
  u                    Undo last move
  s <file>             Save game state
  l <file>             Load game state
  q                    Quit
```

---

## What is a .paciencia File?

A patience game is described in 10-20 lines:

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

**Commands:**
- `JOGO <name>` — game name
- `BARALHOS <n>` — number of decks
- `TIPO <name> <flags>` — pile type (= all visible, _ none visible, etc)
- `INIT <type> <n>` — create a pile with n cards
- `MOV <from> <to> <flags>` — manual move rule
- `AUTO <from> <to> <flags>` — automatic move rule
- `WIN <type> <n>` — win condition

**Move flags:**
- `*` always allowed
- `+` sequence of cards
- `[` descending, `]` ascending
- `<` lower rank, `>` higher rank
- `m` same suit
- `V` empty destination

---

## Adding a New Game

1. Create `paciencias/my_game.paciencia`:

```
JOGO MyGame
BARALHOS 1
TIPO PILE =
INIT PILE 13
MOV PILE PILE <
WIN PILE 0
```

2. Run `make` and `./paciencia`

3. Choose your new game

No C code was changed.

---

## Code Quality

Verified with `pmccabe`:

- **Cyclomatic complexity** <= 10 on all functions
- **Statements** <= 15 on all functions
- No global variables
- No `goto`, `break` or `continue`
- Clean separation between logic and interface

---

## Test Results

```
make test

Suite: Paciencia Parser     5/5  passed
Suite: Game State           4/4  passed
Suite: Undo                 2/2  passed
Suite: Save/Load            1/1  passed

Run Summary: 12 tests, 12 passed, 0 failed
```

---

## Key Files

| File | Purpose |
|------|---------|
| `paciencia.h` | Game structures (DSL definition) |
| `paciencia_parser.c` | Parser (reads .paciencia files) |
| `jogo_estado.c` | Game engine (validates moves, checks win) |
| `jogo_saveload.c` | Save/load state |
| `main.c` | Terminal UI |

---
