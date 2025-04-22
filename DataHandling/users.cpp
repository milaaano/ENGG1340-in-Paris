#include "users.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "constants.h"

string makeSalt()
{
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
    for (int i = 0; i < l; i++)
    {
        data[i] ^= key[i % l];
    }
}

Users buildUsers(const string &path)
{
    ifstream ifs;
    ifs.open(path.c_str());
    if (ifs.fail())
    {
        cout << "Error in file opening" << '\n';
        exit(1);
    }

    Users users;
    string line;
    string name;
    ll password_hash;
    int score;
    while (getline(ifs, line))
    {
        Encrypt(line, CryptoKey);

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

    return users;
}

// void saveUsers(const string & path, const Users & users) {
//     string tmp = path + ".tmp";
//     ofstream ofs;
//     ofs.open(tmp.c_str());
//     if (ofs.fail()) {
//         cout << "Can't open the tmp file" << '\n';
//         exit(1);
//     }

//     string line = "";
//     for (User & user : users) {
//         line += user.getName() +
//     }
// }