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

bool putMarkInBoard(int choice, char mark) {
    int row;
    int col;

    if (!cellEmpty(choice)) {
        return false;
    }

    row = rowFromChoice(choice);
    col = colFromChoice(choice);

    board[row][col] = mark;
    return true;
}

char markNow() {
    if (player == 1) {
        return 'X';
    }

    return 'O';
}

void switchPlayer() {
    if (player == 1) {
        player = 2;
    } else {
        player = 1;
    }
}

char checkRows() {
    int row;

    for (row = 0; row < 3; row++) {
        if (board[row][0] == 0) {
            continue;
        }

        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            return board[row][0];
        }
    }

    return 0;
}

char checkCols() {
    int col;

    for (col = 0; col < 3; col++) {
        if (board[0][col] == 0) {
            continue;
        }

        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            return board[0][col];
        }
    }

    return 0;
}

char checkDiagonals() {
    if (board[0][0] != 0) {
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            return board[0][0];
        }
    }

    if (board[0][2] != 0) {
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            return board[0][2];
        }
    }

    return 0;
}

char checkWinner() {
    char result;

    result = checkRows();
    if (result != 0) return result;

    result = checkCols();
    if (result != 0) return result;

    result = checkDiagonals();
    if (result != 0) return result;

    return 0;
}

bool isBoardFull() {
    int row;
    int col;

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }

    return true;
}

void checkGameState() {
    winnerMark = checkWinner();

    if (winnerMark == 'X') {
        result = 1;
        scoreP1++;
        return;
    }

    if (winnerMark == 'O') {
        result = 1;
        scoreP2++;
        return;
    }

    if (isBoardFull()) {
        result = 2;
        draws++;
        return;
    }

    result = 0;
}

bool moveWins(int choice, char mark) {
    int row;
    int col;
    char tempWinner;

    if (!cellEmpty(choice)) {
        return false;
    }

    row = rowFromChoice(choice);
    col = colFromChoice(choice);

    board[row][col] = mark;
    tempWinner = checkWinner();
    board[row][col] = 0;

    if (tempWinner == mark) {
        return true;
    }

    return false;
}

int bestMoveFor(char mark) {
    int spot;

    for (spot = 1; spot <= 9; spot++) {
        if (moveWins(spot, mark)) {
            return spot;
        }
    }

    return -1;
}

int easyMove() {
    int spot;

    while (1) {
        spot = (rand() % 9) + 1;

        if (cellEmpty(spot)) {
            return spot;
        }
    }
}

int hardMove() {
    int spot;

    spot = bestMoveFor('O');
    if (spot != -1) {
        return spot;
    }

    spot = bestMoveFor('X');
    if (spot != -1) {
        return spot;
    }

    if (cellEmpty(5)) {
        return 5;
    }

    if (cellEmpty(1)) return 1;
    if (cellEmpty(3)) return 3;
    if (cellEmpty(7)) return 7;
    if (cellEmpty(9)) return 9;

    for (spot = 1; spot <= 9; spot++) {
        if (cellEmpty(spot)) {
            return spot;
        }
    }

    return -1;
}

int c_move() {
    if (level == EASY) {
        return easyMove();
    }

    return hardMove();
}

int getDifficultyChoice() {
    if (keypres(KEY_E)) return EASY;
    if (keypres(KEY_H)) return HARD;
    return -1;
}

const char *modeLabel() {
    if (level == HARD) {
        return "Hard";
    }

    return "Easy";
}

int getKeyboardChoice() {
    if (keypres(KEY_ONE)) return 1;
    if (keypres(KEY_KP_1)) return 1;

    if (keypres(KEY_TWO)) return 2;
    if (keypres(KEY_KP_2)) return 2;

    if (keypres(KEY_THREE)) return 3;
    if (keypres(KEY_KP_3)) return 3;

    if (keypres(KEY_FOUR)) return 4;
    if (keypres(KEY_KP_4)) return 4;

    if (keypres(KEY_FIVE)) return 5;
    if (keypres(KEY_KP_5)) return 5;

    if (keypres(KEY_SIX)) return 6;
    if (keypres(KEY_KP_6)) return 6;

    if (keypres(KEY_SEVEN)) return 7;
    if (keypres(KEY_KP_7)) return 7;

    if (keypres(KEY_EIGHT)) return 8;
    if (keypres(KEY_KP_8)) return 8;

    if (keypres(KEY_NINE)) return 9;
    if (keypres(KEY_KP_9)) return 9;

    return -1;
}

