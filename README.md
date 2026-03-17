# Pac-Man

A simple console Pac-Man game in C++. Collect all the dots and avoid the ghosts.

---

## How to run

**Build (CMake):**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**Run:** From the project root, run the executable (e.g. `./build/pacman` or `build\pacman.exe`). The game reads the level from `map.txt` in the parent directory.

**Controls:** `W` up, `A` left, `S` down, `D` right.

---

## Main code

| File | Purpose |
|------|--------|
| **`pacman.cpp`** | Game logic: map loading, player/ghost movement, win/lose checks, main loop. |
| **`colours.h`** / **`colours.cpp`** | Console colours and `getch()` for Windows, macOS, and Linux. |
| **`map.txt`** | Level layout (see below). |

### Map format (`map.txt`)

- **`P`** — Pac-Man (player)  
- **`G`** — Ghosts (two per level)  
- **`W`** — Walls  
- **`.`** — Dots (collect all to win)

The game uses an 11×11 grid: the first and last row/column are treated as walls; the 9×9 inner part is read from `map.txt`.

### Game flow (`pacman.cpp`)

1. **Setup** — Allocate two 11×11 grids: `processMap` (current state: P, G, W, dots) and `dotMap` (which cells originally had dots, for redrawing and win condition).
2. **Load level** — Read `map.txt` into `processMap` and fill `dotMap` with dots where applicable.
3. **Main loop** — Each turn:
   - Read key with `getch()`.
   - **`playerMoving()`** — Find P, check `isWall()` and “no ghost” in the move direction, then update `processMap` and `dotMap`.
   - **`ghostMoving()`** — For each ghost: if it shares a row or column with P and has line-of-sight (no wall between), move one step toward P; otherwise move randomly. Ghosts don’t stack.
   - **`printMap()`** — Restore dots from `dotMap` (so dots reappear where P/ghosts moved), then print the map with **`colourChange()`** (P=yellow, ghosts=pink, dots=white, walls=blue).
   - **`winCheck(dotMap)`** — If no dots remain → win.  
   - **`loseCheck(processMap)`** — If P is gone (eaten by a ghost) → lose.

Win or lose ends the loop; one more keypress then exits.

---

*Author: Wallace Su — December 2022*
