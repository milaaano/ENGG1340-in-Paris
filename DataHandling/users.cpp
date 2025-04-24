#include "users.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "constants.h"
#include <cmath>
#include <algorithm>

bool leaderboardDirtyBit = false;

string makeSalt() {
    int ln = 8;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 127);
    string salt = "";
    for (int i = 0; i < 8; i++)
    {
        salt += (char)(dis(gen));
    }
    return salt;
}

// fill alphabet with chars from 1 to 127 (included)
unordered_map<char, ll> fillAlphabet()
{
    unordered_map<char, ll> alphabet;
    for (int i = 1; i < 128; i++)
    {
        alphabet[(char)i] = i;
    }
    return alphabet;
}

unordered_map<char, ll> alphabet = fillAlphabet();

// fill powers
vector<ll> makePowers()
{
    vector<ll> powers(20, 1);
    for (int i = 1; i < 20; i++)
    {
        powers[i] = (powers[i - 1] * k) % p;
    }
    return powers;
}

vector<ll> powers = makePowers();

// password is no longer than 20 symbols
ll makePasswordHash(const string &password)
{
    int n = password.length();
    ll hash = 0;
    for (int i = 0; i < n; i++)
    {
        hash = (((hash * k) % p) + alphabet[password[i]]) % p;
    }
    return hash;
}

void Encrypt(string &data, const string &key)
{
    int l = key.length();
    int n = data.length();
    for (int i = 0; i < n; i++) {
        data[i] ^= key[i % l];
    }
}

Users buildUsers(const string &path)
{
    ifstream ifs;
    ifs.open(path.c_str());
    if (ifs.fail()) {
        cout << "Error in file opening\n";
        exit(1);
    }

    Users users;
    string line;
    string name;
    ll password_hash;
    int score;
    while (getline(ifs, line)) {

        for (char &ch : line)
        {
            if (ch == ',')
            {
                ch = ' ';
            }
        }

        istringstream iss(line);

        iss >> name >> password_hash >> score;

        User cur_user(name, password_hash, score);
        users[name] = cur_user;
    }
    ifs.close();

    return users;
}

void saveUsers(const string & path, const Users & users) {
    string tmp = path;
    ofstream ofs;
    ofs.open(tmp.c_str());
    if (ofs.fail()) {
        cout << "Can't open the tmp file\n" << '\n';
        exit(1);
    }

    for (const auto & pair : users) {
        const User &u = pair.second;
        string line = u.getName() + "," + to_string(u.getPasswordHash()) + "," + to_string(u.score);
        ofs << line << '\n';
    }
    ofs.close();
}

pair<bool, string> registerUser(Users & users, const string & username, const string & password, Leaderboard & leaderboard) {
    pair<bool, string> response = {true, "Successful registration!\n"};
    if (users.find(username) != users.end()) {
        response.first = false;
        response.second = "User " + username + " already exists!\n";
        return response;
    }
    for (int i = 0; i < username.length(); i++) {
        if (username[i] > 127 || username[i] < 1) {
            response.first = false;
            response.second = "Invalid password!\n";
            return response;
        }
    }
    users[username] = User(username, password, start_rating);
    leaderboardDirtyBit = true;
    leaderboard.push_back( &(users[username]));
    clearScreen();
    return response;
}


pair<bool, string> loginUser(const Users & users, const string & username, const string & password, User * & logged_user) {
    auto it = users.find(username);
    if (it == users.end()) {
        return {false, "No users with username" + username + "!\n"};
    }
    ll attempt = makePasswordHash(password);
    if (attempt != it->second.getPasswordHash()) {
        return {false, "Wrong password!\n"};
    }
    User u = it->second;
    logged_user = &u;
    clearScreen();
    return {true, "Successfull login!\n"};
}

void buildLeaderboard(Users & users, Leaderboard & leaderboard) {
    for (auto & pair : users) {
        leaderboard.push_back(&(pair.second));
    }
    leaderboardDirtyBit = true;
}

void printLeaderboard(Leaderboard & leaderboard) {
    if (leaderboard.empty()) {
        cout << "No registered users!\n";
        return;
    }
    if (leaderboardDirtyBit) {
        sort(leaderboard.begin(), leaderboard.end(), [](User * a, User * b) {
            return !(*a < *b);
        });
    }
    cout << setfill('-') << setw(15) << right << "Leaderboard" << setw(4) << '\n';
    cout <<setfill(' ');
    for (int i = 0; i < leaderboard.size(); i++) {
        (leaderboard[i]->printUser(i + 1));
    }
}

double surprise_boost(double gap) {
    return 1.0 + (gap * gap) / (D * D);
}

void updateRating(int & a, int & b, int outcome) {
    double E_a = 1.0 / (1.0 + pow(10.0, (b - a) / D));

    double gap = b - a;
    double boost = surprise_boost(gap);
    double d_a = C * (outcome - E_a) * boost;

    if (outcome == 1) {
        a = min({round(a + d_a), round(a * maxgain)});
        b = max({round(b - d_a), round(b * maxlost)});
    } else {
        a = max({round(a + d_a), round(a * maxlost)});
        b = min({round(b - d_a), round(b * maxgain)});
    }
}

void clearScreen() {
    system("clear");
}

int main() {
    int a = 600, b = 600;
    updateRating(a, b, 0);
    cout << a << "    " << b << endl;

    // Users users;
    // string username, password;
    // int score;
    // Leaderboard leaderboard;
    // buildLeaderboard(users, leaderboard);
    // for (int i = 0; i < 5; i++) {
    //     cin >> username >> password;
    //     registerUser(users, username, password, leaderboard);
    // }
    // printLeaderboard(leaderboard);
    
    // saveUsers("/Users/macbook/ProgrammingProjects/ENGG1340-in-Paris/Data/users.db", users);
    // Users hello = buildUsers("/Users/macbook/ProgrammingProjects/ENGG1340-in-Paris/Data/users.db");
    // for (const auto & el : hello) {
    //     el.second.printUser(52);
    // }

    // for (int i = 0; i < 3; i++) {
    //     cin >> username >> password;
    //     User * ptr = nullptr;
    //     pair<bool, string> res = loginUser(users, username, password, ptr);
    //     cout << res.second;
    //     if (res.first) {
    //         ptr->score = i + 100;
    //     }
        
    // }

    // printLeaderboard(leaderboard);

    // cout << '\n';
}