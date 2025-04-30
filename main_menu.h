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
void printMenu();

void printDifficulty();

void printPlay();

void printScoreboard();

