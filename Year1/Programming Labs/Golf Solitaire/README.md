
## Current Variant

### Golf Solitaire

Features:
- 7 columns × 5 rows layout
- Stock and discard pile
- Adjacent rank rule (±1)
- Ace and King are considered adjacent
- Terminal (CLI) interface
- Unicode playing cards

---

##  Compilation

From the project root directory:

```bash
gcc -Wall -Wextra -Iinclude src/*.c -o golf

Run : ./golf

---

## Controls

j <0-6>   Play top card from column
p         Draw from stock
q         Quit
