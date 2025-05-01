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

void initializeBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = EMPTY;
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
            return false;

        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int nr = r + dr;
                int nc = c + dc;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == SHIP)
                    return false;
            }
        }
    }
    return true;
}

void placeShip(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int dRow, int dCol)
{
    for (int i = 0; i < size; i++)
    {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE)
        {
            board[r][c] = SHIP;
        }
    }
}

void placeRandomShips(int board[BOARD_SIZE][BOARD_SIZE])
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
            if (edgeBias < 70 && (row > 2 && row < 7 && col > 2 && col < 7)) continue;
            if (isPlacementValid(board, row, col, size, dRow, dCol))
            {
                placeShip(board, row, col, size, dRow, dCol);
                placed = true;
            }
        }
    }
}

void getCoordinates(string input, int &row, int &col)
{
    input[0] = toupper(input[0]);
    col = input[0] - 'A';
    row = stoi(input.substr(1)) - 1;
}

void placePlayerShips(int playerBoard[BOARD_SIZE][BOARD_SIZE]) {
    printBoard(playerBoard, true);
    for (int i = 0; i < NUM_SHIPS; i++) {
        int size = SHIP_SIZES[i];
        bool placed = false;
        while (!placed) {
            string input;
            cout << "Enter starting position and direction (H/V) for your " << size << "-square ship (e.g. A4 H): ";
            getline(cin >> ws, input);

            size_t spacePos = input.find(' ');
            if (spacePos == string::npos || spacePos == 0 || spacePos == input.length() - 1) {
                cout << "Invalid format. Use format like A4 H (with a space). Try again.\n";
                continue;
            }

            string coord = input.substr(0, spacePos);
            string direction = input.substr(spacePos + 1);

            coord[0] = toupper(coord[0]);
            direction[0] = toupper(direction[0]);

            int dRow = 0, dCol = 0;
            if (direction[0] == 'H') dCol = 1;
            else if (direction[0] == 'V') dRow = 1;
            else {
                cout << "Invalid direction. Use H or V. Try again.\n";
                continue;
            }

            int row, col;
            try {
                getCoordinates(coord, row, col);
            } catch (...) {
                cout << "Invalid coordinate format. Try again.\n";
                continue;
            }

            if (isPlacementValid(playerBoard, row, col, size, dRow, dCol)) {
                placeShip(playerBoard, row, col, size, dRow, dCol);
                printBoard(playerBoard, true);
                placed = true;
            } else {
                cout << "Invalid placement. Either out of bounds or overlaps another ship. Try again.\n";
            }
        }
    }
}

void humanTurn(int displayBoard[BOARD_SIZE][BOARD_SIZE], int targetBoard[BOARD_SIZE][BOARD_SIZE]) {
    while (true) {
        string input;
        cout << "Enter your shot (e.g. A5): ";
        getline(cin >> ws, input);

        if (input.length() < 2 || !isalpha(input[0]) || !isdigit(input[1])) {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        input[0] = toupper(input[0]);
        int row, col;
        try {
            getCoordinates(input, row, col);
        } catch (...) {
            cout << "Invalid coordinate. Try again.\n";
            continue;
        }

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            cout << "Out of bounds. Try again.\n";
            continue;
        }

        if (displayBoard[row][col] != EMPTY) {
            cout << "Already targeted. Try again.\n";
            continue;
        }

        if (targetBoard[row][col] == SHIP) {
            cout << "Hit!\n";
            displayBoard[row][col] = HIT;
            targetBoard[row][col] = HIT;
        } else {
            cout << "Miss.\n";
            displayBoard[row][col] = MISS;
        }
        break;
    }
}

void randomTurn(int targetBoard[BOARD_SIZE][BOARD_SIZE]) {
    int row, col;
    bool fired = false;
    while (!fired) {
        row = rand() % BOARD_SIZE;
        col = rand() % BOARD_SIZE;

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
            continue;

        if (targetBoard[row][col] == EMPTY || targetBoard[row][col] == SHIP) {
            if (targetBoard[row][col] == SHIP) {
                cout << "Enemy hit your ship at " << char('A' + col) << row + 1 << "!\n";
                targetBoard[row][col] = HIT;
            } else {
                cout << "Enemy missed at " << char('A' + col) << row + 1 << ".\n";
                targetBoard[row][col] = MISS;
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

int main_gameplay_loop()
{
    srand(time(0));
    int playerBoard[BOARD_SIZE][BOARD_SIZE];
    int enemyBoard[BOARD_SIZE][BOARD_SIZE];
    int enemyDisplay[BOARD_SIZE][BOARD_SIZE];

    initializeBoard(playerBoard);
    initializeBoard(enemyBoard);
    initializeBoard(enemyDisplay);

    cout << "\nPlace your ships:\n";
    placePlayerShips(playerBoard);
    placeRandomShips(enemyBoard);

    while (true)
    {
        cout << "\n--- Player Board ---\n";
        printBoard(playerBoard, true);
        cout << "\n--- Enemy Board ---\n";
        printBoard(enemyDisplay, false);

        humanTurn(enemyDisplay, enemyBoard);
        if (isGameOver(enemyBoard))
        {
            cout << "\nYou win!\n";
            break;
        }

        randomTurn(playerBoard);
        if (isGameOver(playerBoard))
        {
            cout << "\nEnemy wins.\n";
            break;
        }
    }
    return 0;
}
