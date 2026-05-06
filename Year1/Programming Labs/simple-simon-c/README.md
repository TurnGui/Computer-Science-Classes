# Simple Simon — Stage 2
---
## About
Simple Simon is a solitaire card game implemented in C as Stage 2 of the Programming Labs project. It reuses the card and deck logic from Stage 1 (Golf Solitaire) and introduces a generic stack structure to handle variable-size columns.

---

## Game Rules
- 10 tableau columns (TAB) are dealt from a shuffled deck: 8, 8, 8, 7, 6, 5, 4, 3, 2, 1 cards
- 4 foundations (FUND) start empty
- A card can be moved to another column if its value is immediately lower than the top card of the destination
- A sequence of cards of the same suit in descending order can be moved together
- Any card or sequence can be moved to an empty column
- When a complete K→A sequence of the same suit is formed, it is automatically moved to an empty foundation
- **Win condition:** all tableau columns are empty

---

## Modules
| Module | Description |
|--------|-------------|
| `carta` | Card structure and basic operations — reused from Stage 1 |
| `baralho` | Deck of 52 cards with shuffle — reused from Stage 1 |
| `pilha` | Generic fixed-size card stack (push, pop, peek) |
| `simon_regras` | Game rules — sequence validation, move validation |
| `simon_estado` | Full game state — initialization, moves, auto, win check |

---

## How to Run
```bash
# Compile and run
make
./simon

# Run CUnit tests
make test

# Generate Doxygen documentation
doxygen Doxyfile

# Open documentation in browser
firefox docs/html/index.html

# Clean executables
make clean
```

---

## Commands
| Command | Description |
|---------|-------------|
| `m <orig> <dest> <n>` | Move n cards from column orig to column dest |
| `q` | Quit the game |

**Example:** `m 0 1 1` moves 1 card from column 0 to column 1.

---

## Testing
16 CUnit tests across 3 suites covering all logic functions:

| Suite | Tests |
|-------|-------|
| `pilha` | inicializar, push/pop, carta, cheia |
| `simon_regras` | mesmo_naipe, valor_inferior, sequencia_valida, pode_mover, sequencia_completa |
| `simon_estado` | inicializar, ganhou, pode_jogar, jogar, tem_jogada, auto |
