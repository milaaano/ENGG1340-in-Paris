#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include "battleship.h"
using namespace std;

enum Direction { LEFT = 0, UP, RIGHT, DOWN };

struct AttackRecord {
    int row;
    int col;
    bool dir[4];

    AttackRecord(int r, int c) : row(r), col(c) {
        for (int i = 0; i < 4; ++i) dir[i] = true;
    }
};

static vector<AttackRecord> enemyAttacks;

const int BOARD_SIZE = 10;
const int EMPTY = 0;
const int SHIP = 1;
const int HIT = 2;
const int MISS = 3;

const int NUM_SHIPS  = 5;
const int SHIP_SIZES[NUM_SHIPS] = {5, 4, 3, 3, 2};

extern char difficulty_choice;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void initializeBoard(
    int board[BOARD_SIZE][BOARD_SIZE]
) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

void printBoard(
    int board[BOARD_SIZE][BOARD_SIZE],
    bool showShips
) {
    cout << "    ";
    for (char c = 'A'; c < 'A' + BOARD_SIZE; ++c) {
        cout << c << ' ';
    }
    cout << '\n';

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (i + 1 < 10) {
            cout << ' ';
        }
        cout << (i + 1) << "  ";

        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == HIT) {
                cout << "x ";
            }
            else if (board[i][j] == MISS) {
                cout << "o ";
            }
            else if (board[i][j] == SHIP && showShips) {
                cout << "S ";
            }
            else {
                cout << ". ";
            }
        }

        cout << '\n';
    }
}

void getCoordinates(
    const string &input,
    int &row,
    int &col
) {
    char letter = toupper(input[0]);
    col = letter - 'A';

    string numberPart = input.substr(1);
    row = stoi(numberPart) - 1;
}

bool isPlacementValid(
    int board[BOARD_SIZE][BOARD_SIZE],
    int row,
    int col,
    int size,
    int dRow,
    int dCol
) {
    for (int i = 0; i < size; ++i) {
        int r = row + i * dRow;
        int c = col + i * dCol;

        if (r < 0
            || r >= BOARD_SIZE
            || c < 0
            || c >= BOARD_SIZE
            || board[r][c] != EMPTY) {
            return false;
        }

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int nr = r + dr;
                int nc = c + dc;
                if (nr >= 0
                    && nr < BOARD_SIZE
                    && nc >= 0
                    && nc < BOARD_SIZE
                    && board[nr][nc] == SHIP) {
                    return false;
                }
            }
        }
    }

    return true;
}

void placeShip(
    int board[BOARD_SIZE][BOARD_SIZE],
    int row,
    int col,
    int size,
    int dRow,
    int dCol
) {
    for (int i = 0; i < size; ++i) {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r >= 0
            && r < BOARD_SIZE
            && c >= 0
            && c < BOARD_SIZE) {
            board[r][c] = SHIP;
        }
    }
}

void placeRandomShips(
    int board[BOARD_SIZE][BOARD_SIZE]
) {
    for (int index = 0; index < NUM_SHIPS; ++index) {
        int size = SHIP_SIZES[index];
        bool placed = false;

        while (!placed) {
            int r = rand() % BOARD_SIZE;
            int c = rand() % BOARD_SIZE;
            bool horizontal = (rand() % 2 == 1);
            int dR = horizontal ? 0 : 1;
            int dC = horizontal ? 1 : 0;

            if (isPlacementValid(board, r, c, size, dR, dC)) {
                placeShip(board, r, c, size, dR, dC);
                placed = true;
            }
        }
    }
}

void placePlayerShips(
    int board[BOARD_SIZE][BOARD_SIZE]
) {
    for (int index = 0; index < NUM_SHIPS; ++index) {
        int size = SHIP_SIZES[index];
        bool placed = false;

        while (!placed) {
            clearScreen();
            printBoard(board, true);
            cout << "Enter start coord and direction (H/V) for your "
                 << size << "-cell ship: ";

            string line;
            getline(cin >> ws, line);

            size_t spacePos = line.find(' ');
            if (spacePos == string::npos) {
                cout << "Format must be 'A4 H'. Try again.\n";
                continue;
            }

            string coord = line.substr(0, spacePos);
            char dirChar = toupper(line[spacePos + 1]);

            int dR = 0;
            int dC = 0;
            if (dirChar == 'H') {
                dC = 1;
            }
            else if (dirChar == 'V') {
                dR = 1;
            }
            else {
                cout << "Direction must be H or V.\n";
                continue;
            }

            int r, c;
            try {
                getCoordinates(coord, r, c);
            }
            catch (...) {
                cout << "Invalid coordinate. Try again.\n";
                continue;
            }

            if (isPlacementValid(board, r, c, size, dR, dC)) {
                placeShip(board, r, c, size, dR, dC);
                placed = true;
            }
            else {
                cout << "Cannot place ship there.\n";
            }
        }
    }
}

