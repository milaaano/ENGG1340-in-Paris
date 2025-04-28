// battleships_frontpage_smooth_noecho.cpp
#include <iostream>
#include "main_menu.h"
using namespace std;

// ANSI color codes
constexpr char const* CYAN   = "\033[36m";
constexpr char const* YELLOW = "\033[33m";
constexpr char const* RESET  = "\033[0m";

void printHeader() {
    cout << CYAN
         << "######     ##    ######  ######  ##      ######  ######  ######  ##  ##  ###### ####### ######      \n"
            "##   #   ##  ##    ##      ##    ##      ##      ##      ##      ##  ##    ##   ##   ## ##    \n"
            "######   ######    ##      ##    ##      ######  ######  ######  ######    ##   ####### ######    \n"
            "##   #   ##  ##    ##      ##    ##      ##      ##          ##  ##  ##    ##   ##          ##  \n"
            "######   ##  ##    ##      ##    ######  ######  ######  ######  ##  ##  ###### ##      ###### \n"
         << RESET << "\n";
}

int main() {
    bool running = true;
    string line;
    while (running) {
        Console::clear();

        printMenu();
        cout << "Select an option (1 to 4), or press Enter to refresh: ";

        if (!getline(cin, line)) break;  // EOF
        if (line.empty()) continue;       // just Enter → redraw

        char choice = line[0];
        switch (choice) {
          case '1':
            Console::clear();
            printPlay();
            getline(cin, line);
            break;

          case '2':
            Console::clear();
            printScoreboard();
            getline(cin, line);
            break;

          case '3':
            Console::clear();
            printDifficulty();
            getline(cin, line);
            break;

          case '4':
            Console::clear();
            cout << "Thank you for playing!"<<endl;
            running = false;
            break;

          default:
            // invalid: just redraw
            break;
        }
    }

    return 0;
}
