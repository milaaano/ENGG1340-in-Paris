// Include local files
#include "../1340 Group Project/battleship.h"
#include "../DataHandling/users.h"

// Include standard libraries
#include <iostream>

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

        while (!login_success)
        {

            if (loginUser(users, username, password, logged_user)) // Checks if login is successful
            {
                cout << "Welcome " << logged_user->getName() << "!" << endl; // No arguments in new vers
                cout << "Login successful" << endl;
                login_success = true;
            }
            else
            {
                cout << "Login failed" << endl;
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
        if (registerUser(users, username, password))
        {
            cout << "User created successfully" << endl;
        }
        else
        {
            cout << "User creation failed" << endl;
        }
    }
}

int main()
{
    // ---- Login or sign up ----
    // User 1
    Users users = buildUsers("../Data/users.db");
    User *logged_user1 = nullptr;
    login_or_signup(users, logged_user1);

    // User 2
    User *logged_user2 = nullptr;
    login_or_signup(users, logged_user2);

    // ---- Place ships ----
    int player1Board[0][10];
    int player2Board[0][10];
    initializeBoard(player1Board);
    initializeBoard(player2Board);

    // Play game
    // Update leaderboard
    return 0;
}