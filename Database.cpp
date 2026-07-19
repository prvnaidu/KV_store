#include "Database.h"
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

long long getCurrentTime()
{
    auto now = chrono::system_clock::now();

    return chrono::duration_cast<chrono::seconds>(
               now.time_since_epoch())
        .count();
}

static bool isExpiredUnlocked(
    const string &key,
    unordered_map<string, long long> &expiryTime)
{
    auto it = expiryTime.find(key);
    if (it == expiryTime.end())
    {
        return false;
    }
    return getCurrentTime() > it->second;
}

void Database::set(string key, string value)
{
    lock_guard<mutex> lock(dbMutex);
    db[key] = value;
}

string Database::get(string key)
{
    lock_guard<mutex> lock(dbMutex);

    if (isExpiredUnlocked(key, expiryTime))
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
    lock_guard<mutex> lock(dbMutex);

    if (db.find(key) != db.end())
    {
        db.erase(key);
        expiryTime.erase(key);
        return true;
    }

    return false;
}

bool Database::exists(string key)
{
    lock_guard<mutex> lock(dbMutex);

    if (isExpiredUnlocked(key, expiryTime))
    {
        db.erase(key);
        expiryTime.erase(key);
    }

    return db.find(key) != db.end();
}

void Database::expire(string key, int seconds)
{
    lock_guard<mutex> lock(dbMutex);
    expiryTime[key] = getCurrentTime() + seconds;
}

bool Database::isExpired(string key)
{
    lock_guard<mutex> lock(dbMutex);
    return isExpiredUnlocked(key, expiryTime);
}

long long Database::ttl(string key)
{
    lock_guard<mutex> lock(dbMutex);

    if (isExpiredUnlocked(key, expiryTime))
    {
        db.erase(key);
        expiryTime.erase(key);
        return -1;
    }

    auto it = expiryTime.find(key);
    if (it == expiryTime.end())
    {
        return -1;
    }

    return it->second - getCurrentTime();
}

void Database::save()
{
    lock_guard<mutex> lock(dbMutex);

    ofstream file("data.txt");

    for (const auto &entry : db)
    {
        auto it = expiryTime.find(entry.first);
        if (it != expiryTime.end())
        {
            file << entry.first << " "
                 << entry.second << " "
                 << it->second << "\n";
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
    lock_guard<mutex> lock(dbMutex);

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