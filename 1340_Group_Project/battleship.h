#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <string>
#include "../DataHandling/users.h"
using namespace std;
// Constants
extern const int BOARD_SIZE;
extern const int EMPTY;
extern const int SHIP;
extern const int HIT;
extern const int MISS;
extern const int NUM_SHIPS;
extern const int SHIP_SIZES[];

extern char difficulty_choice;

// extern int playerBoard[][10]; // Remove global
// extern int enemyBoard[][10]; // Remove global
// extern int enemyDisplay[][10]; // Remove global

// Function Declarations
void initializeBoard(int board[][10]);
void printBoard(int board[][10], bool showShips);
bool isPlacementValid(int board[][10], int row, int col, int size, int dRow, int dCol);
void placeShip(int board[][10], int row, int col, int size, int dRow, int dCol);
// void placeEnemyShips(); // Old signature
void placeRandomShips(int board[][10]); // New signature
void getCoordinates(const string &input, int &row, int &col);
// void placePlayerShips(); // Old signature
void placePlayerShips(int playerBoard[][10]); // New signature
// void playerTurn(); // Old signature
bool humanTurn(int displayBoard[][10], int targetBoard[][10]); // New signature
// void enemyTurn(); // Old signature
void enemyTurn(int targetBoard[][10]); // New signature
bool isGameOver(int board[][10]);
int main_gameplay_loop(char difficulty_choice);
void updateSunkShips(
    int board[10][10],
    int sunkShips[6]);

#endif

/*
C:/msys64/ucrt64/bin/../lib/gcc/x86_64-w64-mingw32/14.2.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:/msys64/ucrt64/bin/../lib/gcc/x86_64-w64-mingw32/14.2.0/../../../../lib/libmingw32.a(lib64_libmingw32_a-crtexewin.o): in function `main':
C:/M/B/src/mingw-w64/mingw-w64-crt/crt/crtexewin.c:67:(.text.startup+0xc5): undefined reference to `WinMain'
collect2.exe: error: ld returned 1 exit status
*/
