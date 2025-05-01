// battleships_frontpage_smooth_noecho.cpp
#include <iostream>
#include <vector>
// #include <algorithm>
// #include <chrono>
#include <unistd.h>
// #include <thread>
#include "../main_menu.h"
#include <cstdlib>    // for system()
// #include "../Multiplayer/multiplayer.h"
// #include "../DataHandling/users.h"
// #include "../1340 Group Project/battleship.h"

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
}

// --- Animation base (C++11) ---
/*
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
            auto delay = frameDuration - chrono::duration_cast<chrono::milliseconds>(elapsed);
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
*/


// Flashing animation build

/* static const int BOARD_SIZE = 10;
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
*/


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

void printMenu(){
    /* static const char* opts[] = {
        "Play",
        "Scoreboard",
        "Difficulty",
        "Exit"
    };
    for (int i = 0; i < 4; ++i) {
        if (i == choice) cout << YELLOW << "> ";
        else             cout << "  ";
        cout << (i+1) << ". " << opts[i] << RESET << "\n";
    } */
    cout << "1. Play" << endl;
    cout << "2. Scoreboard" << endl;
    cout << "3. Difficulty" << endl;
    cout << "4. Exit" << endl;
}

int main() {
    /*
     string path = "../Data/users.db";
     Users users;
     Leaderboard leaderboard;
     buildUsers(path);
     buildLeaderboard(users, leaderboard);
*/


    int choice;
    bool running = true;
    configure_terminal();

    while (running){
        Console::clear();
        printHeader();
        printMenu();
        // immediate keypress read
        // int input;
        // int result;
        read(STDIN_FILENO, &choice, 1);
        /* switch (input) {
                    case 'w':
                        if (choice > 1) {
                            choice--;
                        }
                        break;
                    case 's':
                        if (choice < 5) {
                            choice++;
                        }
                        break;
                    case '\n': // Enter key */
        switch (choice) {
            case 1:
                Console::clear();
                cout << "=== Play button was run ==="
                << endl
                << "Press any key to return..." << endl;
                cin.ignore();
                case 2:
                // Scoreboard
                Console::clear(); // main_multiplayer(users, leaderboard);
                cout << "=== Scoreboard button was run ==="
                << endl
                << "Press any key to return..." << endl;
                cin.ignore();
                break;

                case 3:
                Console::clear();
                cout << "=== Scoreboard button was run ==="
                << endl << "Press any key to return..." << endl;
                cin.ignore();
                break;

                case 4:
                // Exit the menu
                Console::clear();
                std::cout << "Exiting the menu." << std::endl;
                running = false;
                break;
        }
        break;

    }
    reset_terminal();
    return 0;
}
