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

// ANSI color codes
constexpr char const* CYAN   = "\033[36m";
constexpr char const* YELLOW = "\033[33m";
constexpr char const* RESET  = "\033[0m";
constexpr char const* RED= "\033[31m";
constexpr char const* GREEN = "\033[32m";
constexpr char const* BLUE= "\033[34m";
constexpr char const* MAGENTA = "\033[35m";


// YERAYERA

int playerBoard[BOARD_SIZE][BOARD_SIZE];


int enemyBoard[BOARD_SIZE][BOARD_SIZE];
int enemyDisplay[BOARD_SIZE][BOARD_SIZE];

//Probability boards for all the enemy ships
int enemyProbabilityBoard_5[BOARD_SIZE][BOARD_SIZE];
int enemyProbabilityBoard_4[BOARD_SIZE][BOARD_SIZE];
int enemyProbabilityBoard_3[BOARD_SIZE][BOARD_SIZE];
int enemyProbabilityBoard_2[BOARD_SIZE][BOARD_SIZE];

//the coordinates with maximum probability
int probable_x = -1;
int probable_y = -1;

//The map of all "alive" ships on the field of the user
int alive_ships[]={5, 4, 3, 3, 2};

//The map of all succesful strikes on the field of the computer
int enemyStrikeBoard[BOARD_SIZE][BOARD_SIZE];

//to understand that the algorith is hitting the ship
bool in_the_process=false;
int fired_x=-1;
int fired_y=-1;
int direction=0;
int end_of_a_ship=0;
int SHIP_SIZE_Memory=0;

extern char difficulty_choice;

// void clearScreen() {
// #ifdef _WIN32
//     system("cls");
// #else
//     system("clear");
// #endif
// }

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
        cout << YELLOW << c << RESET <<' ';
    }
    cout << '\n';

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (i + 1 < 10) {
            cout << ' ';
        }
        cout <<YELLOW<< (i + 1) << RESET << "  ";

        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == HIT) {
                cout << RED<<"x "<<RESET;
            }
            else if (board[i][j] == MISS) {
                cout << BLUE << "o "<<RESET;
            }
            else if (board[i][j] == SHIP && showShips) {
                cout << GREEN << "S "<<RESET;
            }
            else {
                cout << YELLOW<< ". "<<RESET;
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
            cout << "Enter start coord and direction (H/V) for your "<< size << "-cell ship: ";

            string line;
            getline(cin >> ws, line);

            size_t spacePos = line.find(' ');
            if (spacePos == string::npos) {
                cout << "Format must be 'A4 H'. Try again.\n";
                //to make the output
                cout<<"Press Enter to continue.\n";
                cin.get();
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
                //to make the output
                cout<<"Press Enter to continue.\n";
                cin.get();
                continue;
            }

            int r, c;
            try {
                getCoordinates(coord, r, c);
            }
            catch (...) {
                cout << "Invalid coordinate. Try again.\n";
                //to make the output
                cout<<"Press Enter to continue.\n";
                cin.get();
                continue;
            }

            if (isPlacementValid(board, r, c, size, dR, dC)) {
                placeShip(board, r, c, size, dR, dC);
                placed = true;
            }
            else {
                cout << "Cannot place ship there.\n";

                //to make the output
                cout<<"Press Enter to continue.\n";
                cin.get();
                continue;
            }
        }
        clearScreen();
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

//Yera Start
void initializeProbabilityBoard(int board[BOARD_SIZE][BOARD_SIZE], int probability_board[BOARD_SIZE][BOARD_SIZE], int SHIP_SIZE) {
    //Initialize and annul board proability board
    initializeBoard(probability_board);

    // firstly for the horizonal placement
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY && j+SHIP_SIZE<BOARD_SIZE){
                bool success=true;
                int j_initial=j;

                //duplicates for the variables to intializa the probability board
                int j_1=j;
                int j_2=j;
                while (j_1<j_initial+SHIP_SIZE){
                    for (int dr = -1; dr <= 1; dr++) {
                        for (int dc = -1; dc <= 1; dc++) {
                            int nr = i + dr;
                            int nc = j_1 + dc;
                            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == SHIP) {
                                success=false;
                            }
                        }
                    }
                    j_1++;
                }
                if (success){
                    while (j_2<j_initial+SHIP_SIZE){
                        probability_board[i][j_2]++;
                        j_2++;
                    }
                    j_2=j;
                }
                
            
            }
        }
    }

