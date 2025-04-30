// battleships_frontpage_smooth_noecho.cpp
#include <iostream>
#include "main_menu.h"

#include "DataHandling/users.h"
#include "Multiplayer/multiplayer.h"
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

void printMenu() {
  cout
    << "1. Versus AI\n"
    << "2. Scoreboard\n"
    << "3. Multiplayer\n"
    << "4. Registeration\n"
    << "5. Exit\n\n";
}

void printDifficulty() {
  cout << "=== Difficulty selected ===\n"
        "1) Easy\n"
        "2) Normal\n"
        "3) Hard\n"
        "4) Exit Versus AI Mode\n\n"
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

int main() {
    string path = "Data/users.db";
    Users users = buildUsers(path);
    Leaderboard leaderboard;
    buildLeaderboard(users, leaderboard);
    int multiplayer_status;
    bool running = true;
    string line;
    while (running) {
        Console::clear();

        printMenu();
        cout << "Select an option (1 to 5), or press Enter to refresh: ";

        if (!getline(cin, line)) break;  // EOF
        if (line.empty()) continue;       // just Enter → redraw

        char choice = line[0];
        string valid_mode_choice = "1234";
        char difficulty_choice;
        switch (choice) {
          case '1': 
            // Chose difficulty
            Console::clear();
            printDifficulty();

            getline(cin, line);
            while (line.empty() || valid_mode_choice.find(line[0]) == string::npos) {
              cout << "Invalid Choice!\n";
              getline(cin, line);
            }
            difficulty_choice = line[0];
            switch (difficulty_choice) {
              case '1':
                // Easy mode
                break;  
              case '2':
                // Medium mode
                break;
              case '3':
                // Hard mode
                break;
              case '4':
                continue;
              default:
                // Invaid choice
                break;
            }

            // Game starts - should be put in switch
            Console::clear();
            printPlay();
            getline(cin, line);
            break;

          case '2':
            Console::clear();
            printLeaderboard(leaderboard);
            getline(cin, line);
            break;

          case '3':
            Console::clear();
            multiplayer_status = main_multiplayer(users, leaderboard);
            if (multiplayer_status == 1) {
              cout << "Error occured during multiplayer!\n";
              exit(1);
            }
            break;
          case '4': {
            Console::clear();
            cout << "Registration\n";
            cout << "Enter 'exit' as username to go back to the main menu\n";
            // pair<bool, string> registerUser(Users & users, const string & username, const string & password, Leaderboard & leaderboard);
            string username = "";
            string password = "";
            cout << "Enter username: ";
            getline(cin, username);
            if (username == "Exit" || username == "exit") {
              break;
            }
            cout << "Enter password: ";
            getline(cin, password);
            pair<bool, string> status = registerUser(users, username, password, leaderboard);
            cout << status.second;
            break;
          }
          case '5':
            Console::clear();
            cout << "Thank you for playing!\n";
            running = false;
            saveUsers(path, users);
            break;
          default:
            // invalid: just redraw
            break;
        }
    }

    return 0;
}
