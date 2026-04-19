#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#define windowwidth 600
#define windowheight 720
#define cellsize 200
#define menu 0
#define difficulty 1
#define game 2
#define pvpmode 1
#define pvcmode 2
#define easy 1
#define hard 2
char board[3][3];
int scorep1 = 0;
int scorep2 = 0;
int draws = 0;
int player = 1;
int screen = menu;
int mode = 0;
int level = easy;
int result = 0;
char winnermark = 0;
Rectangle btnpvp = {150, 200, 300, 50};
Rectangle btnpvc = {150, 300, 300, 50};
Rectangle btneasy = {150, 200, 300, 50};
Rectangle btnhard = {150, 300, 300, 50};
Rectangle btnnewgame = {110, 660, 170, 40};
Rectangle btnmenu = {320, 660, 170, 40};
void clearboard() {
    int row;
    int col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            board[row][col] = 0;
        }
    }
}

void resetround() {
    clearboard();
    player = 1;
    result = 0;
    winnermark = 0;
}

void gotomenu() {
    screen = menu;
    mode = 0;
    resetround();
}

void startpvpgame() {
    mode = pvpmode;
    screen = game;
    resetround();
}

void pvcs() {
    mode = pvcmode;
    screen = difficulty;
    resetround();
}

void startpvcgame(int selectedlevel) {
    level = selectedlevel;
    screen = game;
    resetround();
}

int rowfromchoice(int choice) {
    if (choice >= 1 && choice <= 3) return 0;
    if (choice >= 4 && choice <= 6) return 1;
    if (choice >= 7 && choice <= 9) return 2;
    return -1;
}

int colfromchoice(int choice) {
    if (choice == 1 || choice == 4 || choice == 7) return 0;
    if (choice == 2 || choice == 5 || choice == 8) return 1;
    if (choice == 3 || choice == 6 || choice == 9) return 2;
    return -1;
}

bool validchoice(int choice) {
    if (choice < 1) return false;
    if (choice > 9) return false;
    return true;
}

bool cellempty(int choice) {
    int row;
    int col;
    if (!validchoice(choice)) {
        return false;
    }
    row = rowfromchoice(choice);
    col = colfromchoice(choice);
    if (row == -1 || col == -1) {
        return false;
    }
    if (board[row][col] == 0) {
        return true;
    }
    return false;
}

bool putmark(int choice, char mark) {
    int row;
    int col;
    if (!cellempty(choice)) {
        return false;
    }
    row = rowfromchoice(choice);
    col = colfromchoice(choice);
    board[row][col] = mark;
    return true;
}

char marknow() {
    if (player == 1) {
        return 'X';
    }
    return 'O';
}

void switchplayer() {
    if (player == 1) {
        player = 2;
    } else {
        player = 1;
    }
}