// firstly for the certical placement
for (int j = 0; j < BOARD_SIZE; j++) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][j] == EMPTY && i+SHIP_SIZE<BOARD_SIZE){
            bool success=true;
            int i_initial=i;

            //duplicates for the variables to intializa the probability board
            int i_1=i;
            int i_2=i;
            while (i_1<i_initial+SHIP_SIZE){
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        int nr = i_1 + dr;
                        int nc = j + dc;
                        if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == SHIP) {
                            success=false;
                        }
                    }
                }
                i_1++;
            }
            if (success){
                while (i_2<i_initial+SHIP_SIZE){
                    probability_board[i_2][j]++;
                    i_2++;
                }
            }
            i_2=i;
        }
    }

}
for (int i=0; i<BOARD_SIZE;i++){
    for (int j=0; j<BOARD_SIZE; j++){
        if (board[i][j]==HIT) {
            probability_board[i][j]=0;
        }
    }
}
}


//for finding the coordinate with teh maximum probability
void probable_position(int board[BOARD_SIZE][BOARD_SIZE], int & probable_x, int & probable_y){
    int max=0;
    for (int i=0; i<BOARD_SIZE; i++){
        for (int j=0; j<BOARD_SIZE; j++){
            if (board[i][j]>max){
                max=board[i][j];
                probable_x=i;
                probable_y=j;
            }
        }
    }
}



