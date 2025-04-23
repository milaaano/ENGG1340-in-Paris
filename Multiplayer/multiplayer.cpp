// Include local files
#include "../DataHandling/users.h"
#include "../1340 Group Project/battleship.h"

// Include standard libraries
#include <iostream>
#include <utility>
#include <cstdlib>

// g++ -std=c++11 -pedantic-errors Multiplayer/multiplayer.cpp DataHandling/users.cpp "1340 Group Project/battleship.cpp" -o Multiplayer/multiplayer && Multiplayer/multiplayer
// g++ -std=c++11 -pedantic-errors multiplayer.cpp ../DataHandling/users.cpp "../1340 Group Project/battleship.cpp" -o multiplayer && ./multiplayer
using namespace std;

// Prompts the user to login or sign up
bool login_or_signup(Users &users, User *&logged_user, Leaderboard &leaderboard, string current_users[2][2])
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

        // Checks if player 1 is currently logging in
        if (current_users[0][0] == "" && current_users[0][1] == "")
        {
            current_users[0][0] = username;
            current_users[0][1] = password;
        }
        else if ((current_users[1][0] == "" && current_users[1][1] == "") && (current_users[0][0] == username && current_users[0][1] == password))
        {
            cout << "Sorry, " << username << " is already logged in!" << endl;
            return false;
        }
        else if (current_users[1][0] == username && current_users[1][1] == password)
        {
            current_users[1][0] = username;
            current_users[1][1] = password;
        }

        bool login_success = false;

        while (!login_success)
        {

            if (get<0>(loginUser(users, username, password, logged_user))) // Checks if login is successful
            {
                cout << "Welcome " << logged_user->getName() << "!" << endl; // No arguments in new vers
                cout << "Login successful" << endl;
                login_success = true;
                return true;
            }
            else
            {
                cout << "Login failed" << endl;
                return false;
            }
        }
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
        if (get<0>(registerUser(users, username, password, leaderboard)))
        {
            cout << "User created successfully" << endl;
            return true;
        }
        else
        {
            cout << "User creation failed" << endl;
            return false;
        }
    }
    return false;
}

int main()
{
    // ---- Login or sign up ----
    cout << "Welcome to Battleship Multiplayer!" << endl;

    Users users = buildUsers("../Data/users.db");
    Leaderboard leaderboard;
    buildLeaderboard(users, leaderboard);
    string current_users[2][2] = {{"", ""}, {"", ""}}; // [0][0] is player 1 username, [0][1] is player 1 password, [1][0] is player 2 username, [1][1] is player 2 password
    bool player1_logged_in = false;
    bool player2_logged_in = false;

    // User 1
    cout << "Please login or sign up for user 1" << endl;
    User *logged_user1 = nullptr;
    player1_logged_in = false;
    while (!player1_logged_in)
    {
        player1_logged_in = login_or_signup(users, logged_user1, leaderboard, current_users);
    }

    // User 2
    cout << "Please login or sign up for user 2" << endl;
    User *logged_user2 = nullptr;
    player2_logged_in = false;
    while (!player2_logged_in)
    {
        player2_logged_in = login_or_signup(users, logged_user2, leaderboard, current_users);
    }

    system("clear");

    // ---- Place ships ----
    // Player 1 goes first
    cout << "Player 1, please place your ships" << endl;
    int player1Board[10][10];
    initializeBoard(player1Board);
    printBoard(player1Board, true);

    system("clear");

    // Player 2
    cout << "Player 2, please place your ships" << endl;
    int player2Board[10][10];
    initializeBoard(player2Board);
    printBoard(player2Board, true);

    // Play game
    // Update leaderboard
    return 0;
}
