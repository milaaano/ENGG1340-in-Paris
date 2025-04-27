// battleships_frontpage_smooth_noecho.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unistd.h>
#include <thread>
#include "C:\Users\ilyas\CLionProjects\ENGG1340-in-Paris\DataHandling\users.cpp"
#include "C:\Users\ilyas\CLionProjects\ENGG1340-in-Paris\DataHandling\users.h"
#include "C:\Users\ilyas\CLionProjects\ENGG1340-in-Paris\DataHandling\constants.h"
#include <cstdlib>    // for system()

using namespace std;

// ANSI color codes
constexpr char const* CYAN   = "\033[36m";
constexpr char const* YELLOW = "\033[33m";
constexpr char const* RESET  = "\033[0m";

// Clear screen + move cursor home
#define CLEAR_SCREEN() cout << "\033[2J\033[H"

// Terminal raw mode via stty, no echo
void configure_terminal(){
    // disable canonical mode & echo
    system("stty -icanon -echo min 1 time 0");
    // hide cursor
    cout << "\033[?25l";
    cout.flush();
}

void reset_terminal(){
    // show cursor, reset colors
    cout << "\033[?25h" << RESET;
    cout.flush();
    // restore default terminal behaviour
    system("stty sane");
}

// --- Animation base (C++11) ---
class Animation {
public:
    void run() {
        initialize();
        while (frameCount < maxFrames) {
            auto start = chrono::high_resolution_clock::now();
            update();
            render();
            auto end = chrono::high_resolution_clock::now();
            auto elapsed   = end - start;
            auto delay = frameDuration
                       - chrono::duration_cast<chrono::milliseconds>(elapsed);
            if (delay.count() > 0)
                this_thread::sleep_for(delay);
            ++frameCount;
        }
    }
protected:
    int frameCount = 0;
    static const int maxFrames = 80;
    const chrono::milliseconds frameDuration{50};
    virtual void initialize() {}
    virtual void update() {}
    virtual void render() {
        CLEAR_SCREEN();
    }
};

// --- Flashing‐ship animation for Battleships ---
static const int BOARD_SIZE = 10;
static const int EMPTY = 0, SHIP = 1, HIT = 2, MISS = 3;

class FlashingShipAnimation : public Animation {
    int (&board)[BOARD_SIZE][BOARD_SIZE];
    vector<pair<int,int>> shipCells;
    static const int flashInterval = 5;
public:
    FlashingShipAnimation(int (&b)[BOARD_SIZE][BOARD_SIZE],
                          vector<pair<int,int>> coords)
      : board(b), shipCells(std::move(coords)) {}

protected:
    void render() override {
        CLEAR_SCREEN();
        cout << "    ";
        for (char c = 'A'; c < 'A' + BOARD_SIZE; ++c) cout << c << ' ';
        cout << "\n";

        bool on = ((frameCount / flashInterval) % 2 == 0);
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << (i+1<10?" ":"") << (i+1) << "  ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                bool flash = on &&
                             (find(shipCells.begin(),
                                   shipCells.end(),
                                   make_pair(i,j))
                              != shipCells.end());
                if      (flash)           cout << "# ";
                else if (board[i][j]==HIT)   cout << "x ";
                else if (board[i][j]==MISS)  cout << "o ";
                else if (board[i][j]==SHIP)  cout << "S ";
                else                          cout << ". ";
            }
            cout << "\n";
        }
    }
};

// --- Front‐page drawing ---
 void printHeader() {
    cout << CYAN;
    cout << "######     ##    ######  ######  ##      ######  ######  ######  ##  ##  ###### ####### ######      \n";
    cout << "##   #   ##  ##    ##      ##    ##      ##      ##      ##      ##  ##    ##   ##   ## ##    \n";
    cout << "######   ######    ##      ##    ##      ######  ######  ######  ######    ##   ####### ######    \n";
    cout << "##   #   ##  ##    ##      ##    ##      ##      ##          ##  ##  ##    ##   ##          ##  \n";
    cout << "######   ##  ##    ##      ##    ######  ######  ######  ######  ##  ##  ###### ##      ###### \n";


        cout << RESET;


}

/* void printHeader(){
cout<< CYAN;
cout<<"████   ███  █████ █████ █     █████  ███  █   █  ███  ████\n";
cout<<"█   █ █   █   █     █   █     █     █     █   █   █   █   █\n";
cout<<"████  █████   █     █   █     ████   ███  █████   █   ████\n" ;
cout<<"█   █ █   █   █     █   █     █         █ █   █   █   █\n"   ;
cout<<"████  █   █   █     █   █████ █████  ███  █   █  ███  █\n" ;
}*/
void printMenu(int choice){
    static const char* opts[] = {
        "Play",
        "Scoreboard",
        "Difficulty",
        "Exit"
    };
    for (int i = 0; i < 4; ++i) {
        if (i == choice) cout << YELLOW << "> ";
        else             cout << "  ";
        cout << (i+1) << ". " << opts[i] << RESET << "\n";
    }
}

int main() {
     configure_terminal();
     string db_path = "Data/users.db";
     Users users = buildUsers(db_path);
	Leaderboard leaderboard;
	buildLeaderboard(users, leaderboard);
    int choice = 0;
    bool running = true;
    while (running){
        CLEAR_SCREEN();
        printHeader();
        printMenu(choice);
        cout.flush();

        // immediate keypress read
        char c;
		read(STDIN_FILENO, &c, 1);

        if      (c == 'w' && choice > 0)    --choice;
        else if (c == 's' && choice < 3)    ++choice;
        else if (c == '\n' || c == '\r') {
            switch (choice) {
              case 0: {  // Play
                static int board[BOARD_SIZE][BOARD_SIZE] = {{EMPTY}};
                board[4][4] = HIT;               // one hit
                vector<pair<int,int>> ship{{4,4}};
                FlashingShipAnimation anim(board, ship);
                anim.run();
				cin.ignore();
                break;
              }
              case 1: { // Scoreboard
                CLEAR_SCREEN();
                printLeaderboard(leaderboard);
                cin.ignore();
                break;
              }
              case 2: { // Difficulty
                CLEAR_SCREEN();
                cout << "=== Difficulty ===\n1. Easy\n2. Normal\n3. Hard\n"
                        "Press any key to return...";
                cin.ignore();
                break;
              }
              case 3:
                running = false;
                break;
            }
        }
    }

    reset_terminal();
    return 0;
}