// the full logic of the computer to shoor the ships
void enemyTurn_Pro(int playerBoard[BOARD_SIZE][BOARD_SIZE]) {    
    int SHIP_SIZE;
    for (int i = 0; i < 5; i++) {
        if (alive_ships[i] != 0) {
            SHIP_SIZE = alive_ships[i];
            break;
        }
    }

    bool fired = false;
    while (!fired) {
        if (!in_the_process) {
            // Initialize probability board based on ship size
            switch (SHIP_SIZE) {
                case 5:
                    initializeProbabilityBoard(enemyStrikeBoard, enemyProbabilityBoard_5, SHIP_SIZE);
                    probable_position(enemyProbabilityBoard_5, probable_x, probable_y);
                    break;
                case 4:
                    initializeProbabilityBoard(enemyStrikeBoard, enemyProbabilityBoard_4, SHIP_SIZE);
                    probable_position(enemyProbabilityBoard_4, probable_x, probable_y);
                    break;
                case 3:
                    initializeProbabilityBoard(enemyStrikeBoard, enemyProbabilityBoard_3, SHIP_SIZE);
                    probable_position(enemyProbabilityBoard_3, probable_x, probable_y);
                    break;
                case 2:
                    initializeProbabilityBoard(enemyStrikeBoard, enemyProbabilityBoard_2, SHIP_SIZE);
                    probable_position(enemyProbabilityBoard_2, probable_x, probable_y);
                    break;
            }

            if (playerBoard[probable_x][probable_y] == EMPTY || playerBoard[probable_x][probable_y] == SHIP) {

                if (playerBoard[probable_x][probable_y] == SHIP) {
                    cout << "Enemy hit your ship at " << char('A' + probable_y) << probable_x + 1 << "!\n";
                    playerBoard[probable_x][probable_y] = HIT;
                    enemyStrikeBoard[probable_x][probable_y] = SHIP;
                    in_the_process = true;
                    SHIP_SIZE_Memory++;
                    fired_x = probable_x;
                    fired_y=probable_y;
                } else {
                    cout << "Enemy missed at " << char('A' + probable_y) << probable_x + 1 << ".\n";
                    playerBoard[probable_x][probable_y] = MISS;
                    enemyStrikeBoard[probable_x][probable_y] = HIT;
                    fired = true; // Exit the loop after a miss
                }
            }
        } 
        else {
            int direction_i;
            if (direction==0){
                direction_i=fired_x+1;
                if (direction_i>BOARD_SIZE){
                    end_of_a_ship++;
                    direction++;
                }
                else{
                    if (playerBoard[direction_i][fired_y] == SHIP) {
                        fired_x=direction_i;
                        playerBoard[fired_x][fired_y] = HIT;
                        enemyStrikeBoard[fired_x][fired_y] = SHIP;
                        SHIP_SIZE_Memory++;
                        cout << "Enemy hit your ship at " << char('A' + fired_y) << fired_x + 1 << "!\n";
                    }
                    else{
                        cout << "Enemy missed at " << char('A' + fired_y) << direction_i + 1 << ".\n";
                        playerBoard[direction_i][fired_y] = MISS;
                        enemyStrikeBoard[direction_i][fired_y] = HIT;
                        end_of_a_ship++;
                        fired_x=probable_x;
                        direction++;
                        fired = true;
                    }
                }
                if(end_of_a_ship==2 && SHIP_SIZE_Memory>1){
                    for (int i=0; i<5; i++){
                        if (alive_ships[i]==SHIP_SIZE_Memory){
                            alive_ships[i]=0;
                            SHIP_SIZE_Memory=0;
                            direction=0;
                            end_of_a_ship=0;
                            in_the_process=false;
                        }
                    }
                }
                else if(end_of_a_ship==2 && SHIP_SIZE_Memory==1){
                    end_of_a_ship=0;
                }
            }
            else if (direction==1){
                direction_i=fired_x-1;
                if (direction_i<0){
                    end_of_a_ship++;
                    direction++;
                }
                else{
                    if (playerBoard[direction_i][fired_y] == SHIP) {
                        fired_x=direction_i;
                        playerBoard[fired_x][fired_y] = HIT;
                        enemyStrikeBoard[fired_x][fired_y] = SHIP;
                        SHIP_SIZE_Memory++;
                        cout << "Enemy hit your ship at " << char('A' + fired_y) << fired_x + 1 << "!\n";
                    }
                    else{
                        cout << "Enemy missed at " << char('A' + fired_y) << direction_i + 1 << ".\n";
                        playerBoard[direction_i][fired_y] = MISS;
                        enemyStrikeBoard[direction_i][fired_y] = HIT;
                        end_of_a_ship++;
                        fired_x=probable_x;
                        direction++;
                        fired=true;
                    }
                }
                if(end_of_a_ship==2 && SHIP_SIZE_Memory>1){
                    for (int i=0; i<5; i++){
                        if (alive_ships[i]==SHIP_SIZE_Memory){
                            alive_ships[i]=0;
                            SHIP_SIZE_Memory=0;
                            direction=0;
                            end_of_a_ship=0;
                            in_the_process=false;
                        }
                    }
                }
                else if(end_of_a_ship==2 && SHIP_SIZE_Memory==1){
                    end_of_a_ship=0;
                }
            }
            else if (direction==2){
                direction_i=fired_y+1;
                if (direction_i>BOARD_SIZE){
                    end_of_a_ship++;
                    direction++;
                }
                else{
                    if (playerBoard[fired_x][direction_i] == SHIP) {
                        fired_y=direction_i;
                        playerBoard[fired_x][fired_y] = HIT;
                        enemyStrikeBoard[fired_x][fired_y] = SHIP;
                        SHIP_SIZE_Memory++;
                        cout << "Enemy hit your ship at " << char('A' + fired_y) << fired_x + 1 << "!\n";
                    }
                    else{
                        cout << "Enemy missed at " << char('A' + direction_i) << fired_x + 1 << ".\n";
                        playerBoard[fired_x][direction_i] = MISS;
                        enemyStrikeBoard[fired_x][direction_i] = HIT;
                        end_of_a_ship++;
                        fired_y=probable_y;
                        direction++;
                        fired=true;
                    }
                }
                if(end_of_a_ship==2 && SHIP_SIZE_Memory>1){
                    for (int i=0; i<5; i++){
                        if (alive_ships[i]==SHIP_SIZE_Memory){
                            in_the_process=false;
                            alive_ships[i]=0;
                            SHIP_SIZE_Memory=0;
                            direction=0;
                            end_of_a_ship=0;
                        }
                    }
                }
                else if(end_of_a_ship==2 && SHIP_SIZE_Memory==1){
                    end_of_a_ship=0;
                }
            }
            else if (direction==3){
                direction_i=fired_y-1;
                if (direction_i<0){
                    end_of_a_ship++;
                }
                else{
                    if (playerBoard[fired_x][direction_i] == SHIP ) {
                        fired_y=direction_i;
                        playerBoard[fired_x][fired_y] = HIT;
                        enemyStrikeBoard[fired_x][fired_y] = SHIP;
                        SHIP_SIZE_Memory++;
                        cout << "Enemy hit your ship at " << char('A' + fired_y) << fired_x + 1 << "!\n";
                    }
                    else{
                        cout << "Enemy missed at " << char('A' + direction_i) << fired_x + 1 << ".\n";
                        playerBoard[fired_x][direction_i] = MISS;
                        enemyStrikeBoard[fired_x][direction_i] = HIT;
                        end_of_a_ship++;
                        fired_y=probable_y;
                        direction++;
                        fired=true;
                    }
                }
                if(end_of_a_ship==2 && SHIP_SIZE_Memory>1){
                    for (int i=0; i<5; i++){
                        if (alive_ships[i]==SHIP_SIZE_Memory){
                            in_the_process=false;
                            alive_ships[i]=0;
                            SHIP_SIZE_Memory=0;
                            direction=0;
                            end_of_a_ship=0;
                        }
                    }
                }
                else if(end_of_a_ship==2 && SHIP_SIZE_Memory==1){
                    end_of_a_ship=0;
                }
            }
        }
    }
}
//Yera Final



