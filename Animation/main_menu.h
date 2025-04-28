#pragma once
#include <iostream>
using namespace std;
namespace Console {
    /// Clears the terminal screen and moves cursor to (0,0)
    inline void clear() {
        // ANSI escape sequence: clear entire screen & home cursor
        cout << "\x1B[2J\x1B[H" << flush;
    }
}
void printMenu() {
    cout
      << "1. Play\n"
      << "2. Scoreboard\n"
      << "3. Difficulty\n"
      << "4. Exit\n\n";
}

void printDifficulty() {
    cout << "=== Difficulty selected ===\n"
          "1) Easy\n"
          "2) Normal\n"
          "3) Hard\n\n"
       << "Press Enter to return to menu...";
}

void printPlay() {
    cout << "=== Play selected ===\n\n"
       << "Press Enter to return to menu...";
}
void printScoreboard() {
    cout << "=== Scoreboard selected ===\n\n"
       << "Press Enter to return to menu...";
}