bool humanTurn(
    int displayBoard[BOARD_SIZE][BOARD_SIZE],
    int targetBoard[BOARD_SIZE][BOARD_SIZE]
) {
    while (true) {
        cout << "Your shot (e.g. A5): ";
        string input;
        getline(cin >> ws, input);

        if (input.size() < 2
            || !isalpha(input[0])
            || !isdigit(input[1])) {
            cout << "Invalid input.\n";
            continue;
        }

        int r;
        int c;
        try {
            getCoordinates(input, r, c);
        }
        catch (...) {
            cout << "Bad coordinate.\n";
            continue;
        }

        if (r < 0
            || r >= BOARD_SIZE
            || c < 0
            || c >= BOARD_SIZE) {
            cout << "Out of bounds.\n";
            continue;
        }

        if (displayBoard[r][c] != EMPTY) {
            cout << "Already targeted.\n";
            continue;
        }

        if (targetBoard[r][c] == SHIP) {
            cout << "Hit!\n";
            displayBoard[r][c] = HIT;
            targetBoard[r][c] = HIT;
            return true;
        }
        else {
            cout << "Miss.\n";
            displayBoard[r][c] = MISS;
            return false;
        }
    }
}

bool enemyTurn(
    int board[BOARD_SIZE][BOARD_SIZE],
    char diff,
    int sunkShips[6]
) {
    if (diff == '1') {
        bool fired = false;
        while (!fired) {
            int r = rand() % BOARD_SIZE;
            int c = rand() % BOARD_SIZE;

            if (board[r][c] == EMPTY
                || board[r][c] == SHIP) {
                if (board[r][c] == SHIP) {
                    cout << "Enemy hit at "
                         << char('A' + c)
                         << (r + 1)
                         << "!\n";

                    board[r][c] = HIT;
                    fired = true;
                    return true;
                }
                else {
                    cout << "Enemy missed at "
                         << char('A' + c)
                         << (r + 1)
                         << ".\n";

                    board[r][c] = MISS;
                    fired = true;
                    return false;
                }
            }
        }
    }
    else if (diff == '2'){
        //enemyTurn_HalfPro(playerBoard);
    }
    else if (diff == '3') {
        if (!enemyAttacks.empty()) {
            AttackRecord &rec = enemyAttacks.back();
            for (int d = 0; d < 4; ++d) {
                if (!rec.dir[d]) continue;
                int nr = rec.row;
                int nc = rec.col;
                switch (d) {
                    case LEFT:  nc -= 1; break;
                    case UP:    nr -= 1; break;
                    case RIGHT: nc += 1; break;
                    case DOWN:  nr += 1; break;
                }
                rec.dir[d] = false;
                if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) continue;
                if (board[nr][nc] != EMPTY && board[nr][nc] != SHIP) continue;
                if (board[nr][nc] == SHIP) {
                    cout << "Enemy hit at " << char('A' + nc) << (nr + 1) << "!\n";
                    board[nr][nc] = HIT;
                    if (d == LEFT || d == RIGHT) {
                        rec.dir[UP] = false;
                        rec.dir[DOWN] = false;
                    } else {
                        rec.dir[LEFT] = false;
                        rec.dir[RIGHT] = false;
                    }
                    enemyAttacks.emplace_back(nr, nc);
                    return true;
                } else {
                    cout << "Enemy missed at " << char('A' + nc) << (nr + 1) << ".\n";
                    board[nr][nc] = MISS;
                    return false;
                }
            }
            enemyAttacks.clear();
        }
        int spacing = 2;
        for (int s = 2; s <= 5; ++s) {
            int total = 0;
            for (int i = 0; i < NUM_SHIPS; ++i) {
                if (SHIP_SIZES[i] == s) total++;
            }
            if (sunkShips[s] < total) {
                spacing = s;
                break;
            }
        }
        static int lastIdx = 0;
        int totalCells = BOARD_SIZE * BOARD_SIZE;
        for (int attempt = 0; attempt < totalCells; ++attempt) {
            int idx = (lastIdx + attempt * spacing) % totalCells;
            int r = idx / BOARD_SIZE;
            int c = idx % BOARD_SIZE;
            if (board[r][c] == EMPTY || board[r][c] == SHIP) {
                if (board[r][c] == SHIP) {
                    cout << "Enemy hit at " << char('A' + c) << (r + 1) << "!\n";
                    board[r][c] = HIT;
                    enemyAttacks.emplace_back(r, c);
                    lastIdx = idx;
                    return true;
                } else {
                    cout << "Enemy missed at " << char('A' + c) << (r + 1) << ".\n";
                    board[r][c] = MISS;
                    lastIdx = idx;
                    return false;
                }
            }
        }
    }
    else if (diff == '4'){
        //enemyTurn_Pro(playerBoard);
    }
    return false;
}