char checkrows() {
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

char checkcols() {
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

char checkdiagonals() {
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

char checkwinner() {
    char result;
    result = checkrows();
    if (result != 0) return result;
    result = checkcols();
    if (result != 0) return result;
    result = checkdiagonals();
    if (result != 0) return result;
    return 0;
}

bool isboardfull() {
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

void g_state() {     //g-> state
    winnermark = checkwinner();
    if (winnermark == 'X') {
        result = 1;
        scorep1++;
        return;
    }
    if (winnermark == 'O') {
        result = 1;
        scorep2++;
        return;
    }
    if (isboardfull()) {
        result = 2;
        draws++;
        return;
    }
    result = 0;
}

bool movewins(int choice, char mark) {
    int row;
    int col;
    char tempwinner;
    if (!cellempty(choice)) {
        return false;
    }
    row = rowfromchoice(choice);
    col = colfromchoice(choice);
    board[row][col] = mark;
    tempwinner = checkwinner();
    board[row][col] = 0;
    if (tempwinner == mark) {
        return true;
    }
    return false;
}

int bestmove(char mark) {
    int spot;
    for (spot = 1; spot <= 9; spot++) {
        if (movewins(spot, mark)) {
            return spot;
        }
    }
    return -1;
}

int easymove() {
    int spot;
    while (1) {
        spot = (rand() % 9) + 1;
        if (cellempty(spot)) {
            return spot;
        }
    }
}

int hardmove() {
    int spot;
    spot = bestmove('O');
    if (spot != -1) {
        return spot;
    }
    spot = bestmove('X');
    if (spot != -1) {
        return spot;
    }
    if (cellempty(5)) {
        return 5;
    }
    if (cellempty(1)) return 1;
    if (cellempty(3)) return 3;
    if (cellempty(7)) return 7;
    if (cellempty(9)) return 9;
    for (spot = 1; spot <= 9; spot++) {
        if (cellempty(spot)) {
            return spot;
        }
    }
    return -1;
}

int c_move() {     // c-> computer move 
    if (level == easy) {
        return easymove();
    }
    return hardmove();
}

int diff_choice() {
    if (keypres(KEY_E)) return easy;
    if (keypres(KEY_H)) return hard;
    return -1;
}

const char *modelabel() {
    if (level == hard) {
        return "Hard";
    }
    return "Easy";
}

int key_choice() {
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

int mouseChoice(Vector2 mousep) {   // mousep-> mouse position
    if (mousep.x < 0 || mousep.x >= windowwidth) {
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

void drawbutton(Rectangle button, const char *text) {
    Color fillcolor;
    int textwidth;
    int textx;
    int texty;
    if (CheckCollisionPointRec(GetMousePosition(), button)) {
        fillcolor = GRAY;
    } else {
        fillcolor = LIGHTGRAY;
    }
    DrawRectangleRec(button, fillcolor);
    DrawRectangleLinesEx(button, 2, DARKGRAY);
    textwidth = MeasureText(text, 20);
    textx = (int)(button.x + (button.width / 2) - (textwidth / 2));
    texty = (int)(button.y + (button.height / 2) - 10);
    DrawText(text, textx, texty, 20, BLACK);
}

void drawmenu() {
    DrawText("Select Mode", 200, 100, 30, BLACK);
    drawbutton(btnpvp, "Player vs Player");
    drawbutton(btnpvc, "Player vs Computer");
    DrawText("PVC lets you choose Easy or Hard next", 105, 380, 20, DARKGRAY);
}

void drawdifficulty() {
    DrawText("Player vs Computer", 150, 90, 25, BLACK);
    DrawText("Choose Easy or Hard", 160, 125, 24, DARKGRAY);
    drawbutton(btneasy, "Easy");
    drawbutton(btnhard, "Hard");
    DrawText("Press E for Easy or H for Hard", 135, 385, 18, DARKGRAY);
}

void drawgrid() {
    DrawLine(200, 0, 200, 600, BLACK);
    DrawLine(400, 0, 400, 600, BLACK);
    DrawLine(0, 200, 600, 200, BLACK);
    DrawLine(0, 400, 600, 400, BLACK);
}

void drawcellmark(int row, int col) {
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

void drawmarks() {  
    int row;
    int col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            drawcellmark(row, col);
        }
    }
}

void drawscore() {  
    if (mode == pvcmode) {
        DrawText(TextFormat("You:%d  CPU:%d  Draw:%d", scorep1, scorep2, draws), 135, 610, 20, BLACK);
        DrawText(TextFormat("PVC Mode: %s", modelabel()), 205, 635, 20, DARKGRAY);
    } else {
        DrawText(TextFormat("P1:%d  P2:%d  Draw:%d", scorep1, scorep2, draws), 150, 610, 20, BLACK);
    }
    DrawText("Use mouse or keys 1-9", 190, 585, 20, DARKGRAY);
}

void drawresult() {
    if (result == 1) {
        if (winnermark == 'X') {
            if (mode == pvcmode) {
                DrawText("You Win!", 225, 660, 20, RED);
            } else {
                DrawText("Player 1 Wins!", 180, 640, 20, RED);
            }
        } else {
            if (mode == pvcmode) {
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

void drawgamebuttons() {
    drawbutton(btnnewgame, "New Game");
    drawbutton(btnmenu, "Menu");
}

void drawgame() {
    drawgrid();
    drawmarks();
    drawscore();
    drawresult();
    drawgamebuttons();
}

void menuInput(Vector2 mousep) {
    if (buttonClicked(btnpvp, mousep)) {
        startpvpgame();
    }
    if (buttonClicked(btnpvc, mousep)) {
        pvcs();
    }
}

void levelInput(Vector2 mousep) {
    if (buttonClicked(btneasy, mousep)) {
        startpvcgame(easy);
    }
    if (buttonClicked(btnhard, mousep)) {
        startpvcgame(hard);
    }
    switch (diff_choice()) {
        case easy:
            startpvcgame(easy);
            break;
        case hard:
            startpvcgame(hard);
            break;
        default:
            break;
    }
}

void handlegamebuttons(Vector2 mousep, int *choice) {
    if (buttonClicked(btnnewgame, mousep)) {
        resetround();
        *choice = -1;
        return;
    }
    if (buttonClicked(btnmenu, mousep)) {
        gotomenu();
        *choice = -1;
        return;
    }
}

void boardclick(Vector2 mousep, int *choice) {
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
    mark = marknow();
    placed = putmark(choice, mark);
    if (!placed) {
        return;
    }
    switchplayer();
    g_state();
}

void playPVCPlayerTurn(int choice) {
    bool placed;
    if (player != 1) {
        return;
    }
    if (choice == -1) {
        return;
    }
    placed = putmark(choice, 'X');
    if (!placed) {
        return;
    }
    player = 2;
    g_state();
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
    placed = putmark(c_choice, 'O');
    if (!placed) {
        return;
    }
    player = 1;
    g_state();
}

void playGame(int choice) {
    if (result != 0) {
        return;
    }
    if (mode == pvpmode) {
        playPVP(choice);
    }
    if (mode == pvcmode) {
        playPVCPlayerTurn(choice);
        c_turn();
    }
}

void gameInput(Vector2 mousep, int *choice, bool allowMouseClick) {
    if (!allowMouseClick) {
        *choice = -1;
        return;
    }
    boardclick(mousep, choice);
    handlegamebuttons(mousep, choice);
    playGame(*choice);
}

int main() {
    int choice;
    Vector2 mousep;
    int frameatstart;
    InitWindow(windowwidth, windowheight, "Tic Tac Toe - Final Project");
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));
    resetround();
    while (!wclose()) {
        choice = key_choice();
        mousep = GetMousePosition();
        frameatstart = screen;
        if (frameatstart == menu) {
            menuInput(mousep);
        }
        if (frameatstart == difficulty) {
            levelInput(mousep);
        }
        if (frameatstart == game) {
            gameInput(mousep, &choice, frameatstart == game);
        }
        BeginDrawing();
        ClearBackground((Color){240, 240, 240, 255});
        if (screen == menu) {
            drawmenu();
        }
        if (screen == difficulty) {
            drawdifficulty();
        }
        if (screen == game) {
            drawgame();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
