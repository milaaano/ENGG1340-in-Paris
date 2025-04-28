// Include local files
#include "../DataHandling/users.h"
#include "../1340 Group Project/battleship.h"

// Include standard libraries
#include <iostream>
#include <utility>
#include <cstdlib>
using namespace std;

bool login_or_signup(Users &users, User *&logged_user, Leaderboard &leaderboard, string current_users[2][2]);
void handleWin(User *&winner, User *&loser, Users &users);
int main_multiplayer(Users &users, Leaderboard &leaderboard); // The only function you need