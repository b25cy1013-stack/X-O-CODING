#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 600
#define HEIGHT 720
#define CELL_SIZE 200
#define MENU 0
#define difficulty 1
#define GAME 2
#define MODE_NONE 0
#define MODE_PVP 1
#define MODE_PVC 2
#define EASY 1
#define HARD 2
#define wclose WindowShouldClose
#define keypres IsKeyPressed

char board[3][3];

int scoreP1 = 0;
int scoreP2 = 0;
int draws = 0;

int player = 1;
int screen = MENU;
int mode = MODE_NONE;
int level = EASY;
int result = 0;

char winnerMark = 0;

Rectangle btnPVP = {150, 200, 300, 50};
Rectangle btnPVC = {150, 300, 300, 50};
Rectangle btnEasy = {150, 200, 300, 50};
Rectangle btnHard = {150, 300, 300, 50};
Rectangle btnNewGame = {110, 660, 170, 40};
Rectangle btnMenu = {320, 660, 170, 40};

void clearBoard() {
    int row;
    int col;

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            board[row][col] = 0;
        }
    }
}

void resetRound() {
    clearBoard();
    player = 1;
    result = 0;
    winnerMark = 0;
}

void goToMenu() {
    screen = MENU;
    mode = MODE_NONE;
    resetRound();
}

void startPVPGame() {
    mode = MODE_PVP;
    screen = GAME;
    resetRound();
}

void pvcs() {
    mode = MODE_PVC;
    screen = difficulty;
    resetRound();
}

void startPVCGame(int selectedLevel) {
    level = selectedLevel;
    screen = GAME;
    resetRound();
}

int rowFromChoice(int choice) {
    if (choice >= 1 && choice <= 3) return 0;
    if (choice >= 4 && choice <= 6) return 1;
    if (choice >= 7 && choice <= 9) return 2;
    return -1;
}

int colFromChoice(int choice) {
    if (choice == 1 || choice == 4 || choice == 7) return 0;
    if (choice == 2 || choice == 5 || choice == 8) return 1;
    if (choice == 3 || choice == 6 || choice == 9) return 2;
    return -1;
}

bool validChoice(int choice) {
    if (choice < 1) return false;
    if (choice > 9) return false;
    return true;
}

bool cellEmpty(int choice) {
    int row;
    int col;

    if (!validChoice(choice)) {
        return false;
    }

    row = rowFromChoice(choice);
    col = colFromChoice(choice);

    if (row == -1 || col == -1) {
        return false;
    }

    if (board[row][col] == 0) {
        return true;
    }

    return false;
}