bool enemyTurn(
    int board[BOARD_SIZE][BOARD_SIZE],
    char diff,
    int enemySunk[6]
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
        else if (diff == '2') {
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
            if (enemySunk[s] < total) {
                spacing = s;
                break;
            }
        }
        for (int r = 0; r < BOARD_SIZE; ++r) {  
            for (int c = 0; c < BOARD_SIZE; ++c) {
         
                if ((r + c) % spacing != 0) 
                    continue;
        
        
                if (board[r][c] != EMPTY && board[r][c] != SHIP) 
                    continue;
        
        
                bool adjHit = false;
                for (int dr = -1; dr <= 1 && !adjHit; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) 
                            continue;
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr >= 0 && nr < BOARD_SIZE
                         && nc >= 0 && nc < BOARD_SIZE
                         && board[nr][nc] == HIT) {
                            adjHit = true;
                            break;
                        }
                    }
                }
                if (adjHit) 
                    continue;
        
        
                if (board[r][c] == SHIP) {
                    cout << "Enemy hit at " << char('A' + c) << (r + 1) << "!\n";
                    board[r][c] = HIT;
                    enemyAttacks.emplace_back(r, c);
                    return true;
                }
                else {
                    cout << "Enemy missed at " << char('A' + c) << (r + 1) << ".\n";
                    board[r][c] = MISS;
                    return false;
                }
            }
        }
    }
    else if (diff == '3'){
        enemyTurn_Pro(board);
    }
    return false;
}

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



int main_gameplay_loop(char difficulty_choice) {
    srand(static_cast<unsigned>(time(nullptr)));

    int playerBoard[BOARD_SIZE][BOARD_SIZE];
    int enemyBoard [BOARD_SIZE][BOARD_SIZE];
    int displayBoard[BOARD_SIZE][BOARD_SIZE];
    int humanSunk[6] = {0};
    int enemySunk[6] = {0};

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
                updateSunkShips(enemyBoard, humanSunk);
                if (isGameOver(enemyBoard)) {
                    cout << "You win!\n";
                    break;
                }
            }
        }
        else {
            hit = enemyTurn(playerBoard, difficulty_choice, enemySunk);
            if (hit) {
                updateSunkShips(playerBoard, enemySunk);
                if (isGameOver(playerBoard)) {
                    cout << "Enemy wins!\n";
                    break;
                }
            }
        }
        //to make the output
        cout<<"Press Enter to Update board.\n";
        cin.get();

        if (!hit) {
            playerTurn = !playerTurn;
        }
    }

    return 0;
}

