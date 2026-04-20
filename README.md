
#  Tic Tac Toe (Raylib - C)

A fully interactive **Tic Tac Toe game** built in **C using Raylib**, featuring both **Player vs Player** and **Player vs Computer** modes with multiple difficulty levels.

---

##  Features

* Player vs Player (PVP) mode
* Player vs Computer (PVC) mode
* Two difficulty levels:

  * Easy (random moves)
  * Hard (smart AI logic)
*  Mouse +  Keyboard input support
*  Score tracking (Wins / Losses / Draws)
*  Restart game anytime

---

##  Controls

### Mouse

* Click on grid to place your mark
* Click buttons for menu navigation

### Keyboard

* Keys `1 - 9` → Select grid cells
* `E` → Easy mode
* `H` → Hard mode

---

##  AI Logic

* **Easy Mode:**
  Random valid moves

* **Hard Mode:**

  * Plays winning move if available
  * Blocks opponent's winning move
  * Prioritizes center → corners → sides

---

##  Screens

* Main Menu
* Difficulty Selection
* Game Board

---

##  Requirements

* C Compiler (GCC / Clang)
* Raylib library installed

---

##  Installation & Setup

### 1. Install Raylib

Follow instructions from:
 [https://www.raylib.com/](https://www.raylib.com/)

---

### 2. Compile the Program

```bash
gcc main.c -o tictactoe -lraylib -lm -lpthread -ldl -lrt -lX11
```

---

### 3. Run the Game

```bash
./tictactoe
```

---

##  Project Structure

```
main.c        # Complete game source code
README.md     # Project documentation
```

---

##  How It Works

* The board is stored as a **3x3 character array**
* Game logic checks:

  * Rows
  * Columns
  * Diagonals
* AI uses simple decision-making functions:

  * `bestmove()`
  * `hardmove()`
* UI is rendered using **Raylib drawing functions**

---

##  Game Rules

* Player 1 → `X`
* Player 2 / Computer → `O`
* First to align 3 marks wins
* If all cells are filled → Draw

