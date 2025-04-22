#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <random>
#include "constants.h"

extern unordered_map<char, ll> alphabet;

extern vector<ll> powers;

string makeSalt();

ll makePasswordHash(const string & password);

ll gtSaltedHash(const string & password, const string & salt);

struct User {
private:
    string name;
    ll passwordHash;
public:
    int score;

    User() : name(""), passwordHash(0), score(0) {}

    User(string name, string password, int score) : name(name), score(score) {
        passwordHash = makePasswordHash(password);
    }

    User(string name, ll hashed_password, int score) : name(name), passwordHash(hashed_password), score(score) {}

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

    bool operator < (const User & other) const {
        return score < other.score;
    }
};

using Users = unordered_map<string, User>; // Kinda cache-db, key: name, value: User; neede for O(1) search and dynamic data handling
using Leaderboard = vector<User*>;

// ----DB handling----

Users buildUsers(const string& path); // builds users in live db

void saveUsers(const string & path, const Users & users); // atomic save

// ----Login/registration----

bool registerUser(Users & users, const string & username, const string & password);

bool loginUser(const Users & users, const string & username, const string & password, User * & logged_user);

// ----Leaderboard----

void buildLeaderboard(const Users & users, Leaderboard & leaderboard);

void printLeaderboard(const Leaderboard & leaderboard);

// ----Security----

void clearScreen();

void Encrypt(string & data, const string & key);

#endif