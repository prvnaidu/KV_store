#include "Database.h"
#include <unordered_map>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

unordered_map<string, string> db;

unordered_map<string, long long> expiryTime;

long long getCurrentTime()
{
    auto now = chrono::system_clock::now();

    return chrono::duration_cast<chrono::seconds>(
               now.time_since_epoch())
        .count();
}

void Database::set(string key, string value)
{
    db[key] = value;
}

string Database::get(string key)
{
    if (isExpired(key))
    {
        db.erase(key);
        expiryTime.erase(key);

        return "Key not found";
    }

    if (db.find(key) != db.end())
    {
        return db[key];
    }

    return "Key not found";
}

bool Database::del(string key)
{
    if (db.find(key) != db.end())
    {
        db.erase(key);
        return true;
    }

    return false;
}

bool Database::exists(string key)
{
    if (isExpired(key))
    {
        db.erase(key);
        expiryTime.erase(key);
    }

    return db.find(key) != db.end();
}

void Database::expire(string key, int seconds)
{
    expiryTime[key] = getCurrentTime() + seconds;
}

bool Database::isExpired(string key)
{
    if (expiryTime.find(key) == expiryTime.end())
    {
        return false;
    }

    long long now = getCurrentTime();

    return now > expiryTime[key];
}

long long Database::ttl(string key)
{
    if (isExpired(key))
    {
        db.erase(key);
        expiryTime.erase(key);

        return -1;
    }

    if (expiryTime.find(key) == expiryTime.end())
    {
        return -1;
    }

    long long now = getCurrentTime();

    return expiryTime[key] - now;
}

void Database::save()
{
    ofstream file("data.txt");

    for (const auto &entry : db)
    {
        if (expiryTime.find(entry.first) != expiryTime.end())
        {
            file << entry.first << " "
                 << entry.second << " "
                 << expiryTime[entry.first] << "\n";
        }
        else
        {
            file << entry.first << " "
                 << entry.second << " "
                 << -1 << "\n";
        }
    }

    file.close();
}

void Database::load()
{
    ifstream file("data.txt");

    string key;
    string value;
    long long expiry;

    while (file >> key >> value >> expiry)
    {
        db[key] = value;

        if (expiry != -1)
        {
            expiryTime[key] = expiry;
        }
    }

    file.close();
}