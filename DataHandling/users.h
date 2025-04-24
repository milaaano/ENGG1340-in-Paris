#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <random>
#include "constants.h"

extern bool leaderboardDirtyBit; // true if the leaderboard was changed, false else. If dirty bit is true the leaderboard will be sorted before printing. needed only for Milan

extern unordered_map<char, ll> alphabet; // for polynomail hash, needed only for Milan

extern vector<ll> powers; // for polynomial hash. needed only for Milan


string makeSalt(); // will not be used

ll makePasswordHash(const string & password); // hashes passwords. needed only for Milan

ll gtSaltedHash(const string & password, const string & salt); // will not be used

// User struct with fields: name, password, and score. Rank is the standing in the leaderboard, determined during printing.
struct User {
private:
    string name;
    ll passwordHash;
public:
    int score;

    User() : name(""), passwordHash(0), score(start_rating) {} // default constructor

    // constructor with parameters: name, password (string), score
    User(string name, string password, int score) : name(name), score(score) {
        passwordHash = makePasswordHash(password);
    }

    User(string name, ll hashed_password, int score) : name(name), passwordHash(hashed_password), score(score) {} // constructor with parameters: name, hashed_password, score

    ll getPasswordHash() const {
        return passwordHash;
    }
    string getName() const {
        return name;
    }
    void setPassword(string & password) {
        passwordHash = makePasswordHash(password);
    }
    void setName(string & name) {
        this->name = name;
    }
    void printUser(int rank) const {
        cout << left;
        cout << setw(4) << rank << "  " << setw(10) << name << "  " << setw(5) << score << '\n';
    }

    // comparison oparation for User structure. only < available.
    bool operator < (const User & other) const {
        return score < other.score;
    }
};

using Users = unordered_map<string, User>; // Kinda cache-db which record all the changes during runtime. Key: name, value: User. Needed for O(1) search and dynamic data handling
using Leaderboard = vector<User*>; // leaderboard containing pointers to the User objects in Users map;

// ----DB handling----

Users buildUsers(const string& path); // builds users. In the very beggining of main() one need to create a Users object and build it, otherwise all the data will not be recorded.

void saveUsers(const string & path, const Users & users); // saves changes from Users object to the DB. Must be recalled in the end of the runtime, otherwise the data will not be stored in the DB

// ----Login/registration----

pair<bool, string> registerUser(Users & users, const string & username, const string & password, Leaderboard & leaderboard); // Registers users. Password is in form of string. Returns pair of bool (result of the registration) and message (representing error or  successful registration)

pair<bool, string> loginUser(const Users & users, const string & username, const string & password, User * & logged_user); // Logs users in. Return value is the same as above. User * & logged_user - pointer to the logged user in the Users map, needed to change user's rating after the game. One should create a pointer (not dynamic) and pass it as the argument.

// ----Leaderboard----

void buildLeaderboard(Users & users, Leaderboard & leaderboard); // Builds a leaderboard from Users. Need to be recalled right after built of Users.

void printLeaderboard(Leaderboard & leaderboard); // Prints the leader board with the width of 20 characters

// ----Security----

void clearScreen(); // clean terminal. Read somewhere how to use it for Ubuntu

void Encrypt(string & data, const string & key); // will not be used

// ----Rating System----

void updateRating(int & a, int & b, int outcome); // takes two numbers by reference, which are "user.score", and outcome = 1 if "a" won, 0 otherwise. Updates users ratings.

#endif