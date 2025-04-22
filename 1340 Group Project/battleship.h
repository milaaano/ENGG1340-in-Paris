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
extern const int SHIP_SIZES[]; // Note: Size isn't specified here

extern int playerBoard[][10];
extern int enemyBoard[][10];
extern int enemyDisplay[][10];

// Function Declarations
void initializeBoard(int board[][10]);
void printBoard(int board[][10], bool showShips);
bool isPlacementValid(int board[][10], int row, int col, int size, int dRow, int dCol);
void placeShip(int board[][10], int row, int col, int size, int dRow, int dCol);
void placeEnemyShips();
void getCoordinates(std::string input, int &row, int &col);
void placePlayerShips();
void playerTurn();
void enemyTurn();
bool isGameOver(int board[][10]);

#endif // BATTLESHIP_H