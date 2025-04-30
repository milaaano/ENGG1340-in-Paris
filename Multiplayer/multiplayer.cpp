// Include local files
#include "multiplayer.h"
// Include standard libraries

// g++ -std=c++11 -pedantic-errors Multiplayer/multiplayer.cpp DataHandling/users.cpp "1340 Group Project/battleship.cpp" -o Multiplayer/multiplayer && Multiplayer/multiplayer
// g++ -std=c++11 -pedantic-errors multiplayer.cpp ../DataHandling/users.cpp "../1340 Group Project/battleship.cpp" -o multiplayer && ./multiplayer

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
        if (get<0>(registerUser(users, username, password, leaderboard, logged_user)))
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

// Handles player 1 win and updates leaderboard. Takes in pointer to winner and users map.
void handleWin(User *&winner, User *&loser, Users &users)
{
    cout << winner->getName() << " wins!" << endl;
    updateRating(users[winner->getName()].score, users[loser->getName()].score, 1);
}

int main_multiplayer(Users & users, Leaderboard & leaderboard)
{
    // ---- Login or sign up ----
    cout << "Welcome to Battleship Multiplayer!" << endl;

    string path = "../Data/users.db";
    string current_users[2][2] = {{"", ""}, {"", ""}}; // [0][0] is player 1 username, [0][1] is player 1 password, [1][0] is player 2 username, [1][1] is player 2 password
    bool player1_logged_in = false;
    bool player2_logged_in = false;

    // User 1
    cout << "Please login or sign up for user 1" << endl;
    User *player1 = nullptr;
    player1_logged_in = false;
    while (!player1_logged_in)
    {
        player1_logged_in = login_or_signup(users, player1, leaderboard, current_users);
    }

    // User 2
    cout << "Please login or sign up for user 2" << endl;
    User *player2 = nullptr;
    player2_logged_in = false;
    while (!player2_logged_in)
    {
        player2_logged_in = login_or_signup(users, player2, leaderboard, current_users);
    }

    clearScreen();

    // ---- Place ships ----
    // Player 1 goes first
    cout << "Player 1, please place your ships" << endl;
    int player1Board[10][10];
    int player1Display[10][10];
    initializeBoard(player1Board);
    initializeBoard(player1Display);
    placePlayerShips(player1Board);

    clearScreen();

    // Player 2
    cout << "Player 2, please place your ships" << endl;
    int player2Board[10][10];
    int player2Display[10][10];
    initializeBoard(player2Board);
    initializeBoard(player2Display);
    placePlayerShips(player2Board);

    // --- Main game loop ---
    while (true)
    {
        cout << "\n--- " << player1->getName() << " Turn ---\n";
        printBoard(player1Display, false);
        humanTurn(player1Display, player2Board);
        if (isGameOver(player2Board))
        {
            handleWin(player1, player2, users);
            break;
        }
        clearScreen();
        cout << "\n--- " << player2->getName() << " Turn ---\n";
        printBoard(player2Display, false);
        humanTurn(player2Display, player1Board);
        if (isGameOver(player1Board))
        {
            handleWin(player2, player1, users);
            break;
        }
        clearScreen();
    }

    saveUsers(path, users);

    // Update leaderboard
    return 0;
}
