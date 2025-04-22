#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include <cmath>
#include "battleship.h"

using namespace std;

const int BOARD_SIZE = 10;
const int EMPTY = 0;
const int SHIP = 1;
const int HIT = 2;
const int MISS = 3;

const int NUM_SHIPS = 5;
const int SHIP_SIZES[NUM_SHIPS] = {5, 4, 3, 3, 2};

int playerBoard[BOARD_SIZE][BOARD_SIZE];
int enemyBoard[BOARD_SIZE][BOARD_SIZE];
int enemyDisplay[BOARD_SIZE][BOARD_SIZE];

void initializeBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = EMPTY;
        }
    }
}

void printBoard(int board[BOARD_SIZE][BOARD_SIZE], bool showShips)
{
    cout << "    ";
    for (char c = 'A'; c < 'A' + BOARD_SIZE; c++)
        cout << c << " ";
    cout << endl;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cout << (i + 1 < 10 ? " " : "") << i + 1 << "  ";
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == HIT)
                cout << "x ";
            else if (board[i][j] == MISS)
                cout << "o ";
            else if (board[i][j] == SHIP && showShips)
                cout << "S ";
            else
                cout << ". ";
        }
        cout << endl;
    }
}

bool isPlacementValid(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int dRow, int dCol)
{
    for (int i = 0; i < size; i++)
    {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != EMPTY)
        {
            return false;
        }
        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int nr = r + dr;
                int nc = c + dc;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == SHIP)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void placeShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int dRow, int dCol)
{
    for (int i = 0; i < size; i++)
    {
        board[row + i * dRow][col + i * dCol] = SHIP;
    }
}

void placeEnemyShips()
{
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        int size = SHIP_SIZES[i];
        bool placed = false;
        while (!placed)
        {
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            bool horizontal = rand() % 2;

            int dRow = horizontal ? 0 : 1;
            int dCol = horizontal ? 1 : 0;

            int edgeBias = rand() % 100;
            if (edgeBias < 70 && (row > 2 && row < 7 && col > 2 && col < 7))
                continue;

            if (isPlacementValid(enemyBoard, row, col, size, dRow, dCol))
            {
                placeShip(enemyBoard, row, col, size, dRow, dCol);
                placed = true;
            }
        }
    }
}

void getCoordinates(string input, int &row, int &col)
{
    col = toupper(input[0]) - 'A';
    row = stoi(input.substr(1)) - 1;
}

void placePlayerShips()
{
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        int size = SHIP_SIZES[i];
        bool placed = false;
        while (!placed)
        {
            string startInput, endInput;
            cout << "Enter starting and ending positions for your " << size << "-square ship (e.g. A4 A" << (4 + size - 1) << " or A4 " << char('A' + size - 1) << "4): ";
            cin >> startInput >> endInput;

            int startRow, startCol, endRow, endCol;
            getCoordinates(startInput, startRow, startCol);
            getCoordinates(endInput, endRow, endCol);

            int dRow = endRow - startRow;
            int dCol = endCol - startCol;
            int length = max(abs(dRow), abs(dCol)) + 1;

            if (length != size || (dRow != 0 && dCol != 0))
            {
                cout << "Invalid length or diagonal direction not allowed. Try again.\n";
                continue;
            }

            dRow = (dRow == 0) ? 0 : (dRow > 0 ? 1 : -1);
            dCol = (dCol == 0) ? 0 : (dCol > 0 ? 1 : -1);

            if (isPlacementValid(playerBoard, startRow, startCol, size, dRow, dCol))
            {
                placeShip(playerBoard, startRow, startCol, size, dRow, dCol);
                printBoard(playerBoard, true);
                placed = true;
            }
            else
            {
                cout << "Invalid placement. Try again.\n";
            }
        }
    }
}

void playerTurn()
{
    string input;
    int row, col;
    cout << "Enter your shot (e.g. A5): ";
    cin >> input;

    if (input.length() < 2 || !isalpha(input[0]) || !isdigit(input[1]))
    {
        cout << "Invalid input. Try again.\n";
        playerTurn();
        return;
    }

    getCoordinates(input, row, col);

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        cout << "Out of bounds. Try again.\n";
        playerTurn();
        return;
    }

    if (enemyDisplay[row][col] != EMPTY)
    {
        cout << "Already targeted. Try again.\n";
        playerTurn();
        return;
    }

    if (enemyBoard[row][col] == SHIP)
    {
        cout << "Hit!\n";
        enemyDisplay[row][col] = HIT;
        enemyBoard[row][col] = HIT;
    }
    else
    {
        cout << "Miss.\n";
        enemyDisplay[row][col] = MISS;
    }
}

void enemyTurn()
{
    int row, col;
    bool fired = false;
    while (!fired)
    {
        row = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;
        if (playerBoard[row][col] == EMPTY || playerBoard[row][col] == SHIP)
        {
            if (playerBoard[row][col] == SHIP)
            {
                cout << "Enemy hit your ship at " << char('A' + col) << row + 1 << "!\n";
                playerBoard[row][col] = HIT;
            }
            else
            {
                cout << "Enemy missed at " << char('A' + col) << row + 1 << ".\n";
                playerBoard[row][col] = MISS;
            }
            fired = true;
        }
    }
}

bool isGameOver(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == SHIP)
                return false;
        }
    }
    return true;
}

int main()
{
    srand(time(0));
    initializeBoard(playerBoard);
    initializeBoard(enemyBoard);
    initializeBoard(enemyDisplay);

    cout << "\nPlace your ships:\n";
    placePlayerShips();
    placeEnemyShips();

    while (true)
    {
        cout << "\n--- Player Board ---\n";
        printBoard(playerBoard, true);
        cout << "\n--- Enemy Board ---\n";
        printBoard(enemyDisplay, false);

        playerTurn();
        if (isGameOver(enemyBoard))
        {
            cout << "\nYou win!\n";
            break;
        }

        enemyTurn();
        if (isGameOver(playerBoard))
        {
            cout << "\nEnemy wins.\n";
            break;
        }
    }
    return 0;
}
