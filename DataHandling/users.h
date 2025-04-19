#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <vector>

using namespace std;

long long makePasswordHash(const string & password);

struct User {
private:
    string name;
    long long passwordHash;
public:
    int id, score;

    User(int id, string name, string password, int score) : id(id), name(name), score(score) {
        passwordHash = makePasswordHash(password);
    }

    void getPasswordHash(long long & Hash) {
        Hash = passwordHash;
    }
    void getName(string & name) {
        name = this->name;
    }
    void setPassword(string & password) {
        passwordHash = makePasswordHash(password);
    }
    void setName(string & name) {
        this->name = name;
    }
    void printUser(User * & user, int rank) {
        cout << left;
        cout << setw(4) << rank << "  " << setw(10) << user->name << "  " << setw(5) << score << '\n';
    }

    bool operator < (const User & other) const {
        return score < other.score;
    }
};

using Users = unordered_map<string, User>; // Kinda cache-db, key: name, value: User; neede for O(1) search and dynamic data handling
using Leaderboard = vector<User*>;

// ----DB handling----

void ensureLocalDB(const string& tpl, const string & db); // In the first run creates local db. For consequtive runs cheks existance of db

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

string Encrypt(const strint & data, const string & key);

#endif