# Programming Labs
---
## About
This repository contains the work developed for the **Programming Labs** course. The project is built across 4 stages, each introducing a new card game or feature on top of the previous one.

---

## Project Stages
| Stage | Game | Status |
|-------|------|--------|
| 1 | Golf Solitaire | Done |
| 2 | Simple Simon |  Done |
| 3 | DSL-driven Patience | 🔄 In Progress |
| Final | Full project | ⏳ Pending |

Each stage builds on the previous one — the card logic developed in Stage 1 is reused and extended in every subsequent stage.

---

## Requirements (all stages)
- Written in C, runs on `lambda.di.uminho.pt`
- All functions: cyclomatic complexity ≤ 10
- All functions: ≤ 15 statements
- No global variables
- No `goto`, `break` or `continue`
- Modular code with strict separation between logic and interface
- All logic functions tested with **CUnit**
- All functions documented with **Doxygen**

---

## Project Structure
```
golf/
├── include/        # Header files
├── src/            # Source files
├── tests/          # CUnit tests
├── docs/           # Doxygen generated documentation
├── Makefile
└── Doxyfile
```

---

## How to Run
```bash
# Compile and run the game
make
./simon or ./golf

# Run CUnit tests
make test

# Generate Doxygen documentation
doxygen Doxyfile

# Open documentation in browser
firefox docs/html/index.html

# Clean executables
make clean
```
