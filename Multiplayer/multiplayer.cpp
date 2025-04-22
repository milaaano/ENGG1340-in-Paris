// Include local files
#include "../1340 Group Project/battleship.h"
#include "../DataHandling/users.h"

// Include standard libraries
#include <iostream>

using namespace std;

int main()
{
    // Login or sign up
    string login_option = "";
    while (login_option != "l" && login_option != "c")
    {
        cout << "Login or Create New User? (l/c)" << endl;
        cin >> login_option;
    }
    if (login_option == "l")
    {
        // Prompts user to enter username and password
    }
    else if (login_option == "c")
    {
        // Prompts user to enter username, password and password confirmation
    }

    // Place ships
    // Play game
    // Update leaderboard
    return 0;
}