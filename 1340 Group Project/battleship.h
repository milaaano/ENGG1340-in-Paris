#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <string>

// Constants
extern const int BOARD_SIZE;
extern const int EMPTY;
extern const int SHIP;
extern const int HIT;
extern const int MISS;
extern const int NUM_SHIPS;
extern const int SHIP_SIZES[];

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
void getCoordinates(std::string input, int &row, int &col);
// void placePlayerShips(); // Old signature
void placePlayerShips(int playerBoard[][10]); // New signature
// void playerTurn(); // Old signature
void humanTurn(int displayBoard[][10], int targetBoard[][10]); // New signature
// void enemyTurn(); // Old signature
void randomTurn(int targetBoard[][10]); // New signature
bool isGameOver(int board[][10]);

#endif