// Yerassyl

// Yerassyl

void updateSunkShips(
    int board[BOARD_SIZE][BOARD_SIZE],
    int sunkShips[6]
) {
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] == HIT && !visited[r][c]) {
                int length = 0;

                for (int cc = c;
                     cc < BOARD_SIZE
                     && board[r][cc] == HIT;
                     ++cc) {
                    visited[r][cc] = true;
                    ++length;
                }
                if (length > 1) {
                    bool sunkOk = true;
                    for (int i = 0;
                         i < length;
                         ++i) {
                        if (board[r][c + i] == SHIP) {
                            sunkOk = false;
                            break;
                        }
                    }
                    if (sunkOk) {
                        ++sunkShips[length];
                    }
                    continue;
                }


                length = 0;
                for (int rr = r;
                     rr < BOARD_SIZE
                     && board[rr][c] == HIT;
                     ++rr) {
                    visited[rr][c] = true;
                    ++length;
                }
                if (length > 1) {
                    bool sunkOk = true;
                    for (int i = 0;
                         i < length;
                         ++i) {
                        if (board[r + i][c] == SHIP) {
                            sunkOk = false;
                            break;
                        }
                    }
                    if (sunkOk) {
                        ++sunkShips[length];
                    }
                }
            }
        }
    }
}

bool isGameOver(
    int board[BOARD_SIZE][BOARD_SIZE]
) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == SHIP) {
                return false;
            }
        }
    }
    return true;
}

int main_gameplay_loop() {
    srand(static_cast<unsigned>(time(nullptr)));

    int playerBoard[BOARD_SIZE][BOARD_SIZE];
    int enemyBoard [BOARD_SIZE][BOARD_SIZE];
    int displayBoard[BOARD_SIZE][BOARD_SIZE];
    int sunkShipsArr[6] = {0};

    initializeBoard(playerBoard);
    initializeBoard(enemyBoard);
    initializeBoard(displayBoard);

    cout << "Place your ships...\n";
    placePlayerShips(playerBoard);
    placeRandomShips(enemyBoard);

    bool playerTurn = true;
    while (true) {
        clearScreen();
        cout << "--- Player Board ---\n";
        printBoard(playerBoard, true);
        cout << "--- Enemy Board ---\n";
        printBoard(displayBoard, false);

        bool hit = false;
        if (playerTurn) {
            hit = humanTurn(displayBoard, enemyBoard);
            if (hit) {
                updateSunkShips(enemyBoard, sunkShipsArr);
                if (isGameOver(enemyBoard)) {
                    cout << "You win!\n";
                    break;
                }
            }
        }
        else {
            hit = enemyTurn(playerBoard, difficulty_choice, sunkShipsArr);
            if (hit) {
                updateSunkShips(playerBoard, sunkShipsArr);
                if (isGameOver(playerBoard)) {
                    cout << "Enemy wins!\n";
                    break;
                }
            }
        }

        if (!hit) {
            playerTurn = !playerTurn;
        }
    }

    return 0;
}

int main() {
    return main_gameplay_loop();
}