int mouseChoice(Vector2 mousep) {
    if (mousep.x < 0 || mousep.x >= WIDTH) {
        return -1;
    }

    if (mousep.y < 0 || mousep.y >= 600) {
        return -1;
    }

    if (mousep.x < 200 && mousep.y < 200) return 1;
    if (mousep.x >= 200 && mousep.x < 400 && mousep.y < 200) return 2;
    if (mousep.x >= 400 && mousep.y < 200) return 3;

    if (mousep.x < 200 && mousep.y >= 200 && mousep.y < 400) return 4;
    if (mousep.x >= 200 && mousep.x < 400 && mousep.y >= 200 && mousep.y < 400) return 5;
    if (mousep.x >= 400 && mousep.y >= 200 && mousep.y < 400) return 6;

    if (mousep.x < 200 && mousep.y >= 400) return 7;
    if (mousep.x >= 200 && mousep.x < 400 && mousep.y >= 400) return 8;
    if (mousep.x >= 400 && mousep.y >= 400) return 9;

    return -1;
}

bool buttonClicked(Rectangle button, Vector2 mousep) {
    if (CheckCollisionPointRec(mousep, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return true;
    }

    return false;
}

void drawButton(Rectangle button, const char *text) {
    Color fillColor;
    int textWidth;
    int textX;
    int textY;

    if (CheckCollisionPointRec(GetMousePosition(), button)) {
        fillColor = GRAY;
    } else {
        fillColor = LIGHTGRAY;
    }

    DrawRectangleRec(button, fillColor);
    DrawRectangleLinesEx(button, 2, DARKGRAY);

    textWidth = MeasureText(text, 20);
    textX = (int)(button.x + (button.width / 2) - (textWidth / 2));
    textY = (int)(button.y + (button.height / 2) - 10);

    DrawText(text, textX, textY, 20, BLACK);
}

void drawMenu() {
    DrawText("Select Mode", 200, 100, 30, BLACK);
    drawButton(btnPVP, "Player vs Player");
    drawButton(btnPVC, "Player vs Computer");
    DrawText("PVC lets you choose Easy or Hard next", 105, 380, 20, DARKGRAY);
}

void drawGrid() {
    DrawLine(200, 0, 200, 600, BLACK);
    DrawLine(400, 0, 400, 600, BLACK);
    DrawLine(0, 200, 600, 200, BLACK);
    DrawLine(0, 400, 600, 400, BLACK);
}

void drawCellMark(int row, int col) {
    int x;
    int y;

    x = col * 200 + 80;
    y = row * 200 + 80;

    if (board[row][col] == 'X') {
        DrawText("X", x, y, 60, RED);
    }

    if (board[row][col] == 'O') {
        DrawText("O", x, y, 60, BLUE);
    }
}

void drawMarks() {
    int row;
    int col;

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            drawCellMark(row, col);
        }
    }
}

void drawScore() {
    if (mode == MODE_PVC) {
        DrawText(TextFormat("You:%d  CPU:%d  Draw:%d", scoreP1, scoreP2, draws), 135, 610, 20, BLACK);
        DrawText(TextFormat("PVC Mode: %s", modeLabel()), 205, 635, 20, DARKGRAY);
    } else {
        DrawText(TextFormat("P1:%d  P2:%d  Draw:%d", scoreP1, scoreP2, draws), 150, 610, 20, BLACK);
    }

    DrawText("Use mouse or keys 1-9", 190, 585, 20, DARKGRAY);
}

