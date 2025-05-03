// Include local files
#include "multiplayer.h"
#include <limits>
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
        if ((current_users[1][0] == "" && current_users[1][1] == "") && (current_users[0][0] == username && current_users[0][1] == password))
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
        pair<bool, string> login_status;
        while (!login_success)
        {
            login_status = loginUser(users, username, password, logged_user);
            if (login_status.first) // Checks if login is successful
            {
                cout << "=== Welcome " << username << "! ===" << endl; // No arguments in new version
                cout << "Press Enter to continue..." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();

                login_success = true;

                // If Player 1 slot is empty, assign credentials there.
                if (current_users[0][0] == "" && current_users[0][1] == "")
                {
                    current_users[0][0] = username;
                    current_users[0][1] = password;
                }
                // Else if Player 1 slot is filled AND Player 2 slot is empty, assign credentials to Player 2 slot.
                else if (!(current_users[0][0] == "" && current_users[0][1] == "") && current_users[1][0] == "" && current_users[1][1] == "")
                {
                    current_users[1][0] = username;
                    current_users[1][1] = password;
                }

                return true;
            }
            else
            {
                cout << login_status.second << endl;
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
void handleWin(string winner, string loser, Users &users)
{
    cout << winner << " wins!" << endl;
    updateRating(users[winner].score, users[loser].score, 1);
}

int main_multiplayer(Users &users, Leaderboard &leaderboard)
{
    // ---- Login or sign up ----
    cout << "=== Welcome to Battleship Multiplayer! ===" << endl;

    cout << "Press Enter to continue or type 'exit' to go back to main menu" << endl;
    string continue_multiplayer;
    getline(cin, continue_multiplayer);
    if (continue_multiplayer == "exit" || continue_multiplayer == "Exit")
    {
        return 0;
    }
    clearScreen();

    string path = "../Data/users.db";
    string current_users[2][2] = {{"", ""}, {"", ""}}; // [0][0] is player 1 username, [0][1] is player 1 password, [1][0] is player 2 username, [1][1] is player 2 password
    bool player1_logged_in = false;
    bool player2_logged_in = false;

    // User 1
    User *player1 = nullptr;
    player1_logged_in = false;
    while (!player1_logged_in)
    {
        cout << "Please login or sign up for user 1" << endl;
        player1_logged_in = login_or_signup(users, player1, leaderboard, current_users);
    }

    clearScreen();

    // User 2
    User *player2 = nullptr;
    player2_logged_in = false;
    while (!player2_logged_in)
    {
        cout << "Please login or sign up for user 2" << endl;
        player2_logged_in = login_or_signup(users, player2, leaderboard, current_users);
    }

    clearScreen();
    int sunkShipsArrPlayer1[6] = {0};
    int sunkShipsArrPlayer2[6] = {0};

    // ---- Place ships ----
    // Player 1 goes first
    cout << current_users[0][0] << ", please place your ships (press enter to continue)" << endl;
    int player1Board[10][10];   // P1's ships
    int player1Display[10][10]; // P1's view of P2's board (hits/misses P1 made)
    initializeBoard(player1Board);
    initializeBoard(player1Display);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string input;
    if (getline(cin, input))
    {
        placePlayerShips(player1Board);
    }

    clearScreen();

    // Player 2
    cout << current_users[1][0] << ", please place your ships (press enter to continue)" << endl;
    getline(cin, input);
    int player2Board[10][10];   // P2's ships
    int player2Display[10][10]; // P2's view of P1's board (hits/misses P2 made)
    initializeBoard(player2Board);
    initializeBoard(player2Display);
    placePlayerShips(player2Board);

    string next_user_turn = current_users[0][0];

    clearScreen();

    // --- Main game loop ---
    while (true)
    {
        bool hit = false;
        // Player 1's turn
        while (next_user_turn == current_users[0][0])
        {
            clearScreen();
            cout << "\n------ " << current_users[0][0] << " Turn ------\n";
            cout << "Target Board (You are shooting " << current_users[1][0] << "'s ships):" << endl;
            printBoard(player1Display, false);
            cout << "Your Board (You are being shot at by " << current_users[1][0] << "):" << endl;
            printBoard(player1Board, true);
            hit = humanTurn(player1Display, player2Board);
            if (hit)
            {
                updateSunkShips(player2Board, sunkShipsArrPlayer2);
                if (isGameOver(player2Board))
                {
                    handleWin(current_users[0][0], current_users[1][0], users);
                    cout << "\nPress Enter to continue..." << endl;
                    cin.get();
                    return 0;
                }
            }
            else
            {
                next_user_turn = current_users[1][0];
            }
        }

        if (isGameOver(player1Board) || isGameOver(player2Board))
        {
            break;
        }

        if (!isGameOver(player1Board) && !isGameOver(player2Board))
        {
            cout << "\nPress Enter to continue to " << next_user_turn << "'s turn...";
            cin.get();
        }

        // Player 2's turn
        while (next_user_turn == current_users[1][0])
        {
            clearScreen();
            cout << "\n--- " << current_users[1][0] << " Turn ---\n";
            cout << "Target Board (You are shooting " << current_users[0][0] << "'s ships):" << endl;
            printBoard(player2Display, false);
            cout << "Your Board (You are being shot at by " << current_users[0][0] << "):" << endl;
            printBoard(player2Board, true);
            hit = humanTurn(player2Display, player1Board);
            if (hit)
            {
                updateSunkShips(player1Board, sunkShipsArrPlayer1);
                if (isGameOver(player1Board))
                {
                    handleWin(current_users[1][0], current_users[0][0], users);
                    cout << "\nPress Enter to continue..." << endl;
                    cin.get();
                    return 0;
                }
            }
            else
            {
                next_user_turn = current_users[0][0];
            }
        }

        if (!isGameOver(player1Board) && !isGameOver(player2Board))
        {
            cout << "\nPress Enter to continue to " << next_user_turn << "'s turn...";
            cin.get();
        }
    }

    // Update leaderboard
    return 0;
}
