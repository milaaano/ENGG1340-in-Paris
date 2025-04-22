// Include local files
#include "../1340 Group Project/battleship.h"
#include "../DataHandling/users.h"

// Include standard libraries
#include <iostream>

// g++ -std=c++11 -pedantic-errors Multiplayer/multiplayer.cpp DataHandling/users.cpp "1340 Group Project/battleship.cpp" -o Multiplayer/multiplayer && Multiplayer/multiplayer
// g++ -std=c++11 -pedantic-errors multiplayer.cpp ../DataHandling/users.cpp "../1340 Group Project/battleship.cpp" -o multiplayer && ./multiplayer
using namespace std;

// Prompts the user to login or sign up
bool login_or_signup(Users &users, User *&logged_user)
{
    string login_option = "";
    while (login_option != "l" && login_option != "c")
    {
        cout << "Login or Create New User? (l/c)" << endl;
        cin >> login_option;
    }
    if (login_option == "l") // Login
    {
        // Prompts user to enter username and password
        string username = "";
        string password = "";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        bool login_success = false;

        // while (!login_success)
        // {

        //     if (loginUser(users, username, password, logged_user)) // Checks if login is successful
        //     {
        //         cout << "Welcome " << logged_user->getName() << "!" << endl; // No arguments in new vers
        //         cout << "Login successful" << endl;
        //         login_success = true;
        //         return true;
        //     }
        //     else
        //     {
        //         cout << "Login failed" << endl;
        //         return false;
        //     }
        // }
    }
    else if (login_option == "c") // Create new user
    {
        // Prompts user to enter username, password and password confirmation
        string username = "";
        string password = "";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        // if (registerUser(users, username, password))
        // {
        //     cout << "User created successfully" << endl;
        //     return true;
        // }
        // else
        // {
        //     cout << "User creation failed" << endl;
        //     return false;
        // }
    }
    return false;
}

int main()
{
    // ---- Login or sign up ----
    // User 1
    Users users = buildUsers("../Data/users.db");
    User *logged_user1 = nullptr;
    // login_or_signup(users, logged_user1);

    // User 2
    User *logged_user2 = nullptr;
    // login_or_signup(users, logged_user2);

    // ---- Place ships ----
    // Player 1 goes first
    int player1Board[10][10];
    initializeBoard(player1Board);
    printBoard(player1Board, true);

    // Player 2
    int player2Board[10][10];
    initializeBoard(player2Board);
    printBoard(player2Board, true);

    // Play game
    // Update leaderboard
    return 0;
}