void drawResult() {
    if (result == 1) {
        if (winnerMark == 'X') {
            if (mode == MODE_PVC) {
                DrawText("You Win!", 225, 660, 20, RED);
            } else {
                DrawText("Player 1 Wins!", 180, 640, 20, RED);
            }
        } else {
            if (mode == MODE_PVC) {
                DrawText("Computer Wins!", 185, 660, 20, RED);
            } else {
                DrawText("Player 2 Wins!", 180, 640, 20, RED);
            }
        }
    }

    if (result == 2) {
        DrawText("Draw!", 250, 660, 20, BLUE);
    }
}

void drawGameButtons() {
    drawButton(btnNewGame, "New Game");
    drawButton(btnMenu, "Menu");
}

void drawGame() {
    drawGrid();
    drawMarks();
    drawScore();
    drawResult();
    drawGameButtons();
}

void menuInput(Vector2 mousep) {
    if (buttonClicked(btnPVP, mousep)) {
        startPVPGame();
    }

    if (buttonClicked(btnPVC, mousep)) {
        pvcs();
    }
}

void levelInput(Vector2 mousep) {
    if (buttonClicked(btnEasy, mousep)) {
        startPVCGame(EASY);
    }

    if (buttonClicked(btnHard, mousep)) {
        startPVCGame(HARD);
    }

    switch (getDifficultyChoice()) {
        case EASY:
            startPVCGame(EASY);
            break;
        case HARD:
            startPVCGame(HARD);
            break;
        default:
            break;
    }
}

void handleGameButtons(Vector2 mousep, int *choice) {
    if (buttonClicked(btnNewGame, mousep)) {
        resetRound();
        *choice = -1;
        return;
    }

    if (buttonClicked(btnMenu, mousep)) {
        goToMenu();
        *choice = -1;
        return;
    }
}

void boardClick(Vector2 mousep, int *choice) {
    int picked;

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return;
    }

    picked = mouseChoice(mousep);
    if (picked != -1) {
        *choice = picked;
    }
}

void playPVP(int choice) {
    char mark;
    bool placed;

    if (choice == -1) {
        return;
    }

    mark = markNow();
    placed = putMarkInBoard(choice, mark);

    if (!placed) {
        return;
    }

    switchPlayer();
    checkGameState();
}

void playPVCPlayerTurn(int choice) {
    bool placed;

    if (player != 1) {
        return;
    }

    if (choice == -1) {
        return;
    }

    placed = putMarkInBoard(choice, 'X');

    if (!placed) {
        return;
    }

    player = 2;
    checkGameState();
}

void c_turn() {
    int c_choice;
    bool placed;

    if (player != 2) {
        return;
    }

    if (result != 0) {
        return;
    }

    c_choice = c_move();

    if (c_choice == -1) {
        return;
    }

    placed = putMarkInBoard(c_choice, 'O');

    if (!placed) {
        return;
    }

    player = 1;
    checkGameState();
}

void playGame(int choice) {
    if (result != 0) {
        return;
    }

    if (mode == MODE_PVP) {
        playPVP(choice);
    }

    if (mode == MODE_PVC) {
        playPVCPlayerTurn(choice);
        c_turn();
    }
}

void gameInput(Vector2 mousep, int *choice, bool allowMouseClick) {
    if (!allowMouseClick) {
        *choice = -1;
        return;
    }

    boardClick(mousep, choice);
    handleGameButtons(mousep, choice);
    playGame(*choice);
}

int main() {
    int choice;
    Vector2 mousep;
    int frameAtStart;

    InitWindow(WIDTH, HEIGHT, "Tic Tac Toe - Final Project");
    SetTargetFPS(60);

    srand((unsigned int)time(NULL));
    resetRound();

    while (!wclose()) {
        choice = getKeyboardChoice();
        mousep = GetMousePosition();
        frameAtStart = screen;

        if (frameAtStart == MENU) {
            menuInput(mousep);
        }

        if (frameAtStart == difficulty) {
            levelInput(mousep);
        }

        if (frameAtStart == GAME) {
            gameInput(mousep, &choice, frameAtStart == GAME);
        }

        BeginDrawing();
        ClearBackground((Color){240, 240, 240, 255});

        if (screen == MENU) {
            drawMenu();
        }

        if (screen == difficulty) {
            drawDifficulty();
        }

        if (screen == GAME) {
            drawGame();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
